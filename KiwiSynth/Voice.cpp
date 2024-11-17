#include "Voice.h"



using namespace kiwisynth;



void Voice::Init(int numOscillators, float sampleRate, int voiceNumber)
{
    voiceNumber_ = voiceNumber;
    isNoteTriggered_ = false;
    noteTriggerCount_ = -1;
    currentMidiNote_ = 64;
    currentPlayingNote_ = 64.0f;
    maxOscillators_ = numOscillators;
    numOscillators_ = numOscillators;

    env1_.Init(sampleRate, 0);
    env2_.Init(sampleRate, 1);
    lfo1_.Init(sampleRate, 0);
    lfo2_.Init(sampleRate, 1);
    for (int i = 0; i < numOscillators; i++) {
        Oscillator nextOscillator;
        nextOscillator.Init(sampleRate, i);
        oscillators_.push_back(nextOscillator);
    }
    noise_.Init(sampleRate);
    sampleAndHold_.Init(sampleRate);
    filter_.Init(sampleRate);
    amplifier_.Init();
    initMods();
    prevSourceValues_[SRC_NONE] = 0.0f;
    prevSourceValues_[SRC_FIXED] = 1.0f;

    ParaphonicMode(false);
}



void Voice::UpdateSettings(PatchSettings* patchSettings)
{
    if (patchSettings->getIntValue(OSC_VOICES) != VOICE_MODE_PARA) {
        ParaphonicMode(false);
    }
    env1_.UpdateSettings(patchSettings);
    env2_.UpdateSettings(patchSettings);
    lfo1_.UpdateSettings(patchSettings);
    lfo2_.UpdateSettings(patchSettings);
    oscillators_[0].UpdateSettings(patchSettings);
    oscillators_[1].UpdateSettings(patchSettings);
    oscillators_[2].UpdateSettings(patchSettings);
    noise_.UpdateSettings(patchSettings);
    sampleAndHold_.UpdateSettings(patchSettings);
    filter_.UpdateSettings(patchSettings);
    amplifier_.UpdateSettings(patchSettings);
    baseBalance_ = patchSettings->getFloatValueLinear(GEN_BALANCE, -1.0f, 1.0f);

    isPortamentoOn_ = patchSettings->getBoolValue(PatchSetting::OSC_PORTAMENTO_ON);
    portamentoSpeed_ = patchSettings->getFloatValueLogLookup(PatchSetting::OSC_PORTAMENTO_SPEED);
}



void Voice::initMods()
{
    memset(modValues_, 0, NUM_MOD_DESTINATIONS * sizeof(float));
}



void Voice::calculateMods(Modulation* modulations, Modulation* systemModulations)
{
    modValues_[modulations[0].destination_] += prevSourceValues_[modulations[0].source_] * modulations[0].depth_;
    modValues_[modulations[1].destination_] += prevSourceValues_[modulations[1].source_] * modulations[1].depth_;
    modValues_[modulations[2].destination_] += prevSourceValues_[modulations[2].source_] * modulations[2].depth_;
    modValues_[modulations[3].destination_] += prevSourceValues_[modulations[3].source_] * modulations[3].depth_;
    /*modValues_[modulations[4].destination_] += prevSourceValues_[modulations[4].source_] * modulations[4].depth_;
    modValues_[modulations[5].destination_] += prevSourceValues_[modulations[5].source_] * modulations[5].depth_;
    modValues_[modulations[6].destination_] += prevSourceValues_[modulations[6].source_] * modulations[6].depth_;
    modValues_[modulations[7].destination_] += prevSourceValues_[modulations[7].source_] * modulations[7].depth_;*/

    modValues_[systemModulations[0].destination_] += prevSourceValues_[systemModulations[0].source_] * systemModulations[0].depth_;
    modValues_[systemModulations[1].destination_] += prevSourceValues_[systemModulations[1].source_] * systemModulations[1].depth_;
    modValues_[systemModulations[2].destination_] += prevSourceValues_[systemModulations[2].source_] * systemModulations[2].depth_;
    modValues_[systemModulations[3].destination_] += prevSourceValues_[systemModulations[3].source_] * systemModulations[3].depth_;
    modValues_[systemModulations[4].destination_] += prevSourceValues_[systemModulations[4].source_] * systemModulations[4].depth_;
    modValues_[systemModulations[5].destination_] += prevSourceValues_[systemModulations[5].source_] * systemModulations[5].depth_;

    // The next modulations must be last since they can all be further modified by previous mods
    modValues_[modulations[8].destination_] += prevSourceValues_[modulations[8].source_] * (modulations[8].depth_ + modValues_[DST_LFO_1_TO_MASTER_TUNE]);
    // We are skipping 9 because the note triggering ASDR to amplifer is handled as a special case, but using two loops to
    // avoid having to check an if condition each time and thus save operator executions.
    // Also skipping 10 because filter tracking needs to be handled in a special way.
    modValues_[modulations[11].destination_] += prevSourceValues_[modulations[11].source_] * (modulations[11].depth_ + modValues_[DST_ENV_1_TO_FLT_CUTOFF]);
    modValues_[modulations[12].destination_] += prevSourceValues_[modulations[12].source_] * (modulations[12].depth_ + modValues_[DST_ENV_2_TO_FLT_CUTOFF]);
    modValues_[modulations[13].destination_] += prevSourceValues_[modulations[13].source_] * (modulations[13].depth_ + modValues_[DST_LFO_2_TO_FLT_CUTOFF]);
    modValues_[modulations[14].destination_] += prevSourceValues_[modulations[14].source_] * (modulations[14].depth_ + modValues_[DST_SH_TO_FLT_CUTOFF]);
}



