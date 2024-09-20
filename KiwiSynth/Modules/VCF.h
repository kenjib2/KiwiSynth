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

            void UpdateSettings(PatchSettings* patchSettings);
            void Process(float* sample, PatchSettings* patchSettings, float trackingMod, int currentMidiNote, float mod, float resMod);
    };
}


#endif // __KIWI_SYNTH_VCF_H__
