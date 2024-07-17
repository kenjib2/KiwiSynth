#ifndef __KIWI_SYNTH_VCA_H__
#define __KIWI_SYNTH_VCA_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    
    class VCA
    {
        private:
            float                   level;
            float                   env1Depth;
            bool                    noteTriggered;
            PatchSettings*          patchSettings;

        public:
            VCA() {}
            ~VCA() {}
            void Init(PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void Process(float* sample, float* mods, uint8_t numMods);
    };
}


#endif // __KIWI_SYNTH_VCA_H__
