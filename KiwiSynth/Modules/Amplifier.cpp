#include "Amplifier.h"



using namespace kiwisynth;



void Amplifier::Process(float* sample, PatchSettings* patchSettings, float env1Mod, float mod)
{
    float levelSample = 0.0f;
    if (__builtin_expect(level_ > 0.0015f, 1)) {
        levelSample = *sample * (level_ + mod);
    }
    *sample = fclamp((*sample * env1Mod + levelSample) * AMP_ATTENTUATION_CONSTANT, -0.9999999f, 0.9999999f); // env1Mod is the only mod we multiply instead of add because it is also note triggering
}
