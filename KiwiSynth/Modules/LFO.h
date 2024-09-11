#ifndef __KIWI_SYNTH_LFO_H__
#define __KIWI_SYNTH_LFO_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      LFO_MIN_FREQUENCY = 0.1f;
    static const float      LFO_MAX_FREQUENCY = 20.0f;
    
    class LFO
    {
        private:
            int             waveform;
            float           pulseWidth;
            float           waveFolderGain;
            float           freq;
            bool            noteOnReset;
            float           basePhase;
            float           phase;
            PatchSettings*  patchSettings;
            Oscillator      osc;
            Wavefolder      wavefolder;
            uint8_t         lfoNumber;

        public:
            LFO() {}
            ~LFO() {}
            void Init(PatchSettings* patchSettings, float sampleRate, uint8_t envNumber);

            void UpdateSettings();
            void Process(float* sample, float mod, float pwMod, float tphaseMod, bool fold = true);
            void SetFreq(float frequency);
            void NoteOn();
    };
}


#endif // __KIWI_SYNTH_LFO_H__
