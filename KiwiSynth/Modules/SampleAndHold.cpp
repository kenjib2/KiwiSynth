#include "SampleAndHold.h"

namespace kiwi_synth
{
    void SampleAndHold::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        SAMPLE_AND_HOLD_MINIMUM_RATE = sampleRate / 1000; // 1 ms
        SAMPLE_AND_HOLD_MAXIMUM_RATE = sampleRate; // 1 sec

        rate = SAMPLE_AND_HOLD_MINIMUM_RATE * 100;
        counter = 0;
        currentSample = 0.0F;
    }

    void SampleAndHold::UpdateSettings()
    {
        rate = (int)patchSettings->getFloatValueLogLookup(PatchSetting::SH_RATE);
    }

    void SampleAndHold::Process(float* sample, float mod, bool fullFunctionality)
    {
        
        counter++;
        if (fullFunctionality) {
            if (counter >= rate + SAMPLE_AND_HOLD_MAXIMUM_RATE * mod / 4.0F) {
                counter = 0;
                currentSample = *sample;
            }
        } else {
            if (counter >= rate) {
                counter = 0;
                currentSample = *sample;
            }
        }

        *sample = currentSample;
    }

}
