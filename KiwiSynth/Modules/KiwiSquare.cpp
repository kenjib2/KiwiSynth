#include "KiwiSquare.h"


namespace kiwi_synth
{

    float KiwiSquare::Process()
    {
        float out;

        out = phase_ < pw_ ? (1.0f) : -1.0f;

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

        return out * amp_;
    }

} // namespace kiwi_synth
