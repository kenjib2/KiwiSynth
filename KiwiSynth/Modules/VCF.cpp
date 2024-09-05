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

    void VCF::Process(float* sample, float trackingMod, int currentMidiNote, float mod, float resMod, bool fullFunctionality, bool mono)
    {
        float computedFrequency = frequency;
        computedFrequency = computedFrequency + trackingMod * mtof(currentMidiNote);

        computedFrequency = computedFrequency + (VCF_MAX_FREQUENCY - VCF_MIN_FREQUENCY - computedFrequency) * mod / 4.0f;

        float computedResonance = resonance;
        float output;
        computedResonance = fclamp(resonance + resMod, 0.0f, 1.0f);
        /*if (fullFunctionality && mono) {
            switch (filterType) {
                case LADDER_LOWPASS:
                    filter.SetFreq(computedFrequency);
                    filter.SetRes(computedResonance);
                    output = filter.Process(*sample);
                    break;
                case SVF_LOWPASS:
                    svFilter.SetFreq(computedFrequency);
                    svFilter.SetRes(computedResonance);
                    svFilter.Process(*sample);
                    output = svFilter.Low();
                    break;
                case SVF_HIGHPASS:
                    svFilter.SetFreq(computedFrequency);
                    svFilter.SetRes(computedResonance);
                    svFilter.Process(*sample);
                    output = svFilter.High();
                    break;
                case SVF_BANDPASS:
                    svFilter.SetFreq(computedFrequency);
                    svFilter.SetRes(computedResonance);
                    svFilter.Process(*sample);
                    output = svFilter.Band();
                    break;
                case SVF_NOTCH:
                    svFilter.SetFreq(computedFrequency);
                    svFilter.SetRes(computedResonance);
                    svFilter.Process(*sample);
                    output = svFilter.Notch();
                    break;
                case SVF_PEAK:
                    svFilter.SetFreq(computedFrequency);
                    svFilter.SetRes(computedResonance);
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
        } else {*/
            filter.SetFreq(computedFrequency);
            filter.SetRes(computedResonance);
            output = filter.Process(*sample);
        //}

        *sample = output;
    }
}