/*
 * All modulations are one sample behind due to circular dependencies.
 */
void Voice::Process(float* sample, PatchSettings* patchSettings, Modulation* modulations, Modulation* systemModulations, int numVoices)
{
    float voiceSample = 0.0f;

    // Triggering notes to play after a delay to shut down previous envelopes click-free (i.e. quickrelease)
    if (__builtin_expect(noteTriggerCount_ == 0, 0)) {
        for (int i = 0; i < numOscillators_; i++) {
            oscillators_[i].midiNote_ = (float)triggerNote_;
        }
        currentMidiNote_ = triggerNote_;
        currentVelocity_ = triggerVelocity_;
        if  (isTriggerResetEnvNeeded_) {
            env1_.SetQuickRelease(false);
            env2_.SetQuickRelease(false);
            env1_.NoteOn();
            env2_.NoteOn();
            lfo1_.NoteOn();
            lfo2_.NoteOn();
        }
    }
    noteTriggerCount_--;

    if (isPortamentoOn_) {
        // Handling portamento
        float fCurrentMidiNote = (float)currentMidiNote_;
        if (currentPlayingNote_ < fCurrentMidiNote) {
            currentPlayingNote_ = std::fmin(currentPlayingNote_ + portamentoSpeed_, fCurrentMidiNote);
        } else if (currentPlayingNote_ > currentMidiNote_) {
            currentPlayingNote_ = std::fmax(currentPlayingNote_ - portamentoSpeed_, fCurrentMidiNote);
        } else {
            currentPlayingNote_ = fCurrentMidiNote;
        }
    } else {
        currentPlayingNote_ = (float)currentMidiNote_;
    }
    oscillators_[0].midiNote_ = currentPlayingNote_;
    oscillators_[1].midiNote_ = currentPlayingNote_;
    oscillators_[2].midiNote_ = currentPlayingNote_;

    // Processing modules
    initMods();
    calculateMods(modulations, systemModulations);

    float env1Sample = 1.0f;
    env1_.Process(&env1Sample, patchSettings, modValues_[DST_ENV_1_ATTACK], modValues_[DST_ENV_1_DECAY], modValues_[DST_ENV_1_SUSTAIN], modValues_[DST_ENV_1_RELEASE]);

    float env2Sample = 1.0f;
    env2_.Process(&env2Sample, patchSettings, modValues_[DST_ENV_2_ATTACK], modValues_[DST_ENV_2_DECAY], modValues_[DST_ENV_2_SUSTAIN], modValues_[DST_ENV_2_RELEASE]);

    float lfo1Sample = 1.0f;
    lfo1_.Process(&lfo1Sample, patchSettings, modValues_[DST_LFO_1_FREQ], modValues_[DST_LFO_1_PULSE_WIDTH], modValues_[DST_LFO_1_TRIGGER_PHASE]);

    float lfo2Sample = 1.0f;
    lfo2_.Process(&lfo2Sample, patchSettings, modValues_[DST_LFO_2_FREQ], modValues_[DST_LFO_2_PULSE_WIDTH], modValues_[DST_LFO_2_TRIGGER_PHASE]);

    if (pmMode_ == PM_MODE_OFF) {
        float oscSample = 0.0f;
        oscillators_[0].Process(&oscSample, patchSettings, modValues_[DST_OSCS_FREQ] + modValues_[DST_OSC_1_FREQ], modValues_[DST_OSCS_PULSE_WIDTH] + modValues_[DST_OSC_1_PULSE_WIDTH], modValues_[DST_OSC_1_LEVEL]);
        voiceSample = voiceSample + oscSample * VOICE_ATTENTUATION_CONSTANT * paraOscMask_[0];

        if (isHardSyncOn_ && oscillators_[0].isEoc_) {
            oscillators_[1].HardSync(oscillators_[0].GetPhaseRatio());
            oscillators_[2].HardSync(oscillators_[0].GetPhaseRatio());
        }

        oscSample = 0.0f;
        oscillators_[1].Process(&oscSample, patchSettings, modValues_[DST_OSCS_FREQ] + modValues_[DST_OSC_2_FREQ], modValues_[DST_OSCS_PULSE_WIDTH] + modValues_[DST_OSC_2_PULSE_WIDTH], modValues_[DST_OSC_2_LEVEL]);
        voiceSample = voiceSample + oscSample * VOICE_ATTENTUATION_CONSTANT * paraOscMask_[1];

        oscSample = 0.0f;
        oscillators_[2].Process(&oscSample, patchSettings, modValues_[DST_OSCS_FREQ] + modValues_[DST_OSC_3_FREQ], modValues_[DST_OSCS_PULSE_WIDTH] + modValues_[DST_OSC_3_PULSE_WIDTH], modValues_[DST_OSC_3_LEVEL]);
        voiceSample = voiceSample + oscSample * VOICE_ATTENTUATION_CONSTANT * paraOscMask_[2];
    } else if (pmMode_ == PM_MODE_PARALLEL) {
        float fmSample = 0.0f;
        oscillators_[2].Process(&fmSample, patchSettings, modValues_[DST_OSCS_FREQ] + modValues_[DST_OSC_3_FREQ], modValues_[DST_OSCS_PULSE_WIDTH] + modValues_[DST_OSC_3_PULSE_WIDTH], modValues_[DST_OSC_1_LEVEL]);
        float fmSample2 = 0.0f;
        oscillators_[1].Process(&fmSample2, patchSettings, modValues_[DST_OSCS_FREQ] + modValues_[DST_OSC_2_FREQ], modValues_[DST_OSCS_PULSE_WIDTH] + modValues_[DST_OSC_2_PULSE_WIDTH], modValues_[DST_OSC_2_LEVEL]);
        oscillators_[0].Process(&voiceSample, patchSettings, modValues_[DST_OSCS_FREQ] + modValues_[DST_OSC_1_FREQ], modValues_[DST_OSCS_PULSE_WIDTH] + modValues_[DST_OSC_1_PULSE_WIDTH], modValues_[DST_OSC_3_LEVEL], fmSample + fmSample2);
    } else if (pmMode_ == PM_MODE_SERIAL) {
        float fmSample = 0.0f;
        oscillators_[2].Process(&fmSample, patchSettings, modValues_[DST_OSCS_FREQ] + modValues_[DST_OSC_3_FREQ], modValues_[DST_OSCS_PULSE_WIDTH] + modValues_[DST_OSC_3_PULSE_WIDTH], modValues_[DST_OSC_1_LEVEL]);
        oscillators_[1].Process(&fmSample, patchSettings, modValues_[DST_OSCS_FREQ] + modValues_[DST_OSC_2_FREQ], modValues_[DST_OSCS_PULSE_WIDTH] + modValues_[DST_OSC_2_PULSE_WIDTH], modValues_[DST_OSC_2_LEVEL], fmSample);
        oscillators_[0].Process(&voiceSample, patchSettings, modValues_[DST_OSCS_FREQ] + modValues_[DST_OSC_1_FREQ], modValues_[DST_OSCS_PULSE_WIDTH] + modValues_[DST_OSC_1_PULSE_WIDTH], modValues_[DST_OSC_3_LEVEL], fmSample);
        voiceSample = voiceSample * VOICE_ATTENTUATION_CONSTANT;
    }

    float sampleAndHoldSample = 0.0f;
    float noiseSample = 0.0f;
    noise_.Process(&noiseSample, patchSettings, modValues_[DST_NOISE_LEVEL], modValues_[DST_NOISE_DENSITY]);
    fclamp(voiceSample = voiceSample + noiseSample, -1.0f, 1.0f);

    if  (isSampleAndHoldAvailable_) {
        sampleAndHoldSample = modValues_[DST_SH_IN];
    }  else {
        sampleAndHoldSample = noise_.GetLastSample();
    }
    sampleAndHold_.Process(&sampleAndHoldSample, patchSettings, modValues_[DST_SH_RATE]);

    filter_.Process(&voiceSample, patchSettings, patchSettings->getFloatValue(FLT_TRACKING) + modValues_[DST_NOTE_TO_FLT_CUTOFF], currentMidiNote_, modValues_[DST_FLT_CUTOFF], modValues_[DST_FLT_RESONANCE]);

    amplifier_.Process(&voiceSample, patchSettings, fclamp((modulations[9].depth_ + modValues_[DST_ENV_1_TO_AMP]) + modValues_[DST_AMP_ENV_1_DEPTH], 0.0f, 1.0f) * prevSourceValues_[SRC_ENV_1], modValues_[DST_AMP_LEVEL]);

    float balance = fclamp(baseBalance_ + modValues_[DST_BALANCE], -1.0f, 1.0f);
    sample[0]  = voiceSample  * std::fmin(1.0f - balance, 1.0f);  // max gain = 1.0, pan = 0: left and right unchanged)
    sample[1] = voiceSample * std::fmin(1.0f + balance, 1.0f);

    // Setting up source values for the next round of modulations. We must modulate based on the previous
    // sample because of possible circular dependencies otherwise.
    // No need to change SRC_NONE nor SRC_FIXED from the init values. They can't be modulated.
    prevSourceValues_[SRC_LFO_1] = lfo1Sample;
    prevSourceValues_[SRC_LFO_2] = lfo2Sample;
    prevSourceValues_[SRC_ENV_1] = env1Sample;
    prevSourceValues_[SRC_ENV_2] = env2Sample;
    prevSourceValues_[SRC_SH] = sampleAndHoldSample;
    prevSourceValues_[SRC_NOTE] = currentMidiNote_;
    prevSourceValues_[SRC_VELOCITY] = (float)currentVelocity_ * 0.00787401574803149606f; // same as / 127.0f
    prevSourceValues_[SRC_AFTERTOUCH] = patchSettings->getFloatValue(GEN_AFTERTOUCH);
    prevSourceValues_[SRC_MOD_WHEEL] = patchSettings->getFloatValue(GEN_MOD_WHEEL);
    prevSourceValues_[SRC_PITCH_BEND] = patchSettings->getFloatValue(GEN_PITCH_BEND);
    prevSourceValues_[SRC_EXPRESSION] = patchSettings->getFloatValue(GEN_EXPRESSION);
    prevSourceValues_[SRC_SUSTAIN] = patchSettings->getFloatValue(GEN_SUSTAIN);
    prevSourceValues_[SRC_VOICE_NO] = (float)voiceNumber_ / 2.0f; // Assuming three voices max -- voice 0 is 0.0f, voice 1 is 0.5f, voice 2 is 1.0f
}



