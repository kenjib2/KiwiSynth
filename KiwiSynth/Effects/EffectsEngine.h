#ifndef __KIWI_SYNTH_EFFECTS_ENGINE_H__
#define __KIWI_SYNTH_EFFECTS_ENGINE_H__


#include "daisysp.h"

#include "../Patch/Patch.h"
#include "Zita/reverb.h"
#include "KiwiDistortion.h"
#include "KiwiDelay.h"
#include "KiwiChorus.h"
#include "KiwiPhaser.h"
#include "KiwiFlanger.h"
#include "KiwiDecimator.h"

using namespace daisysp;

namespace kiwi_synth
{

    class EffectsEngine
    {
        private:
            EffectsMode             effectsMode;
            ReverbMode              reverbMode;

            Patch*                  patch;

            Reverb                  reverb;
            KiwiDistortion          distortion;
            KiwiDelay               delay;
            KiwiChorus              chorusL;
            KiwiChorus              chorusR;
            KiwiPhaser              phaserL;
            KiwiPhaser              phaserR;
            KiwiFlanger             flangerL;
            KiwiFlanger             flangerR;
            KiwiDecimator           decimatorL;
            KiwiDecimator           decimatorR;

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
