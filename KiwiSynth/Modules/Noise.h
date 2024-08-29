#ifndef __KIWI_SYNTH_NOISE_H__
#define __KIWI_SYNTH_NOISE_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    class Noise
    {
        private:
            PatchSettings* patchSettings;
            int        noiseType;
            float      level;
            float      lastSample;
            bool       isOn;
            bool       noteTriggered;
            WhiteNoise white;
            Dust       dust;

        public:
            Noise() {}
            ~Noise() {}
            void Init(PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void Process(float* sample, float mod);

            float GetLastSample();
    };
}


#endif // __KIWI_SYNTH_NOISE_H__