bool Voice::IsAvailable()
{
    return !env1_.IsPlaying() && !isNoteTriggered_;
}



bool Voice::IsReleasing()
{
    return env1_.IsReleasing();
}



void Voice::ParaNoteOn(int oscillator, uint8_t note, uint8_t velocity) { // Use paraOffset to set the note with respect to 0
    oscillators_[oscillator].paraOffset_ = (float)note;
    if (IsAvailable() || (IsReleasing() && !isNoteTriggered_)) {
        paraOscMask_[0] = 0.0f; // Make sure all oscillators are turned off since sometimes one is left on for release.
        paraOscMask_[1] = 0.0f;
        paraOscMask_[2] = 0.0f;
        NoteOn(0, velocity, true);
    }
    paraOscMask_[oscillator] = 1.0f;
}



void Voice::ParaNoteOff(int oscillator, bool noteOff) {
    if (noteOff) {
        // We are leaving oscillator masks on for the release so we can not assume they will all be off in ParaNoteOn
        if (env1_.isNoteTriggered_) {
            NoteOff(0, 0);
        }
    } else {
        paraOscMask_[oscillator] = 0.0f;
    }
}



void Voice::ParaAllNotesOff() {
    paraOscMask_[0] = 0.0f;
    paraOscMask_[1] = 0.0f;
    paraOscMask_[2] = 0.0f;
}



void Voice::NoteOn(int note, int velocity, bool reset)
{
    if (currentMidiNote_ != note && reset) {
        env1_.SetQuickRelease(true);
        env2_.SetQuickRelease(true);
    }
    if (!env1_.IsPlaying()) {
        triggerVelocity_ = velocity;
    }
    noteTriggerCount_ = NOTE_TRIGGER_SAMPLES;
    triggerNote_ = note;
    isNoteTriggered_ = true;
    isTriggerResetEnvNeeded_ = reset;
}



void Voice::NoteOff(int note, int velocity)
{
    isNoteTriggered_ = false;
    env1_.NoteOff();
    env2_.NoteOff();
}
