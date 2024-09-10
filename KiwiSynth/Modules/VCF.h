#ifndef __KIWI_SYNTH_VCF_H__
#define __KIWI_SYNTH_VCF_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"
#include "MoogLadder.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      VCF_MIN_FREQUENCY = 100.0F;
    static const float      VCF_MAX_FREQUENCY = 15000.0F;

    enum FilterType {
        LADDER_LOWPASS,
        SVF_LOWPASS,
        SVF_HIGHPASS,
        SVF_BANDPASS,
        SVF_NOTCH,
        SVF_PEAK,
        ONE_POLE_LOWPASS,
        ONE_POLE_HIGHPASS
    };
    
    class VCF
    {
        private:
            bool                    noteTriggered;
            float                   frequency;
            float                   resonance;
            FilterType              filterType;
            infrasonic::MoogLadder  filter;
            OnePole                 opFilter;
            Svf                     svFilter;
            PatchSettings*          patchSettings;

        public:
            VCF() {}
            ~VCF() {}
            void Init(PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void Process(float* sample, float trackingMod, int currentMidiNote, float mod, float resMod);
    };
}


#endif // __KIWI_SYNTH_VCF_H__
