#include "KiwiTriangle.h"


namespace kiwi_synth
{

    float KiwiTriangle::Process()
    {
        float out, t;

        t   = -1.0f + (2.0f * phase_);
        out = 2.0f * (fabsf(t) - 0.5f);

        phase_ += phase_inc_;
        if(phase_ > 1.0f)
        {
            phase_ -= 1.0f;
            eoc_ = true;
        }
        else
        {
            eoc_ = false;
        }
        eor_ = (phase_ - phase_inc_ < 0.5f && phase_ >= 0.5f);

        return out * amp_;
    }

} // namespace kiwi_synth
