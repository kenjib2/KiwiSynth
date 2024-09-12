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
            void Process(float* sample, PatchSettings* patchSettings, float mod);

            float GetLastSample();
    };
}


#endif // __KIWI_SYNTH_NOISE_H__
