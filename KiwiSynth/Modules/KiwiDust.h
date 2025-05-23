/*
Copyright (c) 2020 Electrosmith, Corp, Emilie Gillet

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#ifndef __KIWI_SYNTH_KIWI_DUST_H__
#define __KIWI_SYNTH_KIWI_DUST_H__



#include "Utility/dsp.h"
#include "../Util/KUtils.h"
/** @file KiwiDust.h */



using namespace daisysp;

namespace kiwisynth
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



    inline float Process()
    {
        float u           = rng->xorshift32() * kRandFrac;
        if(u < density_)
        {
            return u * inv_density_;
        }
        return 0.0f;
    }



    void SetDensity(float density)
    {
        density_ = fclamp(density, 0.f, 1.f);
        density_ = density_ * .3f;
        inv_density_ = 1.0f / density_;
    }



  private:
    xorshiftPRNG*          rng;
    float                  density_;
    float                  inv_density_;
    static constexpr float kRandFrac = 1.f / xorshiftPRNG::MAX_XORSHIFT_VALUE;
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_KIWI_DUST_H__
