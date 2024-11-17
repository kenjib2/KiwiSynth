#include "KiwiPBSaw.h"


namespace kiwisynth
{

    static inline float Polyblep(float phase_inc, float phase_inc_recip, float t);

    float KiwiPBSaw::Process()
    {
        float out, t;

        t   = phase_;
        out = (2.0f * t) - 1.0f;
        out -= Polyblep(phase_inc_, phase_inc_recip_, t);
        out *= -1.0f;

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

    float KiwiPBSaw::Process(float phaseMod)
    {
        float out, t;
        float calculatedPhase = phase_ + phaseMod;
        if(calculatedPhase > 1.0f)
        {
            calculatedPhase -= 1.0f;
        }

        t   = calculatedPhase;
        out = (2.0f * t) - 1.0f;
        out -= Polyblep(phase_inc_, phase_inc_recip_, t);
        out *= -1.0f;

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

    static float Polyblep(float phase_inc, float phase_inc_recip, float t)
    {
        float dt = phase_inc;
        float dt_recip = phase_inc_recip;
        if(t < dt)
        {
            t *= dt_recip;
            return t + t - t * t - 1.0f;
        }
        else if(t > 1.0f - dt)
        {
            t = (t - 1.0f) * dt_recip;
            return t * t + t + t + 1.0f;
        }
        else
        {
            return 0.0f;
        }
    }

} // namespace kiwisynth
