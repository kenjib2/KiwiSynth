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

namespace kiwisynth
{

    /*
     * The effects engine operates independently from the synthesis modules. The effects chain is configurable
     * using EffectsMode and ReverbMode values. Process then takes an audio sample input and applies the effects
     * to the sample.
     */
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

            /*
             * Configure the effects parameters based on user input controls.
             */
            void UpdateSettings();
            /*
             * Changes between reverb modes, setting up the various Zita parameters that each mode uses.
             */
            void UpdateReverbMode(ReverbMode newMode);
            /*
             * Process a stereo input and apply the effects. The sample argument should be a pointer to two
             * float values. The first one is the left sample, and the second one is the right sample.
             */
            void Process(float* sample);
    };

} // namespace kiwisynth
#endif // __KIWI_SYNTH_EFFECTS_ENGINE_H__
