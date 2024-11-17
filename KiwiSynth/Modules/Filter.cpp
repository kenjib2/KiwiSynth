#include "Filter.h"



using namespace kiwisynth;



void Filter::Init(float sampleRate)
{
    filter_.Init(sampleRate);
    svFilter_.Init(sampleRate);
    opFilter_.Init();
    opFilter_.SetFilterMode(daisysp::OnePole::FilterMode::FILTER_MODE_LOW_PASS);
}



void Filter::UpdateSettings(PatchSettings* patchSettings)
{
    frequency_ = patchSettings->getFloatValueExponential(PatchSetting::FLT_CUTOFF, FLT_MIN_FREQUENCY, FLT_MAX_FREQUENCY);
    resonance_ = patchSettings->getFloatValue(PatchSetting::FLT_RESONANCE);

    filterType_ = (FilterType)patchSettings->getIntValue(PatchSetting::FLT_FILTER_TYPE);
}



void Filter::Process(float* sample, PatchSettings* patchSettings, float trackingMod, int currentMidiNote, float mod, float resMod)
{
    float computedFrequency = frequency_;
    computedFrequency = computedFrequency + trackingMod * mtof(currentMidiNote);

    computedFrequency = computedFrequency + (FLT_MAX_FREQUENCY - FLT_MIN_FREQUENCY - computedFrequency) * mod;

    float computedResonance = resonance_;
    float output;
    computedResonance = fclamp(resonance_ + resMod, 0.0f, 1.0f);
    switch (filterType_) {
        case LADDER_LOWPASS:
            filter_.SetFreq(computedFrequency);
            filter_.SetRes(computedResonance);
            output = filter_.Process(*sample);
            break;
        case SVF_LOWPASS:
            svFilter_.SetFreqAndRes(computedFrequency, computedResonance);
            svFilter_.Process(*sample);
            output = svFilter_.Low();
            break;
        case SVF_HIGHPASS:
            svFilter_.SetFreqAndRes(computedFrequency, computedResonance);
            svFilter_.Process(*sample);
            output = svFilter_.High();
            break;
        case SVF_BANDPASS:
            svFilter_.SetFreqAndRes(computedFrequency, computedResonance);
            svFilter_.Process(*sample);
            output = svFilter_.Band();
            break;
        case SVF_NOTCH:
            svFilter_.SetFreqAndRes(computedFrequency, computedResonance);
            svFilter_.Process(*sample);
            output = svFilter_.Notch();
            break;
        case SVF_PEAK:
            svFilter_.SetFreqAndRes(computedFrequency, computedResonance);
            svFilter_.Process(*sample);
            output = svFilter_.Peak();
            break;
        case ONE_POLE_LOWPASS:
            opFilter_.SetFilterMode(daisysp::OnePole::FilterMode::FILTER_MODE_LOW_PASS);
            opFilter_.SetFrequency(computedFrequency / 100000.0f);
            output = opFilter_.Process(*sample);
            break;
        case ONE_POLE_HIGHPASS:
            opFilter_.SetFilterMode(daisysp::OnePole::FilterMode::FILTER_MODE_HIGH_PASS);
            opFilter_.SetFrequency(computedFrequency / 100000.0f);
            output = opFilter_.Process(*sample);
            break;
        default:
            output = *sample;
            break;
    }

    *sample = output;
}
