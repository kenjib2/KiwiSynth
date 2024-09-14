#include "Delay.h"

namespace kiwi_synth
{
    float DSY_SDRAM_BSS delayLine[2][MAX_DELAY_SAMPLES];

    void Delay::Init()
    {
        delaySamples = MIN_DELAY_SAMPLES;
        level = 0.0f;
        feedback = 0.0f;
        memset(delayLine[0], 0, MAX_DELAY_SAMPLES * sizeof(float));
        memset(delayLine[1], 0, MAX_DELAY_SAMPLES * sizeof(float));
        writeIndex = 0;
    }

    void Delay::Process(float* input)
    {
        writeIndex++;
        writeIndex &= DELAY_POINTER_MASK;
        int readIndex = (writeIndex - delaySamples);
        readIndex &= DELAY_POINTER_MASK;

        float readL = delayLine[0][readIndex];
        float readR = delayLine[1][readIndex];

        delayLine[0][writeIndex] = input[0] + readL * feedback;
        delayLine[1][writeIndex] = input[1] + readL * feedback;

        input[0] = input[0] + readL * level;
        input[1] = input[1] + readR * level;
    }
}
