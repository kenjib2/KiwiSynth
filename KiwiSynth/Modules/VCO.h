#ifndef __KIWI_SYNTH_VCO_H__
#define __KIWI_SYNTH_VCO_H__


#include "daisysp.h"

#include "../Patch/PatchSettings.h"
#include "KiwiPBSquare.h"
#include "KiwiPBSaw.h"
#include "KiwiTriangle.h"
#include "KiwiFastSine.h"

using namespace daisysp;

namespace kiwi_synth
{
    static const float      VCO_MIN_FREQUENCY = 30.0f;
    static const float      VCO_MAX_FREQUENCY = 15000.0f;
    
    class VCO
    {
        private:
            uint8_t    vcoNumber;
            float      pulseWidth;
            float      variShape;
            float      waveFolderGain;
            float      sawtoothGain;
            float      level;
            float      fineTune; // mod to midi note -1 to 1
            float      interval; // mod to midi note -11 to 11
            float      octave; // mod to midi note -24, -12, 0, 12, 24
            float      playingNote; // midiNote with fineTune, interval, octave, and masterTune applied.
            bool       isOn;
            bool       noteTriggered;
            int        waveform;
            KiwiPBSquare squareOsc;
            KiwiPBSaw    sawOsc;
            KiwiTriangle    triangleOsc;
            VariableShapeOscillator variOsc;
            VariableSawOscillator variSaw;
            KiwiFastSine sineOsc;
            Wavefolder wavefolder;

        public:
            float      midiNote; // 0 to 127 with portamento applied. Needs mtof to make a frequency.
            float      paraOffset; // Midi note for this specific VCO relative to 0

            VCO() {}
            ~VCO() {}
            void Init(float sampleRate, uint8_t vcoNumber);

            void UpdateSettings(PatchSettings* patchSettings);
            void Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod);
    };
}


#endif // __KIWI_SYNTH_VCO_H__
