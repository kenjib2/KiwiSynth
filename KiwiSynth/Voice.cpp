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
        env1.Init(patchSettings, sampleRate, 0);
        env2.Init(patchSettings, sampleRate, 1);
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
        float mods[MAX_MODS];
        uint8_t numMods = 0;

        float env1Sample = 1.0f;
        env1.Process(&env1Sample);

        float env2Sample = 1.0f;
        env2.Process(&env2Sample);

        vcos[0].Process(sample, nullptr, 0);

        numMods = 1;
        mods[0] = env1Sample;
        vca.Process(sample, mods, numMods);

        numMods = 3;
        mods[0] = mtof(currentMidiNote);
        mods[1] = env1Sample;
        mods[2] = env2Sample;
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
        vcos[0].SetFreq(mtof(note));
        env1.NoteOn(retrigger);
        env2.NoteOn(retrigger);
    }

    void Voice::NoteOff(int note, int velocity)
    {
        noteTriggered = false;
        env1.NoteOff();
        env2.NoteOff();
    }

}
