#include "VCA.h"

namespace kiwi_synth
{
    void VCA::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
    }

    void VCA::UpdateSettings()
    {
        level = patchSettings->getFloatValue(PatchSetting::VCA_LEVEL, -0.001F, 1.0F); // Start in negative to deal with low level pot noise
        env1Depth = patchSettings->getFloatValue(PatchSetting::VCA_ENV_1_DEPTH);
    }

    void VCA::Process(float* sample, float* mods, uint8_t numMods)
    {
        float levelSample = 0.0f;
        if (level > 0.000f) {
            levelSample = *sample * level;
        }
        for (uint8_t i = 0; i < numMods; i++)
        {
            if (i == 0) {
                *sample = *sample * env1Depth;
            }
            *sample = *sample * mods[i];
        }
        *sample = (*sample + levelSample) * VCA_ATTENTUATION_CONSTANT;
    }

}
