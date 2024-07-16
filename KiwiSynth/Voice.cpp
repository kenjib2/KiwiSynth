#include "Voice.h"

namespace kiwi_synth
{
    void Voice::Init(int numVCOs, PatchSettings* patchSettings, float sampleRate)
    {
        noteTriggered = false;
        this->numVCOs = numVCOs;
        this->patchSettings = patchSettings;
        for (int i = 0; i < numVCOs; i++) {
            VCO nextVco;
            nextVco.Init(patchSettings, sampleRate);
            vcos.push_back(nextVco);
        }
        vcf.Init(patchSettings, sampleRate);
        vca.Init(patchSettings, sampleRate);
        env1.Init(patchSettings, sampleRate);
        env2.Init(patchSettings, sampleRate);
    }

    void Voice::UpdateSettings()
    {
        env1.UpdateSettings();
        env2.UpdateSettings();

        for (int i = 0; i < numVCOs; i++) {
            vcos[i].UpdateSettings();
        }
        vca.UpdateSettings();
        vcf.UpdateSettings();
    }

    void Voice::Process(float* sample)
    {
        float env1Sample = 1.0f;
        env1.Process(&env1Sample);
        float mods[MAX_MODS];
        uint8_t numMods = 1;
        mods[0] = env1Sample;

        vcos[0].Process(sample);
        //env1.Process(sample);
        //vca.Process(sample);
        vca.Process(sample, mods, numMods);
        vcf.Process(sample);
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
        noteTriggered = true;
        currentMidiNote = note;
        vcos[0].SetFreq(mtof(note));
        env1.NoteOn();
        env2.NoteOn();
    }

    void Voice::NoteOff(int note, int velocity)
    {
        noteTriggered = false;
        env1.NoteOff();
        env2.NoteOff();
    }

}
