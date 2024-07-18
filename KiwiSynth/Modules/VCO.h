#ifndef __KIWI_SYNTH_VCO_H__
#define __KIWI_SYNTH_VCO_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      VCO_MIN_FREQUENCY = 30.0F;
    static const float      VCO_MAX_FREQUENCY = 15000.0F;
    
    class VCO
    {
        private:
            PatchSettings* patchSettings;
            bool       noteTriggered;
            float      masterTune;
            float      baseFreq;
            float      freq;
            float      lfo1Depth;
            int        midiNote;
            Oscillator osc;

            void CalculateBaseFreq();

        public:
            VCO() {}
            ~VCO() {}
            void Init(PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void Process(float* sample, float* mods, uint8_t numMods);
            void SetFreq(float frequency);
    };
}


#endif // __KIWI_SYNTH_VCO_H__
