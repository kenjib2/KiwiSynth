#include "Voice.h"

namespace kiwi_synth
{
    void Voice::Init(int numVCOs, float sampleRate, int voiceNumber)
    {
        this->voiceNumber = voiceNumber;
        noteTriggered = false;
        noteTriggerCount = -1;
        currentMidiNote = 64;
        currentPlayingNote = 64.0f;
        this->maxVcos = numVCOs;
        this->numVcos = maxVcos;

        env1.Init(sampleRate, 0);
        env2.Init(sampleRate, 1);
        lfo1.Init(sampleRate, 0);
        lfo2.Init(sampleRate, 1);
        for (int i = 0; i < numVCOs; i++) {
            VCO nextVco;
            nextVco.Init(sampleRate, i);
            vcos.push_back(nextVco);
        }
        noise.Init(sampleRate);
        sampleAndHold.Init(sampleRate);
        vcf.Init(sampleRate);
        vca.Init();
        initMods();
        prevSourceValues[SRC_NONE] = 0.0f;
        prevSourceValues[SRC_FIXED] = 1.0f;

        ParaphonicMode(false);
    }

    void Voice::UpdateSettings(PatchSettings* patchSettings)
    {
        if (patchSettings->getIntValue(VCO_VOICES) != VOICE_MODE_PARA) {
            ParaphonicMode(false);
        }
        env1.UpdateSettings(patchSettings);
        env2.UpdateSettings(patchSettings);
        lfo1.UpdateSettings(patchSettings);
        lfo2.UpdateSettings(patchSettings);
        vcos[0].UpdateSettings(patchSettings);
        vcos[1].UpdateSettings(patchSettings);
        vcos[2].UpdateSettings(patchSettings);
        noise.UpdateSettings(patchSettings);
        sampleAndHold.UpdateSettings(patchSettings);
        vcf.UpdateSettings(patchSettings);
        vca.UpdateSettings(patchSettings);
        baseBalance = patchSettings->getFloatValueLinear(GEN_BALANCE, -1.0f, 1.0f);

        portamentoOn = patchSettings->getBoolValue(PatchSetting::VCO_PORTAMENTO_ON);
        portamentoSpeed = patchSettings->getFloatValueLogLookup(PatchSetting::VCO_PORTAMENTO_SPEED);
    }

    void Voice::initMods()
    {
        memset(modValues, 0, NUM_MOD_DESTINATIONS * sizeof(float));
    }

    void Voice::calculateMods(Modulation* modulations, Modulation* systemModulations)
    {
        modValues[modulations[0].destination] += prevSourceValues[modulations[0].source] * modulations[0].depth;
        modValues[modulations[1].destination] += prevSourceValues[modulations[1].source] * modulations[1].depth;
        modValues[modulations[2].destination] += prevSourceValues[modulations[2].source] * modulations[2].depth;
        modValues[modulations[3].destination] += prevSourceValues[modulations[3].source] * modulations[3].depth;
        modValues[modulations[4].destination] += prevSourceValues[modulations[4].source] * modulations[4].depth;
        modValues[modulations[5].destination] += prevSourceValues[modulations[5].source] * modulations[5].depth;
        modValues[modulations[6].destination] += prevSourceValues[modulations[6].source] * modulations[6].depth;
        modValues[modulations[7].destination] += prevSourceValues[modulations[7].source] * modulations[7].depth;

        // The next modulations can all be further modified by previous mods
        modValues[modulations[8].destination] += prevSourceValues[modulations[8].source] * (modulations[8].depth + modValues[DST_LFO_1_TO_MASTER_TUNE]);
        // We are skipping 9 because the note triggering ASDR to VCA is handled as a special case, but using two loops to
        // avoid having to check an if condition each time and thus save operator executions.
        // Also skipping 10 because VCF tracking needs to be handled in a special way.
        modValues[modulations[11].destination] += prevSourceValues[modulations[11].source] * (modulations[11].depth + modValues[DST_ENV_1_TO_VCF_CUTOFF]);
        modValues[modulations[12].destination] += prevSourceValues[modulations[12].source] * (modulations[12].depth + modValues[DST_ENV_2_TO_VCF_CUTOFF]);
        modValues[modulations[13].destination] += prevSourceValues[modulations[13].source] * (modulations[13].depth + modValues[DST_LFO_2_TO_VCF_CUTOFF]);
        modValues[modulations[14].destination] += prevSourceValues[modulations[14].source] * (modulations[14].depth + modValues[DST_SH_TO_VCF_CUTOFF]);

        modValues[systemModulations[0].destination] += prevSourceValues[systemModulations[0].source] * systemModulations[0].depth;
        modValues[systemModulations[1].destination] += prevSourceValues[systemModulations[1].source] * systemModulations[1].depth;
        modValues[systemModulations[2].destination] += prevSourceValues[systemModulations[2].source] * systemModulations[2].depth;
        modValues[systemModulations[3].destination] += prevSourceValues[systemModulations[3].source] * systemModulations[3].depth;
        modValues[systemModulations[4].destination] += prevSourceValues[systemModulations[4].source] * systemModulations[4].depth;
        modValues[systemModulations[5].destination] += prevSourceValues[systemModulations[5].source] * systemModulations[5].depth;
    }

