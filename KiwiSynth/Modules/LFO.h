#ifndef __KIWI_SYNTH_LFO_H__
#define __KIWI_SYNTH_LFO_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      LFO_MIN_FREQUENCY = 0.1F;
    static const float      LFO_MAX_FREQUENCY = 20.0F;
    
    class LFO
    {
        private:
            float           pulseWidth;
            float           freq;
            bool            noteOnReset;
            float           phase;
            PatchSettings*  patchSettings;
            Oscillator      osc;
            uint8_t         lfoNumber;

        public:
            LFO() {}
            ~LFO() {}
            void Init(PatchSettings* patchSettings, float sampleRate, uint8_t envNumber);

            void UpdateSettings();
            void Process(float* sample);
            void SetFreq(float frequency);
            void NoteOn();
    };
}


#endif // __KIWI_SYNTH_LFO_H__
