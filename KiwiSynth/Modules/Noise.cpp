#include "Noise.h"

namespace kiwi_synth
{
    void Noise::Init(float sampleRate)
    {
        isOn = true;
        noiseType = 0;
        level = 1.0f;
        lastSample = 0.0f;
        white.Init();
        white.SetSeed(std::rand());
        white.SetAmp(1.0f);
        dust.Init();
        dust.SetDensity(0.015f);
    }

    void Noise::UpdateSettings(PatchSettings* patchSettings)
    {
        isOn = patchSettings->getBoolValue(PatchSetting::VCO_NOISE_ON);
        noiseType = patchSettings->getIntValue(PatchSetting::VCO_NOISE_TYPE);
        level = patchSettings->getFloatValue(PatchSetting::VCO_NOISE_LEVEL);
    }

}
