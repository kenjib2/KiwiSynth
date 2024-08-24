#include "SampleAndHold.h"

namespace kiwi_synth
{
    void SampleAndHold::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        SAMPLE_AND_HOLD_MINIMUM_RATE = sampleRate / 1000; // 1 ms
        SAMPLE_AND_HOLD_MAXIMUM_RATE = sampleRate; // 1 sec

        rate = SAMPLE_AND_HOLD_MINIMUM_RATE * 100;
        shToVcfCutoff = 0.0F;
        counter = 0;
        currentSample = 0.0F;
    }

    void SampleAndHold::UpdateSettings()
    {
        rate = (int)patchSettings->getFloatValue(PatchSetting::SH_RATE, SAMPLE_AND_HOLD_MINIMUM_RATE, SAMPLE_AND_HOLD_MAXIMUM_RATE, LOGARHITHMIC);
        shToVcfCutoff = patchSettings->getFloatValue(PatchSetting::SH_TO_VCF_CUTOFF, -0.001F, 1.001F, EXPONENTIAL);
    }

    void SampleAndHold::Process(float* sample)
    {
        counter++;
        if (counter >= rate) {
            counter = 0;
            currentSample = *sample;
        }

        *sample = currentSample;
    }

}
