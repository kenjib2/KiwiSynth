#ifndef __KIWI_SYNTH_VCA_H__
#define __KIWI_SYNTH_VCA_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    const static float VCA_ATTENTUATION_CONSTANT = 0.30f;
    
    /*
     * The VCA module for the Kiwi Synth. This module serves two primary purpose: to apply modulation to the signal level,
     * and to apply a general attenuation so that when multiple voices are combined they do not clip.
     */
    class VCA
    {
        private:
            float                   level;

        public:
            VCA() {}
            ~VCA() {}
            void Init() {};

            /*
             * Applies user settings to VCA parameters.
             */
            inline void UpdateSettings(PatchSettings* patchSettings) { level = patchSettings->getFloatValue(PatchSetting::VCA_LEVEL); }
            /*
             * Adjust the output level in the incoming sample.
             */
            void Process(float* sample, PatchSettings* patchSettings, float env1Mod, float mod);
    };
}


#endif // __KIWI_SYNTH_VCA_H__
