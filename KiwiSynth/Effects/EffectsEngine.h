#ifndef __KIWI_SYNTH_EFFECTS_ENGINE_H__
#define __KIWI_SYNTH_EFFECTS_ENGINE_H__


#include "daisysp.h"

#include "../Patch/Patch.h"
#include "Zita/reverb.h"
#include "Delay.h"
#include "KiwiChorus.h"
#include "KiwiPhaser.h"
#include "KiwiFlanger.h"

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
            float                   distortionLevel;
            float                   freq;
            float                   chorusDepth;
            float                   phaserDepth;
            float                   feedback;
            float                   downsampleFactor;
            float                   bitcrushFactor;
            float                   decimatorLevel;
            Patch*                  patch;
            Reverb                  reverb;
            Delay                   delay;
            KiwiChorus              chorusL;
            KiwiChorus              chorusR;
            KiwiPhaser              phaserL;
            KiwiPhaser              phaserR;
            KiwiFlanger             flangerL;
            KiwiFlanger             flangerR;
            Decimator               decimatorL;
            Decimator               decimatorR;

        public:
            EffectsEngine() {}
            ~EffectsEngine() {}
            void Init(Patch* patch, float sampleRate);

            void UpdateSettings();
            void UpdateReverbMode(ReverbMode newMode);
            void Process(float* sample);
    };
}

#endif // __KIWI_SYNTH_EFFECTS_ENGINE_H__
