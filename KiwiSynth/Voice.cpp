#include "Voice.h"

namespace kiwi_synth
{
    void Voice::Init(int numVCOs, PatchSettings* patchSettings, float sampleRate, int voiceNumber)
    {
        this->voiceNumber = voiceNumber;
        #ifdef __FUNCTIONALITY_OPTION__
        fullFunctionality = true;
        #endif // __FUNCTIONALITY_OPTION__
        mono = false;
        noteTriggered = false;
        noteTriggerCount = -1;
        noteOffNeeded = false;
        currentMidiNote = 64;
        currentPlayingNote = 64.0f;
        this->maxVcos = numVCOs;
        this->numVcos = maxVcos;
        this->patchSettings = patchSettings;
        for (int i = 0; i < numVCOs; i++) {
            VCO nextVco;
            nextVco.Init(patchSettings, sampleRate, i);
            vcos.push_back(nextVco);
        }
        noise.Init(patchSettings, sampleRate);
        sampleAndHold.Init(patchSettings, sampleRate);
        vcf.Init(patchSettings, sampleRate);
        vca.Init(patchSettings, sampleRate);
        env1.Init(patchSettings, sampleRate, 0);
        env2.Init(patchSettings, sampleRate, 1);
        lfo1.Init(patchSettings, sampleRate, 0);
        lfo2.Init(patchSettings, sampleRate, 1);
        initMods();
        prevSourceValues[SRC_NONE] = 0.0f;
        prevSourceValues[SRC_FIXED] = 1.0f;
    }

    void Voice::UpdateSettings()
    {
        for (int i = 0; i < maxVcos; i++) {
            vcos[i].UpdateSettings();
        }
        noise.UpdateSettings();
        sampleAndHold.UpdateSettings();
        vca.UpdateSettings();
        vcf.UpdateSettings();

        env1.UpdateSettings();
        env2.UpdateSettings();
        lfo1.UpdateSettings();
        lfo2.UpdateSettings();

        portamentoOn = patchSettings->getBoolValue(PatchSetting::VCO_PORTAMENTO_ON);
        portamentoSpeed = patchSettings->getFloatValueLogLookup(PatchSetting::VCO_PORTAMENTO_SPEED);
    }

    void Voice::initMods()
    {
        memset(modValues, 0, NUM_MOD_DESTINATIONS * sizeof(float));
    }

    void Voice::calculateMods(Modulation* modulations)
    {
        #ifdef __FUNCTIONALITY_OPTION__
        if (fullFunctionality) {
        #endif // __FUNCTIONALITY_OPTION__
            modValues[modulations[0].destination] += prevSourceValues[modulations[0].source] * modulations[0].depth;
            modValues[modulations[1].destination] += prevSourceValues[modulations[1].source] * modulations[1].depth;
            modValues[modulations[2].destination] += prevSourceValues[modulations[2].source] * modulations[2].depth;
            modValues[modulations[3].destination] += prevSourceValues[modulations[3].source] * modulations[3].depth;
            modValues[modulations[4].destination] += prevSourceValues[modulations[4].source] * modulations[4].depth;
            modValues[modulations[5].destination] += prevSourceValues[modulations[5].source] * modulations[5].depth;
            modValues[modulations[6].destination] += prevSourceValues[modulations[6].source] * modulations[6].depth;
            modValues[modulations[7].destination] += prevSourceValues[modulations[7].source] * modulations[7].depth;
        #ifdef __FUNCTIONALITY_OPTION__
        }
        #endif // __FUNCTIONALITY_OPTION__

        modValues[modulations[8].destination] += prevSourceValues[modulations[8].source] * modulations[8].depth;
        // We are skipping 9 because the note triggering ASDR to VCA is handled as a special case, but using two loops to
        // avoid having to check an if condition each time and thus save operator executions.
        // Also skipping 10 because VCF tracking needs to be handled in a special way.
        modValues[modulations[11].destination] += prevSourceValues[modulations[11].source] * modulations[11].depth;
        modValues[modulations[12].destination] += prevSourceValues[modulations[12].source] * modulations[12].depth;
        modValues[modulations[13].destination] += prevSourceValues[modulations[13].source] * modulations[13].depth;
        modValues[modulations[14].destination] += prevSourceValues[modulations[14].source] * modulations[14].depth;
    }

