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

            void UpdateSettings(PatchSettings* patchSettings);
            void Process(float* sample, PatchSettings* patchSettings, float mod);
    };
}


#endif // __KIWI_SYNTH_SAMPLE_AND_HOLD_H__
