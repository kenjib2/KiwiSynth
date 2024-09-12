#ifndef __KIWI_SYNTH_VCA_H__
#define __KIWI_SYNTH_VCA_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    const static float VCA_ATTENTUATION_CONSTANT = 0.2f;
    
    class VCA
    {
        private:
            float                   level;

        public:
            VCA() {}
            ~VCA() {}
            void Init(float sampleRate);

            void UpdateSettings(PatchSettings* patchSettings);
            void Process(float* sample, PatchSettings* patchSettings, float env1Mod, float mod);
    };
}


#endif // __KIWI_SYNTH_VCA_H__
