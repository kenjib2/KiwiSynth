#ifndef __KIWI_SYNTH_DISTORTION_H__
#define __KIWI_SYNTH_DISTORTION_H__

#include "daisy_seed.h"

#include "IEffect.h"

namespace kiwi_synth
{
    const float ATAN_DEBOUNCE_THRESHOLD = 5.1f;

    class KiwiDistortion : public IEffect {
        private:
            float gain;
            float level;

        public:
            void Init() {}
            inline float Process(float input) {
                if (gain > ATAN_DEBOUNCE_THRESHOLD) {
                    return atan(input * gain) * level; // atan has a limit of pi/2 or @1.57. The max of level is 0.22 so total output will never exceed +/- 0.346
                }
                return input;
            }
            inline void SetGain(float gain) { this->gain = gain; }
            inline void SetLevel(float level) { this->level = level; }
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_DISTORTION_H__