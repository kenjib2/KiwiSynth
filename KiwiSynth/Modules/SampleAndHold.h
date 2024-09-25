#ifndef __KIWI_SYNTH_SAMPLE_AND_HOLD_H__
#define __KIWI_SYNTH_SAMPLE_AND_HOLD_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    class SampleAndHold
    {
        private:
            float                   SAMPLE_AND_HOLD_MINIMUM_RATE;
            float                   SAMPLE_AND_HOLD_MAXIMUM_RATE;
            int                     rate;
            int                     counter;
            float                   currentSample;

        public:
            SampleAndHold() {}
            ~SampleAndHold() {}
            void Init(float sampleRate);

            inline void UpdateSettings(PatchSettings* patchSettings) { rate = (int)patchSettings->getFloatValueLogLookup(PatchSetting::SH_RATE); }
            inline void Process(float* sample, PatchSettings* patchSettings, float mod)
            {
                counter++;
                if (counter >= rate + SAMPLE_AND_HOLD_MAXIMUM_RATE * mod * 0.25f) {
                    counter = 0;
                    currentSample = *sample;
                }

                *sample = currentSample;
            }
    };
}


#endif // __KIWI_SYNTH_SAMPLE_AND_HOLD_H__
