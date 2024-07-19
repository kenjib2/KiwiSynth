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
            float      level;
            bool       isOn;
            bool       noteTriggered;
            WhiteNoise noise;

        public:
            Noise() {}
            ~Noise() {}
            void Init(PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void Process(float* sample, float* mods, uint8_t numMods);
    };
}


#endif // __KIWI_SYNTH_NOISE_H__
