#ifndef __KIWI_SYNTH_DISTORTION_H__
#define __KIWI_SYNTH_DISTORTION_H__

#include "daisy_seed.h"

#include "IEffect.h"

namespace kiwisynth
{
    const float ATAN_DEBOUNCE_THRESHOLD = 5.1f;

    // Simple atan distortion effect
    class KiwiDistortion : public IEffect {
        private:
            float gain;
            float level;

        public:
            void Init() {}
            /*
             * Applies atan to the signal for distortion. This is a relatively expensive operation.
             */
            inline float Process(float input) {
                if (__builtin_expect(gain > ATAN_DEBOUNCE_THRESHOLD, 1)) {
                    return atan(input * gain) * level; // atan has a limit of pi/2 or @1.57. The max of level is 0.22 so total output will never exceed +/- 0.346
                }
                return input;
            }
            /*
             * Sets the pre-distortion gain, which will determine the amount of clipping.
             */
            inline void SetGain(float gain) { this->gain = gain; }
            /*
             * An output level to account for changes in perceived/RMS volume due to increased gain.
             */
            inline void SetLevel(float level) { this->level = level; }
    };

} // namespace kiwisynth
#endif // __KIWI_SYNTH_DISTORTION_H__