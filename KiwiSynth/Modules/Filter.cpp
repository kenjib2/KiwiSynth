#include "Filter.h"

namespace kiwisynth
{

    void Filter::Init(float sampleRate)
    {
        filter.Init(sampleRate);
        svFilter.Init(sampleRate);
        opFilter.Init();
        opFilter.SetFilterMode(daisysp::OnePole::FilterMode::FILTER_MODE_LOW_PASS);
    }

    void Filter::UpdateSettings(PatchSettings* patchSettings)
    {
        frequency = patchSettings->getFloatValueExponential(PatchSetting::FLT_CUTOFF, FLT_MIN_FREQUENCY, FLT_MAX_FREQUENCY);
        resonance = patchSettings->getFloatValue(PatchSetting::FLT_RESONANCE);

        filterType = (FilterType)patchSettings->getIntValue(PatchSetting::FLT_FILTER_TYPE);
    }

    void Filter::Process(float* sample, PatchSettings* patchSettings, float trackingMod, int currentMidiNote, float mod, float resMod)
    {
        float computedFrequency = frequency;
        computedFrequency = computedFrequency + trackingMod * mtof(currentMidiNote);

        computedFrequency = computedFrequency + (FLT_MAX_FREQUENCY - FLT_MIN_FREQUENCY - computedFrequency) * mod;

        float computedResonance = resonance;
        float output;
        computedResonance = fclamp(resonance + resMod, 0.0f, 1.0f);
        switch (filterType) {
            case LADDER_LOWPASS:
                filter.SetFreq(computedFrequency);
                filter.SetRes(computedResonance);
                output = filter.Process(*sample);
                break;
            case SVF_LOWPASS:
                svFilter.SetFreqAndRes(computedFrequency, computedResonance);
                svFilter.Process(*sample);
                output = svFilter.Low();
                break;
            case SVF_HIGHPASS:
                svFilter.SetFreqAndRes(computedFrequency, computedResonance);
                svFilter.Process(*sample);
                output = svFilter.High();
                break;
            case SVF_BANDPASS:
                svFilter.SetFreqAndRes(computedFrequency, computedResonance);
                svFilter.Process(*sample);
                output = svFilter.Band();
                break;
            case SVF_NOTCH:
                svFilter.SetFreqAndRes(computedFrequency, computedResonance);
                svFilter.Process(*sample);
                output = svFilter.Notch();
                break;
            case SVF_PEAK:
                svFilter.SetFreqAndRes(computedFrequency, computedResonance);
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

} // namespace kiwisynth
