#include "VCO.h"

namespace kiwi_synth
{
    void VCO::Init(float sampleRate, uint8_t vcoNumber)
    {
        this->vcoNumber = vcoNumber;
        isOn = true;
        waveform = 0;
        pulseWidth = 0.5f;
        variShape = 0.5f;
        level = 1.0f;
        fineTune = 0.0f;
        interval = 0.0f;
        octave = 0.0f;
        midiNote = 60;
    	osc.Init(sampleRate);
        osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
        osc.SetFreq(220.0f);
        osc.SetAmp(1.0f);
        variOsc.Init(sampleRate);
        variOsc.SetWaveshape(0.0f);
        variOsc.SetSync(false);
        variSaw.Init(sampleRate);
        variSaw.SetWaveshape(0.0f);
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
            variShape = patchSettings->getFloatValue((PatchSetting)(VCO_1_PULSE_WIDTH + vcoNumber));
            waveFolderGain = 1.0f + variShape * 27.0f;
            sawtoothGain = variShape * variShape * 50.0f;
            level = patchSettings->getFloatValue((PatchSetting)(VCO_1_LEVEL + vcoNumber));
            if (vcoNumber > 0) {
                int vcoMod = vcoNumber - 1;
                level = patchSettings->getFloatValue((PatchSetting)(VCO_2_LEVEL + vcoMod));
                fineTune = patchSettings->getFloatValueLinear((PatchSetting)(VCO_2_FINE_TUNE + vcoMod), -1.0f, 1.0);
                interval = (float)(patchSettings->getIntValue((PatchSetting)(VCO_2_INTERVAL + vcoMod)) - 11);
                octave = (float)((patchSettings->getIntValue((PatchSetting)(VCO_2_OCTAVE + vcoMod)) - 2) * 12);
            }

            osc.SetWaveform(7 - waveform); // 7: WAVE_POLYBLEP_SQUARE, 6: WAVE_POLYBLEP_SAW, 5: WAVE_POLYBLEP_TRI
        }
    }

    void VCO::Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod)
    {
        if (isOn) {
            playingNote = midiNote + octave + interval + fineTune + masterTune;
            float waveSample = 0.0f;
            float freq = std::fmax(mtof(playingNote + mod * 12.0f), 0.0f);
            switch(waveform) {
                case WAVEFORM_SQUARE:
                    osc.SetFreq(freq);
                    osc.SetPw(pulseWidth + pwMod);
                    waveSample = osc.Process();
                    break;
                case WAVEFORM_SAWTOOTH:
                    osc.SetFreq(freq);
                    osc.SetPw(pulseWidth + pwMod);
                    waveSample = osc.Process();
                    waveSample = fclamp(waveSample * (sawtoothGain + pwMod * 50.0f + 1.0f), -1.0f, 1.0f);
                    break;
                case WAVEFORM_TRIANGLE:
                    osc.SetFreq(freq);
                    osc.SetPw(pulseWidth + pwMod);
                    waveSample = osc.Process();
                    wavefolder.SetGain(std::fmax(waveFolderGain + pwMod * 27.0f, 1.0f));
                    waveSample = wavefolder.Process(waveSample);
                    break;
                case WAVEFORM_VARISHAPE:
                    variOsc.SetSyncFreq(freq);
                    variOsc.SetPW((variShape + pwMod) * 0.25f);
                    waveSample = variOsc.Process();
                    break;
                case WAVEFORM_VARISAW:
                    variSaw.SetFreq(freq);
                    variSaw.SetPW(1.0f - variShape + pwMod);
                    waveSample = variSaw.Process();
                    break;
            }

            *sample = waveSample * level;
        }
        else {
            *sample = 0.0f;
        }
    }
}
