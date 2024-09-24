#ifndef __KIWI_SYNTH_VCA_H__
#define __KIWI_SYNTH_VCA_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    const static float VCA_ATTENTUATION_CONSTANT = 0.30f;
    
    class VCA
    {
        private:
            float                   level;

        public:
            VCA() {}
            ~VCA() {}
            void Init() {};

            inline void UpdateSettings(PatchSettings* patchSettings) { level = patchSettings->getFloatValue(PatchSetting::VCA_LEVEL); }
            void Process(float* sample, PatchSettings* patchSettings, float env1Mod, float mod);
    };
}


#endif // __KIWI_SYNTH_VCA_H__
