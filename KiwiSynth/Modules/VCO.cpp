#include "VCO.h"

namespace kiwi_synth
{
    void VCO::Init(PatchSettings* patchSettings, float sampleRate, uint8_t vcoNumber)
    {
        this->patchSettings = patchSettings;
        this->vcoNumber = vcoNumber;
        isOn = true;
        pulseWidth = 0.5f;
        level = 1.0F;
        baseFreq = 220.0F;
        freq = 220.F;
        fineTune = 1.0F;
        interval = 1.0F;
        octave = 1.0F;
        midiNote = 60;
    	osc.Init(sampleRate);
        osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
        osc.SetFreq(220.0F);
    }

    void VCO::CalculateFreq()
    {
        float tuneHz = pow(2, masterTune * 2.0f - 1.0f) * fineTune * interval * octave;

        freq = baseFreq * tuneHz;
    }

    void VCO::SetFreq(float frequency)
    {
        baseFreq = frequency;
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
            masterTune = patchSettings->getFloatValue(PatchSetting::VCO_MASTER_TUNE, -0.08333333333f, 0.08333333333f, Scale::OCTAVE);
            lfo1Depth = patchSettings->getFloatValue(PatchSetting::LFO_1_TO_MASTER_TUNE, -0.001F, 1.0F, EXPONENTIAL);
            switch (vcoNumber) {
                case 0:
                    // case 0 does not use fineTune
                    pulseWidth = patchSettings->getFloatValue(PatchSetting::VCO_1_PULSE_WIDTH, 0.03F, 0.5F);
                    level = patchSettings->getFloatValue(PatchSetting::VCO_1_LEVEL, -0.001F, 1.0F);
                    fineTune = 1.0F;
                    interval = 1.0F;
                    octave = 1.0F;
                    break;
                case 1:
                    pulseWidth = patchSettings->getFloatValue(PatchSetting::VCO_2_PULSE_WIDTH, 0.03F, 0.5F);
                    level = patchSettings->getFloatValue(PatchSetting::VCO_2_LEVEL, -0.001F, 1.0F);
                    fineTune = patchSettings->getFloatValue(PatchSetting::VCO_2_FINE_TUNE, -0.08333333333f, 0.08333333333f, Scale::OCTAVE);
                    interval = patchSettings->getIntValue(PatchSetting::VCO_2_INTERVAL);
                    interval = 1.0f;
                    octave = PatchSettings::octaveToFrequencyMultiplier(patchSettings->getIntValue(PatchSetting::VCO_2_OCTAVE) - 2);
                    break;
                case 2:
                    pulseWidth = patchSettings->getFloatValue(PatchSetting::VCO_3_PULSE_WIDTH, 0.03F, 0.5F);
                    level = patchSettings->getFloatValue(PatchSetting::VCO_3_LEVEL, -0.001F, 1.0F);
                    fineTune = patchSettings->getFloatValue(PatchSetting::VCO_3_FINE_TUNE, -0.08333333333f, 0.08333333333f, Scale::OCTAVE);
                    interval = patchSettings->getIntValue(PatchSetting::VCO_3_INTERVAL);
                    interval = 1.0f;
                    octave = PatchSettings::octaveToFrequencyMultiplier(patchSettings->getIntValue(PatchSetting::VCO_3_OCTAVE) - 2);
                    break;
            }

            CalculateFreq();
        }
    }

    void VCO::Process(float* sample, float* mods, uint8_t numMods)
    {
        if (isOn) {
            float computedFreq = freq;
            for (int i = 0; i < numMods; i++) {
                switch (i)
                {
                    case 0:
                        if (lfo1Depth > 0.0F) {
                            computedFreq = std::fmax(std::fmin(computedFreq * (1.0F + mods[i] * 2.0F * lfo1Depth), VCO_MAX_FREQUENCY), VCO_MIN_FREQUENCY);
                        }
                        break;
                    default:
                        computedFreq = std::fmax(std::fmin(computedFreq + (VCO_MAX_FREQUENCY - computedFreq) * mods[i], VCO_MAX_FREQUENCY), VCO_MIN_FREQUENCY);
                        break;
                }
            }
            osc.SetPw(pulseWidth);
            osc.SetFreq(computedFreq);
            float computedLevel = level;
            if (computedLevel < 0) {
                computedLevel = 0.0F;
            }
            *sample = osc.Process() * computedLevel;
        }
        else {
            *sample = 0.0f;
        }
    }
}
