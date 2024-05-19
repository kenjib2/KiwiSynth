#ifndef __KIWI_SYNTH_VCF_H__
#define __KIWI_SYNTH_VCF_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"
#include "MoogLadder.h"

using namespace daisysp;

namespace kiwi_synth
{
    
    class VCF
    {
        private:
            bool                    noteTriggered;
            float                   frequency;
            float                   resonance;
            float                   envDepth;
            infrasonic::MoogLadder  filter;
            PatchSettings*          patchSettings;

        public:
            VCF(PatchSettings* patchSettings, float sampleRate);
            ~VCF() {}
            void Process(const float* in, float* out, size_t size);
    };
}


#endif // __KIWI_SYNTH_VCF_H__
