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
            bool                    noteTriggered;
            PatchSettings*          patchSettings;

        public:
            VCA() {}
            ~VCA() {}
            void Init(PatchSettings* patchSettings, float sampleRate);
            void Process(const float* in, float* out, size_t size);
            bool IsPlaying();
            bool IsReleasing();
    };
}


#endif // __KIWI_SYNTH_VCA_H__
