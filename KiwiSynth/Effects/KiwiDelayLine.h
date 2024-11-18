/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/



#pragma once
#ifndef __KIWI_SYNTH_KIWI_DELAY_LINE_H__
#define __KIWI_SYNTH_KIWI_DELAY_LINE_H__



#include <stdlib.h>
#include <stdint.h>
#include "daisy_seed.h"



namespace kiwisynth
{



static const int DELAY_LINE_BUFFER_SIZE = 4096; // This must always be an exponent of 2



/** Simple Delay line with a fixed 4k, or 4096 sample, max size.
November 2019

By: shensley
*/
class KiwiDelayLine
{
  public:
    KiwiDelayLine() {}
    ~KiwiDelayLine() {}
    /** initializes the delay line by clearing the values within, and setting delay to 1 sample.
     * The buffer size will be fixed at 4k in length, or 4096 samples.
    */
    void Init(int bufferNumber);



    /** clears buffer, sets write ptr to 0, and delay to 1 sample.
    */
    void Reset()
    {
        for(size_t i = 0; i < DELAY_LINE_BUFFER_SIZE; i++)
        {
            line_[i] = 0.0f;
        }
        write_ptr_ = 0;
        delay_     = 1;
    }



    /** sets the delay time in samples
        If a float is passed in, a fractional component will be calculated for interpolating the delay line.
    */
    inline void SetDelay(size_t delay)
    {
        frac_  = 0.0f;
        delay_ = delay < DELAY_LINE_BUFFER_SIZE ? delay : DELAY_LINE_BUFFER_SIZE - 1;
    }



    /** sets the delay time in samples
        If a float is passed in, a fractional component will be calculated for interpolating the delay line.
    */
    inline void SetDelay(float delay)
    {
        int32_t int_delay = static_cast<int32_t>(delay);
        frac_             = delay - static_cast<float>(int_delay);
        delay_ = static_cast<size_t>(int_delay) < DELAY_LINE_BUFFER_SIZE ? int_delay
                                                           : DELAY_LINE_BUFFER_SIZE - 1;
    }



    /** writes the sample of type T to the delay line, and advances the write ptr
    */
    inline void Write(const float sample)
    {
        line_[write_ptr_] = sample;
        write_ptr_ = (write_ptr_ + 1) & size_mask_;
    }



    /** returns the next sample of type T in the delay line, interpolated if necessary.
    */
    inline const float Read() const
    {
        float a = line_[(write_ptr_ - delay_) & size_mask_];
        float b = line_[(write_ptr_ - (delay_ + 1)) & size_mask_];
        return a + (b - a) * frac_;
    }



    /** Read from a set location */
    inline const float Read(float delay) const
    {
        int32_t delay_integral   = static_cast<int32_t>(delay);
        float   delay_fractional = delay + static_cast<float>(delay_integral);
        const float a = line_[(write_ptr_ - delay_integral) & size_mask_];
        const float b = line_[(write_ptr_ - delay_integral - 1) & size_mask_];
        return a + (b - a) * delay_fractional;
    }



    inline const float Allpass(const float sample, size_t delay, const float coefficient)
    {
        float read  = line_[(write_ptr_ - delay) & size_mask_];
        float write = sample + coefficient * read;
        Write(write);
        return -write * coefficient + read;
    }

  private:
    size_t size_mask_;
    float  frac_;
    size_t write_ptr_;
    size_t delay_;
    float* line_;
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_KIWI_DELAY_LINE_H__
