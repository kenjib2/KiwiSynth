#ifndef __KIWI_SYNTH_EFFECTS_ENGINE_H__
#define __KIWI_SYNTH_EFFECTS_ENGINE_H__


#include "daisysp.h"

#include "../Patch/PatchSettings.h"
#include "Delay.h"
#include "Zita/reverb.h"

using namespace daisysp;

namespace kiwi_synth
{
    class EffectsEngine
    {
        private:
            float                   reverbLevel;
            float                   gain;
            float                   level;
            PatchSettings*          patchSettings;
            Reverb                  reverb;
            Delay                   delay;

        public:
            EffectsEngine() {}
            ~EffectsEngine() {}
            void Init(PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void ProcessReverbOnly(float* sample);
            void Process(float* sample);
    };
}


#endif // __KIWI_SYNTH_EFFECTS_ENGINE_H__
