#include "SampleAndHold.h"

namespace kiwi_synth
{
    void SampleAndHold::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        SAMPLE_AND_HOLD_MINIMUM_RATE = sampleRate / 1000; // 1 ms
        SAMPLE_AND_HOLD_MAXIMUM_RATE = sampleRate; // 1 sec

        rate = SAMPLE_AND_HOLD_MINIMUM_RATE * 100;
        level = 0.0F;
        counter = 0;
        currentSample = 0.0F;
    }

    void SampleAndHold::UpdateSettings()
    {
        rate = (int)patchSettings->getFloatValue(PatchSetting::SH_RATE, SAMPLE_AND_HOLD_MINIMUM_RATE, SAMPLE_AND_HOLD_MAXIMUM_RATE, LOGARHITHMIC);
        level = patchSettings->getFloatValue(PatchSetting::SH_LEVEL, -0.001F, 1.001F, EXPONENTIAL);
    }

    void SampleAndHold::Process(float* sample)
    {
        counter++;
        if (counter >= rate) {
            counter = 0;
            currentSample = *sample;
        }

        float computedLevel = level * level;
        if (computedLevel > 1.0F) {
            computedLevel = 0.9999999999F;
        }

        if (computedLevel > 0.0F) {
            *sample = computedLevel * currentSample;
        } else {
            *sample = 0.0F;
        }
    }

}
