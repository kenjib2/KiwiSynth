/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

// Modified by Kenji Baugham for Triangle non-polyblep only and speed optimization

#pragma once
#ifndef __KIWI_SYNTH_KIWI_TRIANGLE_H__
#define __KIWI_SYNTH_KIWI_TRIANGLE_H__
#include <stdint.h>
#include "Utility/dsp.h"
#ifdef __cplusplus

using namespace daisysp;

namespace kiwisynth
{
/** Synthesis of several waveforms, including polyBLEP bandlimited waveforms.
*/
class KiwiTriangle
{
  public:
    KiwiTriangle() {}
    ~KiwiTriangle() {}

    /** Initializes the Oscillator

        \param sample_rate - sample rate of the audio engine being run, and the frequency that the Process function will be called.

        Defaults:
        - freq_ = 100 Hz
        - amp_ = 0.5
        - waveform_ = sine wave.
    */
    void Init(float sample_rate)
    {
        sr_        = sample_rate;
        sr_recip_  = 1.0f / sample_rate;
        freq_      = 100.0f;
        amp_       = 0.5f;
        phase_     = 0.0f;
        phase_inc_ = CalcPhaseInc(freq_);
        eoc_       = true;
    }


    /** Changes the frequency of the Oscillator, and recalculates phase increment.
    */
    inline void SetFreq(const float f)
    {
        freq_      = f;
        phase_inc_ = CalcPhaseInc(f);
        phase_inc_recip_ = 1.0f / phase_inc_;
    }


    /** Sets the amplitude of the waveform.
    */
    inline void SetAmp(const float a) { amp_ = a; }

    /** Processes the waveform to be generated, returning one sample. This should be called once per sample period.
    */
    float Process();
    /** Processes the waveform to be generated, returning one sample. This should be called once per sample period.
     * The phaseMod parameter can be used to modulate the phase for PM synthesis.
     */
    float Process(float phaseMod);
    /** Resets the phase to the input argument. If no argumeNt is present, it will reset phase to 0.0;
    */
    inline void Reset(float _phase = 0.0f) { phase_ = _phase; }
    /** Returns true if cycle is at end of cycle. Set during call to Process.
    */
    inline bool IsEOC() { return eoc_; }
    /** Returns the ratio of phase to frequency to generate a unit of time passed since the last phase crossing.
     */
    inline float GetPhaseRatio() { return phase_ / freq_; }
    /** Sets the phase based on a given phase ratio.
     */
    inline void HardSync(float phaseRatio) { phase_ = phaseRatio * freq_; }
    
  private:
    inline float   CalcPhaseInc(float f) { return f * sr_recip_; }
    float   amp_, freq_;
    float   sr_, sr_recip_, phase_diff_, phase_, phase_inc_, phase_inc_recip_;
    bool    eoc_;
};

} // namespace kiwisynth
#endif
#endif // __KIWI_SYNTH_KIWI_TRIANGLE_H__
