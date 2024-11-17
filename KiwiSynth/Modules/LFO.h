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

namespace kiwisynth
{



static const float      LFO_MIN_FREQUENCY = 0.1f;
static const float      LFO_MAX_FREQUENCY = 20.0f;



/*
 * Manages the Kiwi Synth LFOs.
 */
class LFO
{
    private:
        int                     waveform_;
        float                   pulseWidth_;
        float                   variShape_;
        float                   waveFolderGain_;
        float                   sawtoothGain_;
        float                   freq_;
        bool                    isNoteOnReset_;
        float                   basePhase_;
        float                   phase_;
        KiwiSquare              squareOsc_;
        KiwiSaw                 sawOsc_;
        KiwiRamp                rampOsc_;
        KiwiTriangle            triangleOsc_;
        VariableShapeOscillator variOsc_;
        VariableSawOscillator   variSaw_;
        KiwiFastSine            sineOsc_;
        Wavefolder              wavefolder_;
        uint8_t                 lfoNumber_;

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
        inline void SetFreq(float frequency) { squareOsc_.SetFreq(frequency); sawOsc_.SetFreq(frequency); rampOsc_.SetFreq(frequency); triangleOsc_.SetFreq(frequency); }



        /*
            * If the synth is configured to retrigger when a note is played, this function will reset the phase.
            */
        inline void NoteOn() { if (isNoteOnReset_) { squareOsc_.Reset(phase_); sawOsc_.Reset(phase_); rampOsc_.Reset(phase_); triangleOsc_.Reset(phase_); } }
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_LFO_H__
