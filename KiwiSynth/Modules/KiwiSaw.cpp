#include "KiwiSaw.h"



using namespace kiwisynth;



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

    return out * amp_;
}