    /*
     * All modulations are one sample behind due to circular dependencies.
     */
    void Voice::Process(float* sample, PatchSettings* patchSettings, Modulation* modulations, Modulation* systemModulations, int numVoices)
    {
        float voiceSample = 0.0f;

        // Triggering notes to play after a delay to shut down previous envelopes click-free (i.e. quickrelease)
        if (__builtin_expect(noteTriggerCount == 0, 0)) {
            for (int i = 0; i < numVcos; i++) {
                vcos[i].midiNote = (float)triggerNote;
            }
            currentMidiNote = triggerNote;
            currentVelocity = triggerVelocity;
            if  (triggerResetEnv) {
                env1.SetQuickRelease(false);
                env2.SetQuickRelease(false);
                env1.NoteOn();
                env2.NoteOn();
                lfo1.NoteOn();
                lfo2.NoteOn();
            }
        }
        noteTriggerCount--;

        if (portamentoOn) {
            // Handling portamento
            float fCurrentMidiNote = (float)currentMidiNote;
            if (currentPlayingNote < fCurrentMidiNote) {
                currentPlayingNote = std::fmin(currentPlayingNote + portamentoSpeed, fCurrentMidiNote);
            } else if (currentPlayingNote > currentMidiNote) {
                currentPlayingNote = std::fmax(currentPlayingNote - portamentoSpeed, fCurrentMidiNote);
            } else {
                currentPlayingNote = fCurrentMidiNote;
            }
        } else {
            currentPlayingNote = (float)currentMidiNote;
        }
        vcos[0].midiNote = currentPlayingNote;
        vcos[1].midiNote = currentPlayingNote;
        vcos[2].midiNote = currentPlayingNote;

        // Processing modules
        initMods();
        calculateMods(modulations, systemModulations);

        float env1Sample = 1.0f;
        env1.Process(&env1Sample, patchSettings, modValues[DST_ENV_1_ATTACK], modValues[DST_ENV_1_DECAY], modValues[DST_ENV_1_SUSTAIN], modValues[DST_ENV_1_RELEASE]);

        float env2Sample = 1.0f;
        env2.Process(&env2Sample, patchSettings, modValues[DST_ENV_2_ATTACK], modValues[DST_ENV_2_DECAY], modValues[DST_ENV_2_SUSTAIN], modValues[DST_ENV_2_RELEASE]);

        float lfo1Sample = 1.0f;
        lfo1.Process(&lfo1Sample, patchSettings, modValues[DST_LFO_1_FREQ], modValues[DST_LFO_1_PULSE_WIDTH], modValues[DST_LFO_1_TRIGGER_PHASE]);

        float lfo2Sample = 1.0f;
        lfo2.Process(&lfo2Sample, patchSettings, modValues[DST_LFO_2_FREQ], modValues[DST_LFO_2_PULSE_WIDTH], modValues[DST_LFO_2_TRIGGER_PHASE]);

        float vcoSample = 0.0f;
        vcos[0].Process(&vcoSample, patchSettings, modValues[DST_VCOS_FREQ] + modValues[DST_VCO_1_FREQ], modValues[DST_VCOS_PULSE_WIDTH] + modValues[DST_VCO_1_PULSE_WIDTH]);
        voiceSample = voiceSample + vcoSample * VOICE_ATTENTUATION_CONSTANT * paraVcoMask[0];

        vcoSample = 0.0f;
        vcos[1].Process(&vcoSample, patchSettings, modValues[DST_VCOS_FREQ] + modValues[DST_VCO_2_FREQ], modValues[DST_VCOS_PULSE_WIDTH] + modValues[DST_VCO_2_PULSE_WIDTH]);
        voiceSample = voiceSample + vcoSample * VOICE_ATTENTUATION_CONSTANT * paraVcoMask[1];

        vcoSample = 0.0f;
        vcos[2].Process(&vcoSample, patchSettings, modValues[DST_VCOS_FREQ] + modValues[DST_VCO_3_FREQ], modValues[DST_VCOS_PULSE_WIDTH] + modValues[DST_VCO_3_PULSE_WIDTH]);
        voiceSample = voiceSample + vcoSample * VOICE_ATTENTUATION_CONSTANT * paraVcoMask[2];

        float sampleAndHoldSample = 0.0f;
        float noiseSample = 0.0f;
        noise.Process(&noiseSample, patchSettings, modValues[DST_NOISE_LEVEL], modValues[DST_NOISE_DENSITY]);
        fclamp(voiceSample = voiceSample + noiseSample, -1.0f, 1.0f);

        sampleAndHoldSample = noise.GetLastSample();
        sampleAndHold.Process(&sampleAndHoldSample, patchSettings, modValues[DST_SH_RATE]);

        vcf.Process(&voiceSample, patchSettings, patchSettings->getFloatValue(VCF_TRACKING) + modValues[DST_NOTE_TO_VCF_CUTOFF], currentMidiNote, modValues[DST_VCF_CUTOFF], modValues[DST_VCF_RESONANCE]);

        vca.Process(&voiceSample, patchSettings, fclamp((modulations[9].depth + modValues[DST_ENV_1_TO_VCA]) + modValues[DST_VCA_ENV_1_DEPTH], 0.0f, 1.0f) * prevSourceValues[SRC_ENV_1], modValues[DST_VCA_LEVEL]);

        float balance = fclamp(baseBalance + modValues[DST_BALANCE], -1.0f, 1.0f);
        sample[0]  = voiceSample  * std::fmin(1.0f - balance, 1.0f);  // max gain = 1.0, pan = 0: left and right unchanged)
        sample[1] = voiceSample * std::fmin(1.0f + balance, 1.0f);

        // Setting up source values for the next round of modulations. We must modulate based on the previous
        // sample because of possible circular dependencies otherwise.
        // No need to change SRC_NONE nor SRC_FIXED from the init values. They can't be modulated.
        prevSourceValues[SRC_LFO_1] = lfo1Sample;
        prevSourceValues[SRC_LFO_2] = lfo2Sample;
        prevSourceValues[SRC_ENV_1] = env1Sample;
        prevSourceValues[SRC_ENV_2] = env2Sample;
        prevSourceValues[SRC_SH] = sampleAndHoldSample;
        prevSourceValues[SRC_NOTE] = currentMidiNote;
        prevSourceValues[SRC_VELOCITY] = (float)currentVelocity * 0.00787401574803149606f; // same as / 127.0f
        prevSourceValues[SRC_AFTERTOUCH] = patchSettings->getFloatValue(GEN_AFTERTOUCH);
        prevSourceValues[SRC_MOD_WHEEL] = patchSettings->getFloatValue(GEN_MOD_WHEEL);
        prevSourceValues[SRC_PITCH_BEND] = patchSettings->getFloatValue(GEN_PITCH_BEND);
        prevSourceValues[SRC_EXPRESSION] = patchSettings->getFloatValue(GEN_EXPRESSION);
        prevSourceValues[SRC_SUSTAIN] = patchSettings->getFloatValue(GEN_SUSTAIN);
        prevSourceValues[SRC_VOICE_NO] = (float)voiceNumber; // Assuming two voices max -- voice 0 is 0.0f, voice 1 is 1.0f
    }

