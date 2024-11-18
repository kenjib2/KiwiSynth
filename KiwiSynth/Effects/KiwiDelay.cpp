#include "KiwiDelay.h"



namespace kiwisynth
{



float DSY_SDRAM_BSS delayLine[2][MAX_DELAY_SAMPLES];



void KiwiDelay::Init()
{
    delaySamples_ = MIN_DELAY_SAMPLES;
    level_ = 0.0f;
    feedback_ = 0.0f;
    memset(delayLine[0], 0, MAX_DELAY_SAMPLES * sizeof(float));
    memset(delayLine[1], 0, MAX_DELAY_SAMPLES * sizeof(float));
    writeIndex_ = 0;
}



void KiwiDelay::Process(float* input)
{
    writeIndex_++;
    writeIndex_ &= DELAY_POINTER_MASK;
    int readIndex = (writeIndex_ - delaySamples_);
    readIndex &= DELAY_POINTER_MASK;

    float readL = delayLine[0][readIndex];
    float readR = delayLine[1][readIndex];

    delayLine[0][writeIndex_] = input[0] + readL * feedback_;
    delayLine[1][writeIndex_] = input[1] + readL * feedback_;

    input[0] = input[0] + readL * level_;
    input[1] = input[1] + readR * level_;
}



} // namespace kiwisynth
