#ifndef __KIWI_SYNTH_VCO_H__
#define __KIWI_SYNTH_VCO_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      VCO_MIN_FREQUENCY = 30.0F;
    static const float      VCO_MAX_FREQUENCY = 15000.0F;
    
    class VCO
    {
        private:
            PatchSettings* patchSettings;
            uint8_t    vcoNumber;
            float      pulseWidth;
            float      waveFolderGain;
            float      level;
            float      fineTune;
            float      interval;
            float      octave;
            float      masterTune;
            float      baseFreq;
            float      freq;
            bool       isOn;
            bool       noteTriggered;
            int        midiNote;
            int        waveform;
            Oscillator osc;
            Wavefolder wavefolder;

            void CalculateFreq();

        public:
            VCO() {}
            ~VCO() {}
            void Init(PatchSettings* patchSettings, float sampleRate, uint8_t vcoNumber);

            void UpdateSettings();
            void Process(float* sample, float mod, float pwMod, bool fullFunctionality = true);
            void SetFreq(float frequency);
    };
}


#endif // __KIWI_SYNTH_VCO_H__
