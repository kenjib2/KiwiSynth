#ifndef __KIWI_SYNTH_SAMPLE_AND_HOLD_H__
#define __KIWI_SYNTH_SAMPLE_AND_HOLD_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwisynth
{

    /*
     * The Kiwi Synth sample and hold module.
     */
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

            /*
             * Updates sample and hold parameters based on user input.
             */
            inline void UpdateSettings(PatchSettings* patchSettings) { rate = (int)patchSettings->getFloatValueLogLookup(PatchSetting::SH_RATE); }
            /*
             * Performs sample and hold on the incoming sample values.
             */
            inline void Process(float* sample, PatchSettings* patchSettings, float mod)
            {
                counter++;
                if (__builtin_expect(counter >= rate + SAMPLE_AND_HOLD_MAXIMUM_RATE * mod * 0.25f, 0)) {
                    counter = 0;
                    currentSample = *sample;
                }

                *sample = currentSample;
            }
    };

} // namespace kiwisynth
#endif // __KIWI_SYNTH_SAMPLE_AND_HOLD_H__
