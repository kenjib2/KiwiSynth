#include "Noise.h"

namespace kiwi_synth
{
    void Noise::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        isOn = true;
        noiseType = 0;
        level = 1.0F;
        lastSample = 0.0F;
        white.Init();
        white.SetSeed(std::rand());
        dust.Init();
        dust.SetDensity(0.015f);
    }

    void Noise::UpdateSettings()
    {
        isOn = patchSettings->getBoolValue(PatchSetting::VCO_NOISE_ON);
        noiseType = patchSettings->getIntValue(PatchSetting::VCO_NOISE_TYPE);
        level = patchSettings->getFloatValue(PatchSetting::VCO_NOISE_LEVEL);
    }

    void Noise::Process(float* sample, float* mods, uint8_t numMods)
    {
        switch (noiseType) {
            case 0:
            default:
                lastSample = white.Process();
                break;
            case 1:
                lastSample = dust.Process();
                break;
        }

        if (isOn) {
            *sample = lastSample * level;
        }
    }

    float Noise::GetLastSample()
    {
        return lastSample;
    }
}
