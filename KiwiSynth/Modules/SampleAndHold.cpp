#include "SampleAndHold.h"



using namespace kiwisynth;



void SampleAndHold::Init(float sampleRate)
{
    minimumRate_ = sampleRate / 1000; // 1 ms
    maximumRate_ = sampleRate; // 1 sec

    rate_ = minimumRate_ * 100;
    counter_ = 0;
    currentSample_ = 0.0f;
}
