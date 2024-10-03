#ifndef __KIWI_SYNTH_LFO_H__
#define __KIWI_SYNTH_LFO_H__


#include "daisysp.h"

#include "../Patch/PatchSettings.h"
#include "KiwiSquare.h"
#include "KiwiSaw.h"
#include "KiwiRamp.h"
#include "KiwiTriangle.h"

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
            float           sawtoothGain;
            float           freq;
            bool            noteOnReset;
            float           basePhase;
            float           phase;
            KiwiSquare      squareOsc;
            KiwiSaw         sawOsc;
            KiwiRamp        rampOsc;
            KiwiTriangle    triangleOsc;
            Wavefolder      wavefolder;
            uint8_t         lfoNumber;

        public:
            LFO() {}
            ~LFO() {}
            void Init(float sampleRate, uint8_t envNumber);

            void UpdateSettings(PatchSettings* patchSettings);
            void Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float tphaseMod);
            inline void SetFreq(float frequency) { squareOsc.SetFreq(frequency); sawOsc.SetFreq(frequency); rampOsc.SetFreq(frequency); triangleOsc.SetFreq(frequency); }
            inline void NoteOn() { if (noteOnReset) { squareOsc.Reset(phase); sawOsc.Reset(phase); rampOsc.Reset(phase); triangleOsc.Reset(phase); } }
    };
}

#endif // __KIWI_SYNTH_LFO_H__
