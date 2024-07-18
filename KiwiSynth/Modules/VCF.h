#ifndef __KIWI_SYNTH_VCF_H__
#define __KIWI_SYNTH_VCF_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"
#include "MoogLadder.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      VCF_MIN_FREQUENCY = 40.0F;
    static const float      VCF_MAX_FREQUENCY = 22000.0F;
    
    class VCF
    {
        private:
            bool                    noteTriggered;
            float                   frequency;
            float                   resonance;
            float                   env1Depth;
            float                   env2Depth;
            float                   lfo2Depth;
            float                   keyboardTracking;
            infrasonic::MoogLadder  filter;
            PatchSettings*          patchSettings;

        public:
            VCF() {}
            ~VCF() {}
            void Init(PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void Process(float* sample, float* mods, uint8_t numMods);
    };
}


#endif // __KIWI_SYNTH_VCF_H__
