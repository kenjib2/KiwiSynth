#include "Voice.h"

namespace kiwi_synth
{
    void Voice::Init(int numVCOs, PatchSettings* patchSettings, float sampleRate)
    {
        fullFunctionality = true;
        noteTriggered = false;
        noteTriggerCount = -1;
        noteOffNeeded = false;
        currentMidiNote = 64;
        currentMidiNote = 64.0f;
        currentPlayingNote = 64.0f;
        this->numVcos = numVCOs;
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
    }

    void Voice::UpdateSettings()
    {
        for (int i = 0; i < numVcos; i++) {
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
        portamentoSpeed = patchSettings->getFloatValue(PatchSetting::VCO_PORTAMENTO_SPEED, 0.0001F, 0.05F, Scale::LOGARHITHMIC);
    }

    void Voice::initMods()
    {
        memset(modValues, 0, NUM_MOD_DESTINATIONS * sizeof(float));
    }

    void Voice::calculateMods(Modulation* modulations)
    {
        for (int i = 0; i < 9; i++) {
            if (modulations[i].destination >= 0 && modulations[i].destination < NUM_MOD_DESTINATIONS 
                && modulations[i].source >= 0 && modulations[i].source < NUM_MOD_SOURCES)
            modValues[modulations[i].destination] += getModValue(modulations[i].source, modulations[i].depth);
        }
        // We are skipping 9 because the note triggering ASDR to VCA is handled as a special case, but using two loops to
        // avoid having to check an if condition each time and thus save operator executions.
        for (int i = 10; i < NUM_MODULATIONS; i++) {
            if (modulations[i].destination >= 0 && modulations[i].destination < NUM_MOD_DESTINATIONS 
                && modulations[i].source >= 0 && modulations[i].source < NUM_MOD_SOURCES)
            modValues[modulations[i].destination] += getModValue(modulations[i].source, modulations[i].depth);
        }
    }

    float Voice::getModValue(ModulationSource source, float depth)
    {
        switch (source) {
            case (SRC_NONE):
                return 0.0f;
                break;
            case (SRC_FIXED):
                return depth;
                break;
            default:
                return prevSourceValues[source] * depth;
                break;
        }
    }

    /*
     * All modulations are one sample behind due to circular dependencies.
     */
    void Voice::Process(float* sample, Modulation* modulations)
    {
        // Triggering notes to play
        if (noteTriggerCount > 0) {
            noteTriggerCount--;
        } else if (noteTriggerCount == 0) {
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

        // Turning off notes
        if (noteOffNeeded) {
            noteOffNeeded = false;
            noteTriggered = false;
            env1.NoteOff();
            env2.NoteOff();
        }

        // Handling portamento
        float fCurrentMidiNote = (float)currentMidiNote;
        if (portamentoOn && currentPlayingNote < fCurrentMidiNote) {
            currentPlayingNote = std::fmin(currentPlayingNote + portamentoSpeed, fCurrentMidiNote);
        } else if (portamentoOn && currentPlayingNote > currentMidiNote) {
            currentPlayingNote = std::fmax(currentPlayingNote - portamentoSpeed, fCurrentMidiNote);
        } else {
            currentPlayingNote = fCurrentMidiNote;
        }
        for (int i = 0; i < numVcos; i++) {
            vcos[i].SetFreq(mtof(currentPlayingNote));
        }

        // Processing modules
        initMods();
        calculateMods(modulations);

        float env1Sample = 1.0f;
        env1.Process(&env1Sample);

        float env2Sample = 1.0f;
        env2.Process(&env2Sample);

        float lfo1Sample = 1.0f;
        lfo1.Process(&lfo1Sample);

        float lfo2Sample = 1.0f;
        lfo2.Process(&lfo2Sample);

        for (int i = 0; i < numVcos; i++) {
            float vcoSample = 0.0f;
            vcos[i].Process(&vcoSample, modValues[DST_VCOS_FREQ] + modValues[DST_VCO_1_FREQ + 2 * i]);
            *sample = *sample + vcoSample * VOICE_ATTENTUATION_CONSTANT;
        }
        if (fullFunctionality) {
            float noiseSample = 0.0f;
            noise.Process(&noiseSample, nullptr, 0);
            *sample = *sample + noiseSample * VOICE_ATTENTUATION_CONSTANT;
        }

        vca.Process(sample, modulations[9].depth * prevSourceValues[SRC_ENV_1], modValues[DST_VCA_LEVEL]);

        numMods = 4;
        mods[0] = mtof(currentMidiNote);
        mods[1] = env1Sample;
        mods[2] = env2Sample;
        mods[3] = lfo2Sample;
        float sampleAndHoldSample = 0.0f;
        if (fullFunctionality) {
            sampleAndHoldSample = noise.GetLastSample();
            sampleAndHold.Process(&sampleAndHoldSample);
            numMods = 5;
            mods[4] = sampleAndHoldSample * patchSettings->getFloatValue(SH_TO_VCF_CUTOFF);
        }
        vcf.Process(sample, mods, numMods);

        // Setting up source values for the next round of modulations. We must modulate based on the previous
        // sample because of possible circular dependencies otherwise.
        // No need to set SRC_NONE nor SRC_FIXED. They can't be modulated.
        prevSourceValues[SRC_LFO_1] = lfo1Sample;
        prevSourceValues[SRC_LFO_2] = lfo2Sample;
        prevSourceValues[SRC_ENV_1] = env1Sample;
        prevSourceValues[SRC_ENV_2] = env2Sample;
        prevSourceValues[SRC_SH] = sampleAndHoldSample;
        prevSourceValues[SRC_NOTE] = currentMidiNote;
        prevSourceValues[SRC_VELOCITY] = currentVelocity;
        prevSourceValues[SRC_AFTERTOUCH] = 0.0f;
        prevSourceValues[SRC_MOD_WHEEL] = 0.0f;
        prevSourceValues[SRC_PITCH_BEND] = 0.0f;
        prevSourceValues[SRC_EXPRESSION] = 0.0f;
        prevSourceValues[SRC_PEDAL] = 0.0f;
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
        noteTriggerCount = NOTE_TRIGGER_SAMPLES;
        triggerNote = note;
        triggerVelocity = velocity;
        noteTriggered = true;
    }

    void Voice::NoteOff(int note, int velocity)
    {
        noteOffNeeded = true;
    }

}
