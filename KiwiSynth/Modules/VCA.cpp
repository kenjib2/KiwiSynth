#include "VCA.h"

namespace kiwi_synth
{

    void VCA::Process(float* sample, PatchSettings* patchSettings, float env1Mod, float mod)
    {
        float levelSample = 0.0f;
        if (__builtin_expect(level > 0.0015f, 1)) {
            levelSample = *sample * (level + mod);
        }
        *sample = fclamp((*sample * env1Mod + levelSample) * VCA_ATTENTUATION_CONSTANT, -0.9999999f, 0.9999999f); // env1Mod is the only mod we multiple instead of add because it is also note triggering
    }
}
