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
    }

    void VCA::Process(float* sample, float mod)
    {
        float levelSample = 0.0f;
        if (level > 0.000f) {
            levelSample = *sample * level;
        }
        *sample = *sample * mod;
        *sample = std::fmax(std::fmin((*sample + levelSample) * VCA_ATTENTUATION_CONSTANT, 0.99999f), 0.00001f);
    }

}
