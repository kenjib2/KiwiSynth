#include "Noise.h"

namespace kiwi_synth
{
    void Noise::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
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

    void Noise::UpdateSettings()
    {
        isOn = patchSettings->getBoolValue(PatchSetting::VCO_NOISE_ON);
        noiseType = patchSettings->getIntValue(PatchSetting::VCO_NOISE_TYPE);
        level = patchSettings->getFloatValue(PatchSetting::VCO_NOISE_LEVEL);
    }

    void Noise::Process(float* sample, float mod)
    {
        if (noiseType == 0) {
            lastSample = white.Process();
        } else {
            lastSample = dust.Process();
        }

        if (isOn) {
            *sample = lastSample * fclamp((level + mod), -1.0f, 1.0f);
        }
    }

    float Noise::GetLastSample()
    {
        return lastSample;
    }
}
