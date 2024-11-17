#include "KiwiDecimator.h"

namespace kiwisynth
{

void KiwiDecimator::Init()
{
    downsample_factor_ = 1.0f;
    bitcrush_factor_   = 0.0f;
    downsampled_       = 0.0f;
    bitcrushed_        = 0.0f;
    inc_               = 0;
    threshold_         = 0;
    smooth_crushing_   = false;
    bit_overflow_      = 1.0f;
    level_             = 1.0f;
}

float KiwiDecimator::Process(float input)
{
    int32_t temp;
    //downsample
    threshold_ = (uint32_t)((downsample_factor_ * downsample_factor_) * 96.0f);
    inc_ += 1;
    if(inc_ > threshold_)
    {
        inc_         = 0;
        downsampled_ = input;
    }

    //bitcrush
    if(smooth_crushing_)
    {
        temp = (int32_t)(downsampled_ * 65536.0f * bit_overflow_);
        temp >>= bits_to_crush_ + 1; // shift off
        temp <<= bits_to_crush_ + 1; // move back with zeros
        bitcrushed_ = (float)temp / (65536.0f * bit_overflow_);
    }
    else
    {
        temp = (int32_t)(downsampled_ * 65536.0f);
        temp >>= bits_to_crush_; // shift off
        temp <<= bits_to_crush_; // move back with zeros
        bitcrushed_ = (float)temp / 65536.0f;
    }

    return bitcrushed_ * level_;
}

} // namespace kiwisynth
