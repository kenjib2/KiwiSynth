#include "Noise.h"



using namespace kiwisynth;



void Noise::Init(float sampleRate)
{
    isOn_ = true;
    noiseType_ = NOISE_TYPE_WHITE;
    level_ = 1.0f;
    lastSample_ = 0.0f;
    white_.Init();
    white_.SetSeed(std::rand());
    white_.SetAmp(1.0f);
    dust_.Init();
    dust_.SetDensity(DEFAULT_DENSITY);
}



void Noise::UpdateSettings(PatchSettings* patchSettings)
{
    isOn_ = patchSettings->getBoolValue(PatchSetting::OSC_NOISE_ON);
    noiseType_ = (NoiseType)patchSettings->getIntValue(PatchSetting::OSC_NOISE_TYPE);
    level_ = patchSettings->getFloatValue(PatchSetting::OSC_NOISE_LEVEL);
}
