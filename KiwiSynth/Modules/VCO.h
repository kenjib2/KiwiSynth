#ifndef __KIWI_SYNTH_VCO_H__
#define __KIWI_SYNTH_VCO_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    
    class VCO
    {
        private:
            PatchSettings* patchSettings;
            bool       noteTriggered;
            float      baseFreq;
            int        midiNote;
            Oscillator osc;

            void CalculateBaseFreq();

        public:
            VCO() {}
            ~VCO() {}
            void Init(PatchSettings* patchSettings, float sampleRate);
            void Process(float * out, size_t size);
    };
}


#endif // __KIWI_SYNTH_VCO_H__
