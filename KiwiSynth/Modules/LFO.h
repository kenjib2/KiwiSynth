#ifndef __KIWI_SYNTH_LFO_H__
#define __KIWI_SYNTH_LFO_H__


#include "daisysp.h"

#include "../Patch/PatchSettings.h"
#include "KiwiSquare.h"
#include "KiwiSaw.h"
#include "KiwiRamp.h"
#include "KiwiTriangle.h"
#include "KiwiFastSine.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      LFO_MIN_FREQUENCY = 0.1f;
    static const float      LFO_MAX_FREQUENCY = 20.0f;
    
    /*
     * Manages the Kiwi Synth LFOs.
     */
    class LFO
    {
        private:
            int             waveform;
            float           pulseWidth;
            float           variShape;
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
            VariableShapeOscillator variOsc;
            VariableSawOscillator variSaw;
            KiwiFastSine    sineOsc;
            Wavefolder      wavefolder;
            uint8_t         lfoNumber;

        public:
            LFO() {}
            ~LFO() {}
            void Init(float sampleRate, uint8_t envNumber);

            /*
             * Applies user settings to LFO parameters.
             */
            void UpdateSettings(PatchSettings* patchSettings);
            /*
             * Updates the sample by applying an LFO to amplitude. The envelope can be directly applied to the input signal,
             * or if 1.0f is passed in each time, the result will be a modulation source that represents the LFO.
             */
            void Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float tphaseMod);
            /*
             * Sets the LFO frequency in Hz.
             */
            inline void SetFreq(float frequency) { squareOsc.SetFreq(frequency); sawOsc.SetFreq(frequency); rampOsc.SetFreq(frequency); triangleOsc.SetFreq(frequency); }
            /*
             * If the synth is configured to retrigger when a note is played, this function will reset the phase.
             */
            inline void NoteOn() { if (noteOnReset) { squareOsc.Reset(phase); sawOsc.Reset(phase); rampOsc.Reset(phase); triangleOsc.Reset(phase); } }
    };
}

#endif // __KIWI_SYNTH_LFO_H__
