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
        float                   minimumRate_;
        float                   maximumRate_;
        int                     rate_;
        int                     counter_;
        float                   currentSample_;

    public:
        SampleAndHold() {}
        ~SampleAndHold() {}
        void Init(float sampleRate);



        /*
            * Updates sample and hold parameters based on user input.
            */
        inline void UpdateSettings(PatchSettings* patchSettings) { rate_ = (int)patchSettings->getFloatValueLogLookup(PatchSetting::SH_RATE); }



        /*
            * Performs sample and hold on the incoming sample values.
            */
        inline void Process(float* sample, PatchSettings* patchSettings, float mod)
        {
            counter_++;
            if (__builtin_expect(counter_ >= rate_ + maximumRate_ * mod * 0.25f, 0)) {
                counter_ = 0;
                currentSample_ = *sample;
            }

            *sample = currentSample_;
        }
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_SAMPLE_AND_HOLD_H__
