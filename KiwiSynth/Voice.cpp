#include "Voice.h"

namespace kiwi_synth
{
    void Voice::Init(int numVCOs, PatchSettings* patchSettings, float sampleRate)
    {
        noteTriggered = false;
        this->numVcos = numVCOs;
        this->patchSettings = patchSettings;
        for (int i = 0; i < numVCOs; i++) {
            VCO nextVco;
            nextVco.Init(patchSettings, sampleRate, i);
            vcos.push_back(nextVco);
        }
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
        vca.UpdateSettings();
        vcf.UpdateSettings();

        env1.UpdateSettings();
        env2.UpdateSettings();
        lfo1.UpdateSettings();
        lfo2.UpdateSettings();
    }

    void Voice::Process(float* sample)
    {
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
        /*float test = 0.0f;
        vcos[0].Process(&test, mods, numMods);
        *sample += test * 0.3f;
        vcos[1].Process(&test, mods, numMods);
        *sample += test * 0.3f;
        vcos[2].Process(&test, mods, numMods);
        *sample += test * 0.3f;*/
        for (int i = 0; i < numVcos; i++) {
            float vcoSample = 0.0f;
            vcos[i].Process(&vcoSample, mods, numMods);
            vcoSample = vcoSample * 0.3f;
            *sample = *sample + vcoSample;
        }

        numMods = 1;
        mods[0] = env1Sample;
        vca.Process(sample, mods, numMods);

        numMods = 4;
        mods[0] = mtof(currentMidiNote);
        mods[1] = env1Sample;
        mods[2] = env2Sample;
        mods[3] = lfo2Sample;
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
        bool retrigger = true;
        if (currentMidiNote == note) {
            retrigger = false;
        }
        noteTriggered = true;
        currentMidiNote = note;
        for (int i = 0; i < numVcos; i++) {
            vcos[i].SetFreq(mtof(note));
        }
        env1.NoteOn(retrigger);
        env2.NoteOn(retrigger);
        lfo1.NoteOn();
        lfo2.NoteOn();
    }

    void Voice::NoteOff(int note, int velocity)
    {
        noteTriggered = false;
        env1.NoteOff();
        env2.NoteOff();
    }

}
