#ifndef __KIWI_SYNTH_DELAY_H__
#define __KIWI_SYNTH_DELAY_H__



#include "daisy_seed.h"



namespace kiwisynth
{



const int MAX_DELAY_SAMPLES =   0b10000000000000000; // 65536
const int DELAY_POINTER_MASK =  0b01111111111111111;
const int MIN_DELAY_SAMPLES =   1200;
const int DEBOUNCE_RESOLUTION = 100;



/*
 * A very simple delay effect. This delay uses pointer distance for delay time, so there
 * will be audible artifacts when changing the delay time. This is done to avoid interpolating
 * and multiple buffer writes per sample, thus using the fewest clock cycles to operate.
 * Because of the artifacts when changing delay time, we also debounce the signal to keep it
 * from jittering from analog noise on the pots.
 * 
 * Another optimization is using bitwise & operations instead of the expensive modulus operator.
 * In order to do this, the buffer size must always be a power of two and the mask must always
 * be the buffer size minus one.
 */
class KiwiDelay
{
    private:
        int delaySamples_;   // MIN_DELAY_SAMPLES to MAX_DELAY_SAMPLES
        float level_;        // 0.0f to 1.0f
        float feedback_;     // 0.0f to 1.0f
        int writeIndex_ = 0;

    public:
        void Init();
        void Process(float* input);  // input[0] = left, input[1] = right



        // We need to debounce these values here. Otherwise the pointer will jump around and cause pops.
        inline void SetDelaySamples(int delaySamples) { if (abs(delaySamples - delaySamples_) > DEBOUNCE_RESOLUTION) delaySamples_ = delaySamples; }



        inline void SetLevel(float level) { level_ = level; }



        inline void SetFeedback(float feedback) { feedback_ = feedback; }
};



}// namespace kiwisynth
#endif // __KIWI_SYNTH_EFFECTS_ENGINE_H__
