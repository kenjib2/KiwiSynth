/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#ifndef __KIWI_SYNTH_KIWI_DUST_H__
#define __KIWI_SYNTH_KIWI_DUST_H__

#include "Utility/dsp.h"

/** @file KiwiDust.h */

using namespace daisysp;

namespace kiwi_synth
{
/**  
       @brief Dust Module
       @author Ported by Ben Sergentanis 
       @date Jan 2021 
       Randomly Clocked Samples \n \n
       Ported from pichenettes/eurorack/plaits/dsp/noise/dust.h \n
       to an independent module. \n
       Original code written by Emilie Gillet in 2016. \n

*/

const static uint32_t MAX_XORSHIFT_VALUE = 4294967295;

class xorshiftPRNG {
  private:
  struct xorshift_state {
     uint64_t a32;
  };
  xorshift_state state;

  public:
  xorshiftPRNG(int seed)
  {
     state.a32 = seed;
  }


  uint32_t xorshift32()
  {
     uint32_t x = state.a32;
     x ^= x <<17;
     x ^= x >>7;
     x ^= x <<5;
     state.a32 = x;
     return (x);
  }


};

// Modified from the DaisySP version to use xorshift32 rng
class KiwiDust
{
  public:
    KiwiDust() {}
    ~KiwiDust() {}

    void Init() {
        SetDensity(.5f);
        rng = new xorshiftPRNG(rand());
    }

    float Process()
    {
        float inv_density = 1.0f / density_;
        float u           = rng->xorshift32() * kRandFrac;
        if(u < density_)
        {
            return u * inv_density;
        }
        return 0.0f;
    }

    void SetDensity(float density)
    {
        density_ = fclamp(density, 0.f, 1.f);
        density_ = density_ * .3f;
    }

  private:
    xorshiftPRNG*          rng;
    float                  density_;
    static constexpr float kRandFrac = 1.f / MAX_XORSHIFT_VALUE;
};
} // namespace daisysp
#endif // __KIWI_SYNTH_KIWI_DUST_H__
