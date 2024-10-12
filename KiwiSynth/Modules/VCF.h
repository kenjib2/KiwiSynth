#ifndef __KIWI_SYNTH_VCF_H__
#define __KIWI_SYNTH_VCF_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"
#include "MoogLadder.h"
#include "KiwiSvf.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      VCF_MIN_FREQUENCY = 100.0f;
    static const float      VCF_MAX_FREQUENCY = 15000.0f;

    /*
     * The filter module for the Kiwi Synth. Multiple filter types are provided.
     */
    class VCF
    {
        private:
            bool                    noteTriggered;
            float                   frequency;
            float                   resonance;
            FilterType              filterType;
            infrasonic::MoogLadder  filter;
            OnePole                 opFilter;
            KiwiSvf                 svFilter;

        public:
            VCF() {}
            ~VCF() {}
            void Init(float sampleRate);

            /*
             * Applies user settings to VCA parameters.
             */
            void UpdateSettings(PatchSettings* patchSettings);
            /*
             * Applies filtering to the incoming sample. Multiple different types of modulation are managed separately. TrackingMod
             * and currentMidiNote are used in conjunction to apply keyboard tracking. Mod is the modulation to frequency cutoff.
             * ResMod is the modulation to resonance.
             */
            void Process(float* sample, PatchSettings* patchSettings, float trackingMod, int currentMidiNote, float mod, float resMod);
    };
}


#endif // __KIWI_SYNTH_VCF_H__
