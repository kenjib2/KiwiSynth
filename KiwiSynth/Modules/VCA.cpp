#include "VCA.h"

namespace kiwi_synth
{
    void VCA::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
    }

    void VCA::UpdateSettings()
    {
    }

    void VCA::Process(float* sample, float* mods, uint8_t numMods)
    {
        *sample = *sample * 0.08f;
        for (uint8_t i = 0; i < numMods; i++)
        {
            *sample = *sample * mods[i];
        }
    }

}
