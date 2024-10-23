#include "KiwiFastSine.h"


namespace kiwi_synth
{

    float KiwiFastSine::Process()
    {
        float out;

        out = fastCos(phase_ * TWOPI_F);

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

    float KiwiFastSine::Process(float phaseMod)
    {
        float out;
        float calculatedPhase = phase_ + phaseMod;
        if(calculatedPhase > 1.0f)
        {
            calculatedPhase -= 1.0f;
        }

        out = fastCos(calculatedPhase * TWOPI_F);

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
