#ifndef __KIWI_SYNTH_VCO_H__
#define __KIWI_SYNTH_VCO_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      VCO_MIN_FREQUENCY = 30.0f;
    static const float      VCO_MAX_FREQUENCY = 15000.0f;
    
    class VCO
    {
        private:
            PatchSettings* patchSettings;
            uint8_t    vcoNumber;
            float      pulseWidth;
            float      waveFolderGain;
            float      level;
            float      fineTune; // mod to midi note -1 to 1
            float      interval; // mod to midi note -11 to 11
            float      octave; // mod to midi note -24, -12, 0, 12, 24
            float      masterTune; // mod to midi note -1.0f to 1.0f
            float      playingNote; // midiNote with fineTune, interval, octave, and masterTune applied.
            bool       isOn;
            bool       noteTriggered;
            int        waveform;
            Oscillator osc;
            Wavefolder wavefolder;

        public:
            float        midiNote; // 0 to 127 with portamento applied. Needs mtof to make a frequency.

            VCO() {}
            ~VCO() {}
            void Init(PatchSettings* patchSettings, float sampleRate, uint8_t vcoNumber);

            void UpdateSettings();
            void Process(float* sample, float mod, float pwMod, bool fullFunctionality = true);
    };
}


#endif // __KIWI_SYNTH_VCO_H__