    bool Voice::IsAvailable()
    {
        return !env1.IsPlaying() && !noteTriggered;
    }


    bool Voice::IsReleasing()
    {
        return env1.IsReleasing();
    }

    void Voice::ParaNoteOn(int vco, uint8_t note, uint8_t velocity) { // Use paraOffset to set the note with respect to 0
        vcos[vco].paraOffset = (float)note;
        if (IsAvailable() || (IsReleasing() && !noteTriggered)) {
            paraVcoMask[0] = 0.0f; // Make sure all vcos are turned off since sometimes one is left on for release.
            paraVcoMask[1] = 0.0f;
            paraVcoMask[2] = 0.0f;
            NoteOn(0, velocity, false);
        }
        paraVcoMask[vco] = 1.0f;
    }

    void Voice::ParaNoteOff(int vco, bool noteOff) {
        if (noteOff) {
            // We are leaving vco masks on for the release so we can not assume they will all be off in ParaNoteOn
            if (env1.noteTriggered) {
                NoteOff(0, 0);
            }
        } else {
            paraVcoMask[vco] = 0.0f;
        }
    }

    void Voice::ParaAllNotesOff() {
        paraVcoMask[0] = 0.0f;
        paraVcoMask[1] = 0.0f;
        paraVcoMask[2] = 0.0f;
    }

    void Voice::NoteOn(int note, int velocity, bool reset)
    {
        if (currentMidiNote != note && reset) {
            env1.SetQuickRelease(true);
            env2.SetQuickRelease(true);
        }
        noteTriggerCount = NOTE_TRIGGER_SAMPLES;
        triggerNote = note;
        triggerVelocity = velocity;
        noteTriggered = true;
        triggerResetEnv = reset;
    }

    void Voice::NoteOff(int note, int velocity)
    {
        noteTriggered = false;
        env1.NoteOff();
        env2.NoteOff();
    }

}
