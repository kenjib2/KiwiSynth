#ifndef __KIWI_SYNTH_OSCILLATOR_H__
#define __KIWI_SYNTH_OSCILLATOR_H__



#include "daisysp.h"

#include "../Patch/PatchSettings.h"
#include "KiwiPBSquare.h"
#include "KiwiPBSaw.h"
#include "KiwiTriangle.h"
#include "KiwiFastSine.h"



using namespace daisysp;

namespace kiwisynth
{



/*
 * The Kiwi Synth oscillator module provides a wide range of tonal texture. The following waveforms are supported:
 *
 * Square: A polyblep square with variable pulse width.
 * Saw: A polyblep sawtooth. A variable waveform squashing algorithm lops off the top and bottom of the
 *     waveform as it is increased, giving the waveform an increasingly square-wave type quality.
 * Triangle: A polyblep triangle with variable wavefolding.
 * Variable Shape: A waveform that transitions from saw-like to triangle-like.
 * Variable Saw: A sawtooth that can add a variable amount of notch to the waveform.
 * Sine: A polynomial sine wave approximation with variable wavefolding.
 */
class Oscillator
{
    private:
        uint8_t    oscNumber_;
        float      basePhase_;
        float      pulseWidth_;
        float      variShape_;
        float      waveFolderGain_;
        float      sawtoothGain_;
        float      level_;
        float      fineTune_; // mod to midi note -1 to 1
        float      interval_; // mod to midi note -11 to 11
        float      octave_; // mod to midi note -24, -12, 0, 12, 24
        float      playingNote_; // midiNote with fineTune, interval, octave, and masterTune applied.
        bool       isOn_;
        bool       isNoteTriggered_;
        int        waveform_;
        KiwiPBSquare squareOsc_;
        KiwiPBSaw    sawOsc_;
        KiwiTriangle    triangleOsc_;
        VariableShapeOscillator variOsc_;
        VariableSawOscillator variSaw_;
        KiwiFastSine sineOsc_;
        Wavefolder wavefolder_;

    public:
        float      midiNote_; // 0 to 127 with portamento applied. Needs mtof to make a frequency.
        float      paraOffset_; // Midi note for this specific oscillator relative to 0
        bool       isEoc_; // End of cycle (ready for hard sync)

        Oscillator() {}
        ~Oscillator() {}
        void Init(float sampleRate, uint8_t oscNumber);

        /*
         * Applies user settings to oscillator parameters.
         */
        void UpdateSettings(PatchSettings* patchSettings);
        /*
         * Generates the oscillator output. The value of the sample received as input will be ignored
         * and overwritten. There are two sources of modulation. Mod will vary the oscillator
         * frequency. The function of pwMod will vary depending on the waveform selected, and is only
         * actually pulse width modulation for the square wave.
         */
        void Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float ampMod);
        /*
         * Process override to include phase modulation.
         */
        void Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float ampMod, float phaseMod);
        /* Returns the ratio of phase to frequency to generate a unit of time passed since the last phase crossing.
         */
        float GetPhaseRatio();
        /* Sets the phase based on a given phase ratio.
         */
        void HardSync(float phaseRatio);
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_OSCILLATOR_H__
