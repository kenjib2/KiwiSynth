#ifndef __KIWI_SYNTH_DELAY_H__
#define __KIWI_SYNTH_DELAY_H__

#include "daisy_seed.h"

#include "IEffect.h"

namespace kiwi_synth
{
    const int MAX_DELAY_SAMPLES =   0b10000000000000000; // 65536
    const int DELAY_POINTER_MASK =  0b01111111111111111;
    const int MIN_DELAY_SAMPLES =   1200;
    const int DEBOUNCE_RESOLUTION = 100;

    class KiwiDelay : public IEffect
    {
        private:
            int delaySamples;   // MIN_DELAY_SAMPLES to MAX_DELAY_SAMPLES
            float level;        // 0.0f to 1.0f
            float feedback;     // 0.0f to 1.0f
            int writeIndex = 0;
            int bufferNumber;

        public:
            void Init(int bufferNumber);
            float Process(float input);

            // We need to debounce these values here. Otherwise the pointer will jump around and cause pops.
            inline void SetDelaySamples(int delaySamples) { if (abs(delaySamples - this->delaySamples) > DEBOUNCE_RESOLUTION) this->delaySamples = delaySamples; }
            inline void SetLevel(float level) { this->level = level; }
            inline void SetFeedback(float feedback) { this->feedback = feedback; }
    };
}


#endif // __KIWI_SYNTH_EFFECTS_ENGINE_H__
