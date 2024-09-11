#ifndef __KIWI_SYNTH_EFFECTS_ENGINE_H__
#define __KIWI_SYNTH_EFFECTS_ENGINE_H__


#include "daisysp.h"

#include "../../KUtils.h"
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
            #ifdef __FUNCTIONALITY_OPTION__
                void ProcessReverbOnly(float* sample);
            #endif // __FUNCTIONALITY_OPTION__
            void Process(float* sample);
    };
}


#endif // __KIWI_SYNTH_EFFECTS_ENGINE_H__
