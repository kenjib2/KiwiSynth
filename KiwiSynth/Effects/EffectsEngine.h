#ifndef __KIWI_SYNTH_EFFECTS_ENGINE_H__
#define __KIWI_SYNTH_EFFECTS_ENGINE_H__


#include "daisysp.h"

#include "../Patch/Patch.h"
#include "Delay.h"
#include "Zita/reverb.h"

using namespace daisysp;

namespace kiwi_synth
{

    class EffectsEngine
    {
        private:
            EffectsMode             effectsMode;
            ReverbMode              reverbMode;
            float                   reverbLevel;
            float                   gain;
            float                   level;
            Patch*                  patch;
            Reverb                  reverb;
            Delay                   delay;

        public:
            EffectsEngine() {}
            ~EffectsEngine() {}
            void Init(Patch* patch, float sampleRate);

            void UpdateSettings();
            void UpdateEffectsMode(EffectsMode newMode);
            void UpdateReverbMode(ReverbMode newMode);
            void Process(float* sample);
    };
}

#endif // __KIWI_SYNTH_EFFECTS_ENGINE_H__
