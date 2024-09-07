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
    }

    void VCA::Process(float* sample, float env1Mod, float mod)
    {
        float levelSample = 0.0f;
        if (level > 0.0015f) {
            levelSample = *sample * (level + mod);
        }
        *sample = fclamp((*sample * env1Mod + levelSample) * VCA_ATTENTUATION_CONSTANT, -0.9999999F, 0.9999999F); // env1Mod is the only mod we multiple instead of add because it is also note triggering
    }

}
