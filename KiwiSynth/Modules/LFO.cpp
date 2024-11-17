#include "LFO.h"



using namespace kiwisynth;



void LFO::Init(float sampleRate, uint8_t lfoNumber)
{
    lfoNumber_ = lfoNumber;
    isNoteOnReset_ = false;
    pulseWidth_ = 0.5f;
    squareOsc_.Init(sampleRate);
    squareOsc_.SetAmp(1.0f);
    sawOsc_.Init(sampleRate);
    sawOsc_.SetAmp(1.0f);
    rampOsc_.Init(sampleRate);
    rampOsc_.SetAmp(1.0f);
    triangleOsc_.Init(sampleRate);
    triangleOsc_.SetAmp(1.0f);
    variOsc_.Init(sampleRate);
    variOsc_.SetWaveshape(0.0f);
    variOsc_.SetSync(false);
    variSaw_.Init(sampleRate);
    variSaw_.SetWaveshape(0.0f);
    sineOsc_.Init(sampleRate);
    sineOsc_.SetAmp(1.0f);
    wavefolder_.Init();
    wavefolder_.SetGain(1.0f);
}



void LFO::UpdateSettings(PatchSettings* patchSettings)
{
    waveform_ = patchSettings->getIntValue((PatchSetting)(LFO_1_WAVEFORM + lfoNumber_));
    freq_ = patchSettings->getFloatValueLogLookup((PatchSetting)(LFO_1_RATE + lfoNumber_));
    basePhase_ = patchSettings->getFloatValue((PatchSetting)(PatchSetting::LFO_1_TRIGGER_PHASE + lfoNumber_));
    isNoteOnReset_ = patchSettings->getBoolValue((PatchSetting)(PatchSetting::LFO_1_TRIGGER_RESET_ON + lfoNumber_));
    pulseWidth_ = patchSettings->getFloatValueLinear((PatchSetting)(PatchSetting::LFO_1_PULSE_WIDTH + lfoNumber_), 0.005f, 0.995f);
    variShape_ = patchSettings->getFloatValue((PatchSetting)(LFO_1_PULSE_WIDTH + lfoNumber_));
    variShape_ = variShape_ > 0.003 ? variShape_ - 0.003 : 0.0f;
    if (pulseWidth_ < 0.032f) {
        sawtoothGain_ = waveFolderGain_ = 1.0f;
    } else {
        waveFolderGain_ = 0.84f + pulseWidth_ * 5.0f;
        sawtoothGain_ = 0.999606784f + pulseWidth_ * pulseWidth_ * pulseWidth_ * 12.0f;
    }
}



void LFO::Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float tphaseMod)
{
    phase_ = fmod(basePhase_ + tphaseMod, 1.0f); // This is used for note retriggers

    float waveSample;
    switch (waveform_) {
        case LFO_WAVEFORM_TRIANGLE:
        default:
            triangleOsc_.SetFreq(freq_ * (1.0f + mod * 10.0f));
            waveSample = triangleOsc_.Process();
            wavefolder_.SetGain(std::fmax(waveFolderGain_ + pwMod * 5.0f, 1.0f));
            waveSample = wavefolder_.Process(waveSample);
            break;
        case LFO_WAVEFORM_SAWTOOTH:
            sawOsc_.SetFreq(freq_ * (1.0f + mod * 10.0f));
            waveSample = sawOsc_.Process();
            waveSample = fclamp(waveSample * sawtoothGain_, -1.0f, 1.0f);
            break;
        case LFO_WAVEFORM_RAMP:
            rampOsc_.SetFreq(freq_ * (1.0f + mod * 10.0f));
            waveSample = rampOsc_.Process();
            waveSample = fclamp(waveSample * sawtoothGain_, -1.0f, 1.0f);
            break;
        case LFO_WAVEFORM_SQUARE:
            squareOsc_.SetPw(pulseWidth_ + pwMod);
            squareOsc_.SetFreq(freq_ * (1.0f + mod * 10.0f));
            waveSample = squareOsc_.Process();
            break;
        case LFO_WAVEFORM_VARISHAPE:
            variOsc_.SetSyncFreq(freq_);
            variOsc_.SetPW((variShape_ + pwMod) * 0.25f);
            waveSample = variOsc_.Process();
            break;
        case LFO_WAVEFORM_VARISAW:
            variSaw_.SetFreq(freq_);
            variSaw_.SetPW(1.0f - variShape_ + pwMod);
            waveSample = variSaw_.Process();
            break;
        case LFO_WAVEFORM_SINE:
            sineOsc_.SetFreq(freq_);
            waveSample = sineOsc_.Process();
            wavefolder_.SetGain(std::fmax(waveFolderGain_ + pwMod * 27.0f, 1.0f));
            waveSample = fclamp(wavefolder_.Process(waveSample), -1.0f, 1.0f);
            break;
        case LFO_WAVEFORM_WAVEFOLDED_SAWTOOTH:
            sawOsc_.SetFreq(freq_);
            waveSample = sawOsc_.Process();
            wavefolder_.SetGain(std::fmax(waveFolderGain_ + pwMod * 27.0f, 1.0f));
            waveSample = fclamp(wavefolder_.Process(waveSample), -1.0f, 1.0f);
            break;
    }

    *sample = waveSample;
}
