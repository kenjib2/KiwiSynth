/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef __KIWI_SYNTH_KIWI_PHASER_H__
#define __KIWI_SYNTH_KIWI_PHASER_H__
#ifdef __cplusplus

#include <stdint.h>
#include "IEffect.h"
#include "KiwiDelayLine.h"

/** @file phaser.h */

namespace kiwi_synth
{
/**  
    @brief Single Phaser engine. Used in Phaser.
    @author Ben Sergentanis
*/
class KiwiPhaserEngine
{
  public:
    KiwiPhaserEngine() {}
    ~KiwiPhaserEngine() {}

    /** Initialize the module
        \param sample_rate Audio engine sample rate.
    */
    void Init(float sample_rate, int bufferNumber);

    /** Get the next sample
        \param in Sample to process
    */
    float Process(float in);

    /** How much to modulate the allpass filter by.
        \param depth Works 0-1.
    */
    void SetLfoDepth(float depth);

    /** Set lfo frequency.
        \param lfo_freq Frequency in Hz
    */
    void SetLfoFreq(float lfo_freq);

    /** Set the allpass frequency
        \param ap_freq Frequency in Hz.
    */
    void SetFreq(float ap_freq);

    /** Set the feedback amount.
        \param feedback Amount from 0-1.
    */
    void SetFeedback(float feedback);

  private:
    float                    sample_rate_;
    static constexpr int32_t kDelayLength
        = 2400; // 50 ms at 48kHz = .05 * 48000

    //triangle lfo
    float lfo_phase_;
    float lfo_freq_;
    float lfo_amp_;

    float os_;

    float feedback_;
    float ap_freq_;

    float deltime_;
    float last_sample_;

    KiwiDelayLine del_;

    float ProcessLfo();
};

//wraps up all of the phaser engines
/**  
    @brief Phaser Effect.
    @author Ben Sergentanis
    @date Jan 2021
*/
class KiwiPhaser : public IEffect
{
  public:
    KiwiPhaser() {}
    ~KiwiPhaser() {}

    /** Initialize the module
        \param sample_rate Audio engine sample rate
    */
    void Init(float sample_rate, int bufferNumber);

    /** Get the next floating point sample.
        \param in Sample to process
    */
    float Process(float in);

    /** Number of allpass stages.
        \param poles Works 1 to 8.
    */
    void SetPoles(int poles);

    /** Set all lfo depths
        \param depth Works 0-1.
    */
    void SetLfoDepth(float depth);

    /** Set all lfo frequencies.
        \param lfo_freq Lfo freq in Hz.
    */
    void SetLfoFreq(float lfo_freq);

    /** Set all channel allpass freq in Hz.
        \param ap_freq Frequency in Hz.
    */
    void SetFreq(float ap_freq);

    /** Set all channels feedback.
        \param feedback Works 0-1.
    */
    void SetFeedback(float feedback);

  private:
    static constexpr int kMaxPoles = 8;
    KiwiPhaserEngine     engines_[kMaxPoles];
    float                gain_frac_;
    float                inv_poles_;
    int                  poles_;
};
} //namespace kiwi_synth
#endif // __cplusplus
#endif // __KIWI_SYNTH_KIWI_PHASER_H__
