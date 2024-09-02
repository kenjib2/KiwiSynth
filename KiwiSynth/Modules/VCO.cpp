#include "VCO.h"

namespace kiwi_synth
{
    void VCO::Init(PatchSettings* patchSettings, float sampleRate, uint8_t vcoNumber)
    {
        this->patchSettings = patchSettings;
        this->vcoNumber = vcoNumber;
        isOn = true;
        waveform = 0;
        pulseWidth = 0.5f;
        level = 1.0F;
        fineTune = 0.0F;
        interval = 0.0F;
        octave = 0.0F;
        midiNote = 60;
    	osc.Init(sampleRate);
        osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
        osc.SetFreq(220.0F);
        osc.SetAmp(1.0F);
        wavefolder.Init();
        wavefolder.SetGain(1.0F);
    }

    void VCO::UpdateSettings()
    {
        switch (vcoNumber) {
            case 0:
                isOn = true;
                break;
            case 1:
                isOn = patchSettings->getBoolValue(PatchSetting::VCO_2_ON);
                break;
            case 2:
                isOn = patchSettings->getBoolValue(PatchSetting::VCO_3_ON);
                break;
        }
        if (isOn) {
            masterTune = patchSettings->getFloatValue(PatchSetting::VCO_MASTER_TUNE, -1.0f, 1.0f);
            switch (vcoNumber) {
                case 0:
                    waveform = patchSettings->getIntValue(PatchSetting::VCO_1_WAVEFORM);
                    pulseWidth = 0.53F - patchSettings->getFloatValue(PatchSetting::VCO_1_PULSE_WIDTH, 0.03F, 0.5F);
                    level = patchSettings->getFloatValue(PatchSetting::VCO_1_LEVEL);
                    // case 0 does not use fineTune, interval, or octave
                    fineTune = 0.0F;
                    interval = 0.0F;
                    octave = 0.0F;
                    break;
                case 1:
                    waveform = patchSettings->getIntValue(PatchSetting::VCO_2_WAVEFORM);
                    pulseWidth = 0.53F - patchSettings->getFloatValue(PatchSetting::VCO_2_PULSE_WIDTH, 0.03F, 0.5F);
                    level = patchSettings->getFloatValue(PatchSetting::VCO_2_LEVEL);
                    fineTune = patchSettings->getFloatValue(PatchSetting::VCO_2_FINE_TUNE, -1.0f, 1.0);
                    interval = (float)(patchSettings->getIntValue(PatchSetting::VCO_2_INTERVAL) - 11);
                    octave = (float)((patchSettings->getIntValue(PatchSetting::VCO_2_OCTAVE) - 2) * 12);
                    break;
                case 2:
                    waveform = patchSettings->getIntValue(PatchSetting::VCO_3_WAVEFORM);
                    pulseWidth = 0.53F - patchSettings->getFloatValue(PatchSetting::VCO_3_PULSE_WIDTH, 0.03F, 0.5F);
                    level = patchSettings->getFloatValue(PatchSetting::VCO_3_LEVEL);
                    fineTune = patchSettings->getFloatValue(PatchSetting::VCO_3_FINE_TUNE, -1.0f, 1.0);
                    interval = (float)(patchSettings->getIntValue(PatchSetting::VCO_3_INTERVAL) - 11);
                    octave = (float)((patchSettings->getIntValue(PatchSetting::VCO_3_OCTAVE) - 2) * 12);
                    break;
            }
            switch (waveform) {
                case 0:
                    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
                    break;
                case 1:
                    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
                    break;
                case 2:
                    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_TRI);
                    break;
            }
            if (pulseWidth > 0.495f) {
                waveFolderGain = 1.0f;
            } else {
                waveFolderGain = 1.0f + (0.495F - pulseWidth) * 55;
            }

            playingNote = midiNote + octave + interval + fineTune + masterTune;
        }
    }

    void VCO::Process(float* sample, float mod, float pwMod, bool fullFunctionality)
    {
        if (isOn) {
            float waveSample;
            osc.SetFreq(std::fmax(mtof(playingNote + mod * 12), 0.0f));

            if (fullFunctionality) {
                osc.SetPw(pulseWidth + pwMod);

                waveSample = osc.Process();
                if (waveform == 2 && fullFunctionality) { // Triangle
                    wavefolder.SetGain(std::fmax(waveFolderGain + pwMod * 27, 1.0F));
                    waveSample = wavefolder.Process(waveSample);
                } else if (waveform == 1) { // Sawtooth
                    waveSample = fclamp(waveSample * (waveFolderGain + 1.0f) / 2, -1.0F, 1.0F);
                }

                *sample = waveSample * level;
            } else {
                osc.SetPw(pulseWidth);
                *sample = osc.Process() * level;
            }
        }
        else {
            *sample = 0.0f;
        }
    }
}
