#include "Voice.h"

namespace kiwi_synth
{
    void Voice::Init(int numVCOs, PatchSettings* patchSettings, float sampleRate)
    {
        noteTriggered = false;
        noteTriggerCount = -1;
        noteOffNeeded = false;
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
    }

    void Voice::Process(float* sample)
    {
        if (noteTriggerCount > 0) {
            noteTriggerCount--;
        } else if (noteTriggerCount == 0) {
            noteTriggerCount = -1;

            currentMidiNote = triggerNote;
            for (int i = 0; i < numVcos; i++) {
                vcos[i].SetFreq(mtof(triggerNote));
            }
            env1.SetQuickRelease(false);
            env2.SetQuickRelease(false);
            env1.NoteOn();
            env2.NoteOn();
            lfo1.NoteOn();
            lfo2.NoteOn();
        }

        if (noteOffNeeded) {
            noteOffNeeded = false;
            noteTriggered = false;
            env1.NoteOff();
            env2.NoteOff();
        }

        float mods[MAX_MODS];
        uint8_t numMods = 0;

        float env1Sample = 1.0f;
        env1.Process(&env1Sample);

        float env2Sample = 1.0f;
        env2.Process(&env2Sample);

        float lfo1Sample = 1.0f;
        lfo1.Process(&lfo1Sample);

        float lfo2Sample = 1.0f;
        lfo2.Process(&lfo2Sample);

        numMods = 1;
        mods[0] = lfo1Sample;
        *sample = 0.0f;
        for (int i = 0; i < numVcos; i++) {
            float vcoSample = 0.0f;
            vcos[i].Process(&vcoSample, mods, numMods);
            *sample = *sample + vcoSample * VOICE_ATTENTUATION_CONSTANT;
        }

        float noiseSample = 0.0f;
        noise.Process(&noiseSample, nullptr, 0);
        *sample = *sample + noiseSample * VOICE_ATTENTUATION_CONSTANT;

        numMods = 1;
        mods[0] = env1Sample;
        vca.Process(sample, mods, numMods);

        float sampleAndHoldSample = noise.GetLastSample();
        sampleAndHold.Process(&sampleAndHoldSample);

        numMods = 5;
        mods[0] = mtof(currentMidiNote);
        mods[1] = env1Sample;
        mods[2] = env2Sample;
        mods[3] = lfo2Sample;
        mods[4] = sampleAndHoldSample;
        vcf.Process(sample, mods, numMods);
    }

    bool Voice::IsAvailable()
    {
        return !env1.IsPlaying();
    }


    bool Voice::IsReleasing()
    {
        return env1.IsReleasing();
    }

    void Voice::NoteOn(int note, int velocity)
    {
        if (currentMidiNote != note) {
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
