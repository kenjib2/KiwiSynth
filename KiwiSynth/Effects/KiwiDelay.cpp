#include "KiwiDelay.h"

namespace kiwi_synth
{
    float DSY_SDRAM_BSS delayLine[2][MAX_DELAY_SAMPLES];

    void KiwiDelay::Init(int bufferNumber)
    {
        this->bufferNumber = bufferNumber;
        delaySamples = MIN_DELAY_SAMPLES;
        level = 0.0f;
        feedback = 0.0f;
        memset(delayLine[bufferNumber], 0, MAX_DELAY_SAMPLES * sizeof(float));
        writeIndex = 0;
    }

    float KiwiDelay::Process(float input)
    {
        writeIndex++;
        writeIndex &= DELAY_POINTER_MASK;
        int readIndex = (writeIndex - delaySamples);
        readIndex &= DELAY_POINTER_MASK;

        float read = delayLine[bufferNumber][readIndex];

        delayLine[bufferNumber][writeIndex] = input + read * feedback;

        return input + read * level;
    }
}
