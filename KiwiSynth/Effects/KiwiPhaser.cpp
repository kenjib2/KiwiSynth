#include "daisysp.h"
#include "KiwiPhaser.h"
#include <math.h>

using namespace daisysp;

namespace kiwi_synth
{
//PhaserEngine stuff
void KiwiPhaserEngine::Init(float sample_rate, int bufferNumber)
{
    sample_rate_ = sample_rate;

    del_.Init(bufferNumber, kDelayLength);
    lfo_amp_  = 0.f;
    feedback_ = .2f;
    SetFreq(200.f);

    del_.SetDelay(0.f);

    os_ = 30.f; //30 hertz frequency offset, lower than this introduces crunch
    deltime_ = 0.f;

    last_sample_ = 0.f;
    lfo_phase_   = 0.f;
    SetLfoFreq(.3);
    SetLfoDepth(.9);
}

float KiwiPhaserEngine::Process(float in)
{
    float lfo_sig = ProcessLfo();
    fonepole(deltime_, sample_rate_ / (lfo_sig + ap_freq_ + os_), .0001f);

    last_sample_ = del_.Allpass(in + feedback_ * last_sample_, deltime_, .3f);

    return (in + last_sample_);
}

void KiwiPhaserEngine::SetLfoDepth(float depth)
{
    lfo_amp_ = fclamp(depth, 0.f, 1.f);
}

void KiwiPhaserEngine::SetLfoFreq(float lfo_freq)
{
    lfo_freq = 4.f * lfo_freq / sample_rate_;
    lfo_freq *= lfo_freq_ < 0.f ? -1.f : 1.f; //if we're headed down, keep going
    lfo_freq_ = fclamp(lfo_freq, -.25f, .25f); //clip at +/- .125 * sr
}

void KiwiPhaserEngine::SetFreq(float ap_freq)
{
    ap_freq_ = fclamp(ap_freq, 0.f, 20000.f); //0 - 20kHz
}

void KiwiPhaserEngine::SetFeedback(float feedback)
{
    feedback_ = fclamp(feedback, 0.f, .75f);
}

float KiwiPhaserEngine::ProcessLfo()
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

    return lfo_phase_ * lfo_amp_ * ap_freq_;
}

//Phaser Stuff
void KiwiPhaser::Init(float sample_rate, int bufferNumber)
{
    for(size_t i = 0; i < kMaxPoles; i++)
    {
        engines_[i].Init(sample_rate, bufferNumber);
    }

    poles_     = 4;
    inv_poles_ = 1.0f / (float)poles_;
    gain_frac_ = .5f;
}

float KiwiPhaser::Process(float in)
{
    float sig = 0.f;

    for(int i = 0; i < poles_; i++)
    {
        sig += engines_[i].Process(in) * inv_poles_; // Equal mix
    }

    return sig;
}

void KiwiPhaser::SetPoles(int poles)
{
    poles_ = DSY_CLAMP(poles, 1, 8);
    inv_poles_ = 1.0f / (float)poles_;
}

void KiwiPhaser::SetLfoDepth(float depth)
{
    for(int i = 0; i < kMaxPoles; i++)
    {
        engines_[i].SetLfoDepth(depth);
    }
}

void KiwiPhaser::SetLfoFreq(float lfo_freq)
{
    for(int i = 0; i < kMaxPoles; i++)
    {
        engines_[i].SetLfoFreq(lfo_freq);
    }
}

void KiwiPhaser::SetFreq(float ap_freq)
{
    for(int i = 0; i < kMaxPoles; i++)
    {
        engines_[i].SetFreq(ap_freq);
    }
}

void KiwiPhaser::SetFeedback(float feedback)
{
    for(int i = 0; i < kMaxPoles; i++)
    {
        engines_[i].SetFeedback(feedback);
    }
}
} // namespace kiwi_synth
