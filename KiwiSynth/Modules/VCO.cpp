#include "VCO.h"

namespace kiwi_synth
{
    void VCO::Init(float sampleRate, uint8_t vcoNumber)
    {
        this->vcoNumber = vcoNumber;
        isOn = true;
        waveform = 0;
        pulseWidth = 0.5f;
        level = 1.0f;
        fineTune = 0.0f;
        interval = 0.0f;
        octave = 0.0f;
        midiNote = 60;
    	osc.Init(sampleRate);
        osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
        osc.SetFreq(220.0f);
        osc.SetAmp(1.0f);
        wavefolder.Init();
        wavefolder.SetGain(1.0f);
    }

    void VCO::UpdateSettings(PatchSettings* patchSettings)
    {
        isOn = patchSettings->getBoolValue((PatchSetting)(VCO_1_ON + vcoNumber));
        if (isOn) {
            masterTune = patchSettings->getFloatValueLinear(PatchSetting::VCO_MASTER_TUNE, -1.0f, 1.0f);

            waveform = patchSettings->getIntValue((PatchSetting)(VCO_1_WAVEFORM + vcoNumber));
            pulseWidth = 0.53f - patchSettings->getFloatValueLinear((PatchSetting)(VCO_1_PULSE_WIDTH + vcoNumber), 0.03f, 0.5f);
            level = patchSettings->getFloatValue((PatchSetting)(VCO_1_LEVEL + vcoNumber));
            if (vcoNumber > 0) {
                int vcoMod = vcoNumber - 1;
                level = patchSettings->getFloatValue((PatchSetting)(VCO_2_LEVEL + vcoMod));
                fineTune = patchSettings->getFloatValueLinear((PatchSetting)(VCO_2_FINE_TUNE + vcoMod), -1.0f, 1.0);
                interval = (float)(patchSettings->getIntValue((PatchSetting)(VCO_2_INTERVAL + vcoMod)) - 11);
                octave = (float)((patchSettings->getIntValue((PatchSetting)(VCO_2_OCTAVE + vcoMod)) - 2) * 12);
            }

            osc.SetWaveform(7 - waveform); // 7: WAVE_POLYBLEP_SQUARE, 6: WAVE_POLYBLEP_SAW, 5: WAVE_POLYBLEP_TRI
            waveFolderGain = std::fmax(1.0f + (0.495f - pulseWidth) * 55, 1.0f);
        }
    }

    #ifdef __FUNCTIONALITY_OPTION__
    void VCO::Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, bool fullFunctionality)
    #else
    void VCO::Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod)
    #endif // __FUNCTIONALITY_OPTION__
    {
        if (isOn) {
            playingNote = midiNote + octave + interval + fineTune + masterTune;

            float waveSample;
            osc.SetFreq(std::fmax(mtof(playingNote + mod * 12), 0.0f));

            #ifdef __FUNCTIONALITY_OPTION__
            if (fullFunctionality) {
            #endif // __FUNCTIONALITY_OPTION__
                osc.SetPw(pulseWidth + pwMod);

                waveSample = osc.Process();
                if (waveform == 2) { // Triangle
                    wavefolder.SetGain(std::fmax(waveFolderGain + pwMod * 27, 1.0f));
                    waveSample = wavefolder.Process(waveSample);
                } else if (waveform == 1) { // Sawtooth
                    waveSample = fclamp(waveSample * (waveFolderGain + pwMod + 1.0f) / 2, -1.0f, 1.0f);
                }

                *sample = waveSample * level;
            #ifdef __FUNCTIONALITY_OPTION__
            } else {
                osc.SetPw(pulseWidth);
                *sample = osc.Process() * level;
            }
             #endif // __FUNCTIONALITY_OPTION__
        }
        else {
            *sample = 0.0f;
        }
    }
}
