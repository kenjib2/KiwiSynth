#include "SampleAndHold.h"

namespace kiwi_synth
{
    void SampleAndHold::Init(float sampleRate)
    {
        SAMPLE_AND_HOLD_MINIMUM_RATE = sampleRate / 1000; // 1 ms
        SAMPLE_AND_HOLD_MAXIMUM_RATE = sampleRate; // 1 sec

        rate = SAMPLE_AND_HOLD_MINIMUM_RATE * 100;
        counter = 0;
        currentSample = 0.0f;
    }

    void SampleAndHold::UpdateSettings(PatchSettings* patchSettings)
    {
        rate = (int)patchSettings->getFloatValueLogLookup(PatchSetting::SH_RATE);
    }

    void SampleAndHold::Process(float* sample, PatchSettings* patchSettings, float mod)
    {
        
        counter++;
        if (counter >= rate + SAMPLE_AND_HOLD_MAXIMUM_RATE * mod / 4.0f) {
            counter = 0;
            currentSample = *sample;
        }

        *sample = currentSample;
    }

}