    /*
     * All modulations are one sample behind due to circular dependencies.
     */
    void Voice::Process(float* sample, Modulation* modulations, int numVoices)
    {
        float voiceSample = 0.0f;

        // Triggering notes to play after a delay to shut down previous envelopes click-free (i.e. quickrelease)
        if (noteTriggerCount >= 0) {
            if (noteTriggerCount == 0) {
                noteTriggerCount = -1;
                currentMidiNote = triggerNote;
                currentVelocity = triggerVelocity;
                env1.SetQuickRelease(false);
                env2.SetQuickRelease(false);
                env1.NoteOn();
                env2.NoteOn();
                lfo1.NoteOn();
                lfo2.NoteOn();
            }
            noteTriggerCount--;
        }

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
        for (int i = 0; i < maxVcos; i++) {
            vcos[i].midiNote = currentPlayingNote;
        }

        // Processing modules
        initMods();
        calculateMods(modulations);

        #ifdef __FUNCTIONALITY_OPTION__
        float env1Sample = 1.0f;
        env1.Process(&env1Sample, modValues[DST_ENV_1_ATTACK], modValues[DST_ENV_1_DECAY], modValues[DST_ENV_1_SUSTAIN], modValues[DST_ENV_1_RELEASE], fullFunctionality);

        float env2Sample = 1.0f;
        env2.Process(&env2Sample, modValues[DST_ENV_2_ATTACK], modValues[DST_ENV_2_DECAY], modValues[DST_ENV_2_SUSTAIN], modValues[DST_ENV_2_RELEASE], fullFunctionality);

        float lfo1Sample = 1.0f;
        lfo1.Process(&lfo1Sample, modValues[DST_LFO_1_FREQ], modValues[DST_LFO_1_PULSE_WIDTH], modValues[DST_LFO_1_TRIGGER_PHASE], fullFunctionality);

        float lfo2Sample = 1.0f;
        lfo2.Process(&lfo2Sample, modValues[DST_LFO_2_FREQ], modValues[DST_LFO_2_PULSE_WIDTH], modValues[DST_LFO_2_TRIGGER_PHASE], fullFunctionality);

        for (int i = 0; i < numVcos; i++) {
            float vcoSample = 0.0f;
            vcos[i].Process(&vcoSample, modValues[DST_VCOS_FREQ] + modValues[DST_VCO_1_FREQ + 2 * i], modValues[DST_VCOS_PULSE_WIDTH] + modValues[DST_VCO_1_PULSE_WIDTH + 2 * i], fullFunctionality);
            voiceSample = voiceSample + vcoSample * VOICE_ATTENTUATION_CONSTANT;
        }
        #else
        float env1Sample = 1.0f;
        env1.Process(&env1Sample, modValues[DST_ENV_1_ATTACK], modValues[DST_ENV_1_DECAY], modValues[DST_ENV_1_SUSTAIN], modValues[DST_ENV_1_RELEASE]);

        float env2Sample = 1.0f;
        env2.Process(&env2Sample, modValues[DST_ENV_2_ATTACK], modValues[DST_ENV_2_DECAY], modValues[DST_ENV_2_SUSTAIN], modValues[DST_ENV_2_RELEASE]);

        float lfo1Sample = 1.0f;
        lfo1.Process(&lfo1Sample, modValues[DST_LFO_1_FREQ], modValues[DST_LFO_1_PULSE_WIDTH], modValues[DST_LFO_1_TRIGGER_PHASE]);

        float lfo2Sample = 1.0f;
        lfo2.Process(&lfo2Sample, modValues[DST_LFO_2_FREQ], modValues[DST_LFO_2_PULSE_WIDTH], modValues[DST_LFO_2_TRIGGER_PHASE]);

        float vcoSample = 0.0f;
        vcos[0].Process(&vcoSample, modValues[DST_VCOS_FREQ] + modValues[DST_VCO_1_FREQ], modValues[DST_VCOS_PULSE_WIDTH] + modValues[DST_VCO_1_PULSE_WIDTH]);
        voiceSample = voiceSample + vcoSample * VOICE_ATTENTUATION_CONSTANT;

        vcoSample = 0.0f;
        vcos[1].Process(&vcoSample, modValues[DST_VCOS_FREQ] + modValues[DST_VCO_2_FREQ], modValues[DST_VCOS_PULSE_WIDTH] + modValues[DST_VCO_2_PULSE_WIDTH]);
        voiceSample = voiceSample + vcoSample * VOICE_ATTENTUATION_CONSTANT;

        vcoSample = 0.0f;
        vcos[2].Process(&vcoSample, modValues[DST_VCOS_FREQ] + modValues[DST_VCO_3_FREQ], modValues[DST_VCOS_PULSE_WIDTH] + modValues[DST_VCO_3_PULSE_WIDTH]);
        voiceSample = voiceSample + vcoSample * VOICE_ATTENTUATION_CONSTANT;
        #endif // __FUNCTIONALITY_OPTION__

        float sampleAndHoldSample = 0.0f;
        #ifdef __FUNCTIONALITY_OPTION__
        if (fullFunctionality) {
        #endif // __FUNCTIONALITY_OPTION__
            float noiseSample = 0.0f;
            noise.Process(&noiseSample, modValues[DST_NOISE_LEVEL]);
            fclamp(voiceSample = voiceSample + noiseSample, -1.0f, 1.0f);

            sampleAndHoldSample = noise.GetLastSample();
            sampleAndHold.Process(&sampleAndHoldSample, modValues[DST_SH_RATE]);
        #ifdef __FUNCTIONALITY_OPTION__
        }
        #endif // __FUNCTIONALITY_OPTION__

        vcf.Process(&voiceSample, patchSettings->getFloatValue(VCF_TRACKING), currentMidiNote, modValues[DST_VCF_CUTOFF], modValues[DST_VCF_RESONANCE]);

        vca.Process(&voiceSample, fclamp(modulations[9].depth + modValues[DST_VCA_ENV_1_DEPTH], 0.0f, 1.0f) * prevSourceValues[SRC_ENV_1], modValues[DST_VCA_LEVEL]);

        float balance = fclamp(patchSettings->getFloatValueLinear(GEN_BALANCE, -1.0f, 1.0f) + modValues[DST_BALANCE], -1.0f, 1.0f);
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
        prevSourceValues[SRC_VELOCITY] = (float)currentVelocity / 127.0f;
        prevSourceValues[SRC_AFTERTOUCH] = patchSettings->getFloatValue(GEN_AFTERTOUCH);
        prevSourceValues[SRC_MOD_WHEEL] = patchSettings->getFloatValue(GEN_MOD_WHEEL);
        prevSourceValues[SRC_PITCH_BEND] = patchSettings->getFloatValue(GEN_PITCH_BEND);
        prevSourceValues[SRC_EXPRESSION] = patchSettings->getFloatValue(GEN_EXPRESSION);
        prevSourceValues[SRC_SUSTAIN] = patchSettings->getFloatValue(GEN_SUSTAIN);
        prevSourceValues[SRC_VOICE_NO] = (float)voiceNumber / (float)(numVoices - 1);
    }

    bool Voice::IsAvailable()
    {
        return !env1.IsPlaying() && !noteTriggered;
    }


    bool Voice::IsReleasing()
    {
        return env1.IsReleasing();
    }

    void Voice::NoteOn(int note, int velocity, bool reset)
    {
        if (currentMidiNote != note && reset) {
            env1.SetQuickRelease(true);
            env2.SetQuickRelease(true);
        }
        for (int i = 0; i < numVcos; i++) {
            vcos[i].midiNote = (float)note;
        }
        noteTriggerCount = NOTE_TRIGGER_SAMPLES;
        triggerNote = note;
        triggerVelocity = velocity;
        noteTriggered = true;
    }

    void Voice::NoteOff(int note, int velocity)
    {
        //noteOffNeeded = true;
        noteOffNeeded = false;
        noteTriggered = false;
        env1.NoteOff();
        env2.NoteOff();
    }

}
