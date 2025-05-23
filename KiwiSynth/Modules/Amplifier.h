#ifndef __KIWI_SYNTH_AMPLIFIER_H__
#define __KIWI_SYNTH_AMPLIFIER_H__



#include "daisysp.h"
#include "../Patch/PatchSettings.h"



using namespace daisysp;

namespace kiwisynth
{



const static float AMP_ATTENTUATION_CONSTANT = 0.30f;



/*
 * The amplifer module for the Kiwi Synth. This module serves two primary purpose: to apply modulation to the signal level,
 * and to apply a general attenuation so that when multiple voices are combined they do not clip.
 */
class Amplifier
{
    private:
        float                   level_;

    public:
        Amplifier() {}
        ~Amplifier() {}
        void Init() {};

        /*
         * Adjust the output level in the incoming sample.
         */
        void Process(float* sample, PatchSettings* patchSettings, float env1Mod, float mod);



        /*
         * Applies user settings to amplifer parameters.
         */
        inline void UpdateSettings(PatchSettings* patchSettings) { level_ = patchSettings->getFloatValue(PatchSetting::AMP_LEVEL); }
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_AMPLIFIER_H__
