#include "VCO.h"

namespace kiwi_synth
{
    void VCO::Init(PatchSettings* patchSettings, float sampleRate, uint8_t vcoNumber)
    {
        this->patchSettings = patchSettings;
        this->vcoNumber = vcoNumber;
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
        masterTune = patchSettings->getFloatValue(PatchSetting::VCO_MASTER_TUNE, -0.08333333333f, 0.08333333333f, Scale::OCTAVE);
        lfo1Depth = patchSettings->getFloatValue(PatchSetting::LFO_1_TO_MASTER_TUNE, -0.001F, 1.0F, EXPONENTIAL);
        switch (vcoNumber) {
            case 0:
                // case 0 does not use fineTune
                level = patchSettings->getFloatValue(PatchSetting::VCO_1_LEVEL, -0.001F, 1.0F);
                fineTune = 1.0F;
                interval = 1.0F;
                octave = 1.0F;
                break;
            case 1:
                level = patchSettings->getFloatValue(PatchSetting::VCO_2_LEVEL, -0.001F, 1.0F);
                fineTune = patchSettings->getFloatValue(PatchSetting::VCO_2_FINE_TUNE, -0.08333333333f, 0.08333333333f, Scale::OCTAVE);
                interval = 1.0F;
                octave = 1.0F;
                break;
            case 2:
                level = patchSettings->getFloatValue(PatchSetting::VCO_3_LEVEL, -0.001F, 1.0F);
                fineTune = patchSettings->getFloatValue(PatchSetting::VCO_3_FINE_TUNE, -0.08333333333f, 0.08333333333f, Scale::OCTAVE);
                interval = 1.0F;
                octave = 1.0F;
                break;
        }

        CalculateFreq();
    }

    void VCO::Process(float* sample, float* mods, uint8_t numMods)
    {
        float computedFreq = freq;
        for (int i = 0; i < numMods; i++) {
            switch (i)
            {
                case 0:
                    if (lfo1Depth > 0.0F) {
                        computedFreq = std::max(std::min(computedFreq * (1.0F + mods[i] * 2.0F * lfo1Depth), VCO_MAX_FREQUENCY), VCO_MIN_FREQUENCY);
                    }
                    break;
                default:
                    computedFreq = std::max(std::min(computedFreq + (VCO_MAX_FREQUENCY - computedFreq) * mods[i], VCO_MAX_FREQUENCY), VCO_MIN_FREQUENCY);
                    break;
            }
        }
        osc.SetFreq(computedFreq);
        float computedLevel = level;
        if (computedLevel < 0) {
            computedLevel = 0.0F;
        }
        *sample = osc.Process() * computedLevel;
    }
}
