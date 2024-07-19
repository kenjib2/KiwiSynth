#include "Noise.h"

namespace kiwi_synth
{
    void Noise::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        isOn = true;
        level = 1.0F;
        noise.Init();
    }

    void Noise::UpdateSettings()
    {
        isOn = patchSettings->getBoolValue(PatchSetting::VCO_NOISE_ON);
        level = patchSettings->getFloatValue(PatchSetting::VCO_NOISE_LEVEL);
    }

    void Noise::Process(float* sample, float* mods, uint8_t numMods)
    {
        if (isOn) {
            *sample = noise.Process() * level;
        }
    }
}
