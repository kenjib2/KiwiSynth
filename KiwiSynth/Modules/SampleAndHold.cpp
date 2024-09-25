#include "SampleAndHold.h"

namespace kiwi_synth
{
    void SampleAndHold::Init(float sampleRate)
    {
        SAMPLE_AND_HOLD_MINIMUM_RATE = sampleRate / 1000; // 1 ms
        SAMPLE_AND_HOLD_MAXIMUM_RATE = sampleRate; // 1 sec

        rate = SAMPLE_AND_HOLD_MINIMUM_RATE * 100;
        counter = 0;
        currentSample = 0.0f;
    }
}
