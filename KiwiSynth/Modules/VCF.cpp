#include "VCF.h"

namespace kiwi_synth
{
    void VCF::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        filter.Init(sampleRate);
        svFilter.Init(sampleRate);
        opFilter.Init();
        opFilter.SetFilterMode(daisysp::OnePole::FilterMode::FILTER_MODE_LOW_PASS);
    }

    void VCF::UpdateSettings()
    {
        frequency = patchSettings->getFloatValue(PatchSetting::VCF_CUTOFF, VCF_MIN_FREQUENCY, VCF_MAX_FREQUENCY, kiwi_synth::LOGARHITHMIC);
        resonance = patchSettings->getFloatValue(PatchSetting::VCF_RESONANCE);
        keyboardTracking = patchSettings->getFloatValue(PatchSetting::VCF_TRACKING);
        env1Depth = patchSettings->getFloatValue(PatchSetting::VCF_ENV_1_DEPTH);
        env2Depth = patchSettings->getFloatValue(PatchSetting::VCF_ENV_2_DEPTH);
        lfo2Depth = patchSettings->getFloatValue(PatchSetting::LFO_2_TO_VCF_CUTOFF, -0.001F, 1.0F);

        switch (patchSettings->getIntValue(PatchSetting::VCF_FILTER_TYPE))
        {
            case 0:
            default:
                filterType = LADDER_LOWPASS;
                break;
            case 1:
                filterType = SVF_LOWPASS;
                break;
            case 2:
                filterType = SVF_HIGHPASS;
                break;
            case 3:
                filterType = SVF_BANDPASS;
                break;
            case 4:
                filterType = SVF_NOTCH;
                break;
            case 5:
                filterType = SVF_PEAK;
                break;
            case 6:
                filterType = ONE_POLE_LOWPASS;
                break;
            case 7:
                filterType = ONE_POLE_HIGHPASS;
                break;
        }
    }

    void VCF::Process(float* sample, float trackingMod, int currentMidiNote, float mod)
    {
        float computedFrequency = frequency;
        computedFrequency = std::fmin(computedFrequency + trackingMod * mtof(currentMidiNote), VCF_MAX_FREQUENCY) ;

        if (mod != 0.0f) {
            // avoiding extra calculations for fullFunctionality = false
            computedFrequency = std::fmax(std::fmin(computedFrequency + (VCF_MAX_FREQUENCY - VCF_MIN_FREQUENCY - computedFrequency) * mod, VCF_MAX_FREQUENCY), VCF_MIN_FREQUENCY);
        }

        float output;
        switch (filterType) {
            case LADDER_LOWPASS:
                filter.SetFreq(computedFrequency);
                filter.SetRes(resonance);
                output = filter.Process(*sample);
                break;
            case SVF_LOWPASS:
                svFilter.SetFreq(computedFrequency);
                svFilter.SetRes(resonance);
                svFilter.Process(*sample);
                output = svFilter.Low();
                break;
            case SVF_HIGHPASS:
                svFilter.SetFreq(computedFrequency);
                svFilter.SetRes(resonance);
                svFilter.Process(*sample);
                output = svFilter.High();
                break;
            case SVF_BANDPASS:
                svFilter.SetFreq(computedFrequency);
                svFilter.SetRes(resonance);
                svFilter.Process(*sample);
                output = svFilter.Band();
                break;
            case SVF_NOTCH:
                svFilter.SetFreq(computedFrequency);
                svFilter.SetRes(resonance);
                svFilter.Process(*sample);
                output = svFilter.Notch();
                break;
            case SVF_PEAK:
                svFilter.SetFreq(computedFrequency);
                svFilter.SetRes(resonance);
                svFilter.Process(*sample);
                output = svFilter.Peak();
                break;
            case ONE_POLE_LOWPASS:
                opFilter.SetFilterMode(daisysp::OnePole::FilterMode::FILTER_MODE_LOW_PASS);
                opFilter.SetFrequency(computedFrequency / 100000.0f);
                output = opFilter.Process(*sample);
                break;
            case ONE_POLE_HIGHPASS:
                opFilter.SetFilterMode(daisysp::OnePole::FilterMode::FILTER_MODE_HIGH_PASS);
                opFilter.SetFrequency(computedFrequency / 100000.0f);
                output = opFilter.Process(*sample);
                break;
            default:
                output = *sample;
                break;
        }
        *sample = output;
    }
}
