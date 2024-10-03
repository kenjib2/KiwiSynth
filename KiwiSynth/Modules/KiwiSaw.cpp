#include "KiwiSaw.h"


namespace kiwi_synth
{

    float KiwiSaw::Process()
    {
        float out;

        out = -1.0f * (((phase_ * 2.0f)) - 1.0f);

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
