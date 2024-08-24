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

    void VCA::Process(float* sample, float env1Mod, float mod)
    {
        float levelSample = 0.0f;
        if (level > 0.0005f) {
            levelSample = *sample * std::fmax((level + mod), 0.00001f);
        }
        if (mod != 0.0f) {
            *sample = (*sample + *sample * mod * 2) * env1Mod + levelSample; // env1Mod is the only mod we multiple instead of add because it is also note triggering
        } else {
            // avoiding extra calculations for fullFunctionality = false
            *sample = *sample * env1Mod + levelSample;
        }
        *sample = std::fmax(std::fmin(*sample * VCA_ATTENTUATION_CONSTANT, 0.99999f), 0.00001f);
    }

}
