#include "VCA.h"

namespace kiwi_synth
{
    void VCA::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
    }

    void VCA::UpdateSettings()
    {
        level = patchSettings->getFloatValue(PatchSetting::VCA_LEVEL);
        env1Depth = patchSettings->getFloatValue(PatchSetting::VCA_ENV_1_DEPTH);
    }

    void VCA::Process(float* sample, float* mods, uint8_t numMods)
    {
        *sample = *sample * 0.12f * level;
        for (uint8_t i = 0; i < numMods; i++)
        {
            *sample = *sample * mods[i];
        }
    }

}
