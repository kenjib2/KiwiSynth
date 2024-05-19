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
            VCO(PatchSettings* patchSettings, float sampleRate);
            ~VCO() {}
            void Process(float *, size_t size);
    };
}


#endif // __KIWI_SYNTH_VCO_H__
