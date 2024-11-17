/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef __KIWI_SYNTH_KIWI_CHORUS_H__
#define __KIWI_SYNTH_KIWI_CHORUS_H__
#ifdef __cplusplus

#include <stdint.h>
#include "IEffect.h"
#include "KiwiDelayLine.h"
#include "../Util/KUtils.h"

/** @file chorus.h */

namespace kiwisynth
{
/**  
    @brief Single Chorus engine. Used in Chorus.
    @author Ben Sergentanis
*/
class KiwiChorusEngine
{
  public:
    KiwiChorusEngine() {}
    ~KiwiChorusEngine() {}

    /** Initialize the module
        \param sample_rate Audio engine sample rate.
    */
    void Init(float sample_rate, int bufferNumber);

    /** Get the next sample
        \param in Sample to process
    */
    float Process(float in);

    /** How much to modulate the delay by.
        \param depth Works 0-1.
    */
    void SetLfoDepth(float depth);

    /** Set lfo frequency.
        \param freq Frequency in Hz
    */
    void SetLfoFreq(float freq);

    /** Set the internal delay rate. 
        \param delay Tuned for 0-1. Maps to .1 to 50 ms.
    */
    void SetDelay(float delay);

    /** Set the delay time in ms.
        \param ms Delay time in ms, 0 to 50 ms.
    */
    void SetDelayMs(float ms);

    /** Set the feedback amount.
        \param feedback Amount from 0-1.
    */
    void SetFeedback(float feedback);

  private:
    float                    sample_rate_;
    static constexpr int32_t kDelayLength
        = SAMPLE_RATE_INT / 20; 
        // 50 ms at 32kHz = .05 * 32000
        // 50 ms at 48kHz = .05 * 48000

    //triangle lfos
    float lfo_phase_;
    float lfo_freq_;
    float lfo_amp_;

    float feedback_;

    float delay_;

    KiwiDelayLine del_;

    float ProcessLfo();
};

//wraps up all of the chorus engines
/**  
    @brief Chorus Effect.
    @author Ben Sergentanis
    @date Jan 2021
    Based on https://www.izotope.com/en/learn/understanding-chorus-flangers-and-phasers-in-audio-production.html \n
    and https://www.researchgate.net/publication/236629475_Implementing_Professional_Audio_Effects_with_DSPs \n
*/
class KiwiChorus : public IEffect
{
  public:
    KiwiChorus() {}
    ~KiwiChorus() {}

    /** Initialize the module
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate, int bufferNumber);

    /** Get the net floating point sample. Defaults to left channel.
        \param in Sample to process
    */
    float Process(float in);

    /** Set both lfo depths.
        \param depth Both channels lfo depth. Works 0-1.
    */
    void SetLfoDepth(float depth);

    /** Set both lfo frequencies.
        \param depth Both channel lfo freqs in Hz.
    */
    void SetLfoFreq(float freq);

    /** Set both channel delay amounts.
        \param delay Both channel delay amount. Works 0-1.
    */
    void SetDelay(float delay);

    /** Set both channel delay in ms.
        \param ms Both channel delay amounts in ms.
    */
    void SetDelayMs(float ms);

    /** Set both channels feedback.
        \param feedback Both channel feedback. Works 0-1.
    */
    void SetFeedback(float feedback);

  private:
    KiwiChorusEngine engines_[2];
    float        gain_frac_;

    float sig_;
};
} //namespace kiwisynth
#endif // __cplusplus
#endif // __KIWI_SYNTH_KIWI_CHORUS_H__
