#ifndef __KIWI_SYNTH_NOISE_H__
#define __KIWI_SYNTH_NOISE_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"
#include "KiwiDust.h"

using namespace daisysp;

namespace kiwi_synth
{
    class Noise
    {
        private:
            int        noiseType;
            float      level;
            float      lastSample;
            bool       isOn;
            bool       noteTriggered;
            WhiteNoise white;
            KiwiDust   dust;

        public:
            Noise() {}
            ~Noise() {}
            void Init(float sampleRate);

            void UpdateSettings(PatchSettings* patchSettings);
            inline void Process(float* sample, PatchSettings* patchSettings, float mod)
            {
                if (noiseType == 0) {
                    lastSample = white.Process() * 0.2;
                } else {
                    lastSample = dust.Process();
                }

                if (isOn) {
                    *sample = lastSample * fclamp((level + mod), -1.0f, 1.0f);
                }
            }

            inline float GetLastSample() { return lastSample; }
    };
}


#endif // __KIWI_SYNTH_NOISE_H__
