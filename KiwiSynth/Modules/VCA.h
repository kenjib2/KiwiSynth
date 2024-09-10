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
            PatchSettings*          patchSettings;

        public:
            VCA() {}
            ~VCA() {}
            void Init(PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void Process(float* sample, float env1Mod, float mod);
    };
}


#endif // __KIWI_SYNTH_VCA_H__
