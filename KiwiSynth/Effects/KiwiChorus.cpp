#include "daisysp.h"
#include "KiwiChorus.h"
#include <math.h>

using namespace daisysp;

namespace kiwi_synth
{
//ChorusEngine stuff
void KiwiChorusEngine::Init(float sample_rate, int bufferNumber)
{
    sample_rate_ = sample_rate;

    del_.Init(bufferNumber, kDelayLength);
    lfo_amp_  = 0.f;
    feedback_ = .2f;
    SetDelay(.75);

    lfo_phase_ = 0.f;
    SetLfoFreq(.3f);
    SetLfoDepth(.9f);
}

float KiwiChorusEngine::Process(float in)
{
    float lfo_sig = ProcessLfo();
    del_.SetDelay(lfo_sig + delay_);

    float out = del_.Read();
    del_.Write(in + out * feedback_);

    return (in + out) * .5f; //equal mix
}

void KiwiChorusEngine::SetLfoDepth(float depth)
{
    depth    = daisysp::fclamp(depth, 0.f, .93f);
    lfo_amp_ = depth * delay_;
}

void KiwiChorusEngine::SetLfoFreq(float freq)
{
    freq = 4.f * freq / sample_rate_;
    freq *= lfo_freq_ < 0.f ? -1.f : 1.f;  //if we're headed down, keep going
    lfo_freq_ = daisysp::fclamp(freq, -.25f, .25f); //clip at +/- .125 * sr
}

void KiwiChorusEngine::SetDelay(float delay)
{
    delay = (.1f + delay * 7.9f); //.1 to 8 ms
    SetDelayMs(delay);
}

void KiwiChorusEngine::SetDelayMs(float ms)
{
    ms     = daisysp::fmax(.1f, ms);
    delay_ = ms * .001f * sample_rate_; //ms to samples

    lfo_amp_ = daisysp::fmin(lfo_amp_, delay_); //clip this if needed
}

void KiwiChorusEngine::SetFeedback(float feedback)
{
    feedback_ = daisysp::fclamp(feedback, 0.f, 1.f);
}

float KiwiChorusEngine::ProcessLfo()
{
    lfo_phase_ += lfo_freq_;

    //wrap around and flip direction
    if(lfo_phase_ > 1.f)
    {
        lfo_phase_ = 1.f - (lfo_phase_ - 1.f);
        lfo_freq_ *= -1.f;
    }
    else if(lfo_phase_ < -1.f)
    {
        lfo_phase_ = -1.f - (lfo_phase_ + 1.f);
        lfo_freq_ *= -1.f;
    }

    return lfo_phase_ * lfo_amp_;
}

//Chorus Stuff
void KiwiChorus::Init(float sample_rate, int bufferNumber)
{
    engines_[0].Init(sample_rate, bufferNumber);
    engines_[1].Init(sample_rate, bufferNumber);
    SetPan(.25f, .75f);

    gain_frac_ = .5f;
    sigl_ = sigr_ = 0.f;
}

float KiwiChorus::Process(float in)
{
    sigl_ = 0.f;
    sigr_ = 0.f;

    for(int i = 0; i < 2; i++)
    {
        float sig = engines_[i].Process(in);
        sigl_ += (1.f - pan_[i]) * sig;
        sigr_ += pan_[i] * sig;
    }

    sigl_ *= gain_frac_;
    sigr_ *= gain_frac_;

    return sigl_;
}

float KiwiChorus::GetLeft()
{
    return sigl_;
}

float KiwiChorus::GetRight()
{
    return sigr_;
}

void KiwiChorus::SetPan(float panl, float panr)
{
    pan_[0] = daisysp::fclamp(panl, 0.f, 1.f);
    pan_[1] = daisysp::fclamp(panr, 0.f, 1.f);
}

void KiwiChorus::SetPan(float pan)
{
    SetPan(pan, pan);
}

void KiwiChorus::SetLfoDepth(float depthl, float depthr)
{
    engines_[0].SetLfoDepth(depthl);
    engines_[1].SetLfoDepth(depthr);
}

void KiwiChorus::SetLfoDepth(float depth)
{
    SetLfoDepth(depth, depth);
}

void KiwiChorus::SetLfoFreq(float freql, float freqr)
{
    engines_[0].SetLfoFreq(freql);
    engines_[1].SetLfoFreq(freqr);
}

void KiwiChorus::SetLfoFreq(float freq)
{
    SetLfoFreq(freq, freq);
}

void KiwiChorus::SetDelay(float delayl, float delayr)
{
    engines_[0].SetDelay(delayl);
    engines_[1].SetDelay(delayr);
}

void KiwiChorus::SetDelay(float delay)
{
    SetDelay(delay, delay);
}

void KiwiChorus::SetDelayMs(float msl, float msr)
{
    engines_[0].SetDelayMs(msl);
    engines_[1].SetDelayMs(msr);
}

void KiwiChorus::SetDelayMs(float ms)
{
    SetDelayMs(ms, ms);
}

void KiwiChorus::SetFeedback(float feedbackl, float feedbackr)
{
    engines_[0].SetFeedback(feedbackl);
    engines_[1].SetFeedback(feedbackr);
}

void KiwiChorus::SetFeedback(float feedback)
{
    SetFeedback(feedback, feedback);
}
} // namespace kiwi_synth
