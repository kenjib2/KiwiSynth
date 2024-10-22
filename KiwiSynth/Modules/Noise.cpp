#include "Noise.h"

namespace kiwi_synth
{
    void Noise::Init(float sampleRate)
    {
        isOn = true;
        noiseType = NOISE_TYPE_WHITE;
        level = 1.0f;
        lastSample = 0.0f;
        white.Init();
        white.SetSeed(std::rand());
        white.SetAmp(1.0f);
        dust.Init();
        dust.SetDensity(DEFAULT_DENSITY);
    }

    void Noise::UpdateSettings(PatchSettings* patchSettings)
    {
        isOn = patchSettings->getBoolValue(PatchSetting::OSC_NOISE_ON);
        noiseType = (NoiseType)patchSettings->getIntValue(PatchSetting::OSC_NOISE_TYPE);
        level = patchSettings->getFloatValue(PatchSetting::OSC_NOISE_LEVEL);
    }

}
