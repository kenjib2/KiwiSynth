#include "Oscillator.h"



namespace kiwisynth
{



void Oscillator::Init(float sampleRate, uint8_t oscNumber)
{
    oscNumber_ = oscNumber;
    isOn_ = true;
    waveform_ = 0;
    pulseWidth_ = 0.5f;
    variShape_ = 0.5f;
    level_ = 1.0f;
    fineTune_ = 0.0f;
    interval_ = 0.0f;
    octave_ = 0.0f;
    midiNote_ = 60;
    squareOsc_.Init(sampleRate);
    squareOsc_.SetFreq(220.0f);
    squareOsc_.SetAmp(1.0f);
    sawOsc_.Init(sampleRate);
    sawOsc_.SetFreq(220.0f);
    sawOsc_.SetAmp(1.0f);
    triangleOsc_.Init(sampleRate);
    triangleOsc_.SetFreq(220.0f);
    triangleOsc_.SetAmp(1.0f);
    variOsc_.Init(sampleRate);
    variOsc_.SetWaveshape(0.0f);
    variOsc_.SetSync(false);
    variSaw_.Init(sampleRate);
    variSaw_.SetWaveshape(0.0f);
    sineOsc_.Init(sampleRate);
    sineOsc_.SetFreq(220.0f);
    sineOsc_.SetAmp(1.0f);
    wavefolder_.Init();
    wavefolder_.SetGain(1.0f);
}



void Oscillator::UpdateSettings(PatchSettings* patchSettings)
{
    isOn_ = patchSettings->getBoolValue((PatchSetting)(OSC_1_ON + oscNumber_));
    if (isOn_) {
        waveform_ = patchSettings->getIntValue((PatchSetting)(OSC_1_WAVEFORM + oscNumber_));
        pulseWidth_ = 0.53f - patchSettings->getFloatValueLinear((PatchSetting)(OSC_1_PULSE_WIDTH + oscNumber_), 0.03f, 0.5f);
        variShape_ = patchSettings->getFloatValue((PatchSetting)(OSC_1_PULSE_WIDTH + oscNumber_));
        variShape_ = variShape_ > 0.003 ? variShape_ - 0.003 : 0.0f;
        waveFolderGain_ = 1.0f + variShape_ * 27.0f;
        sawtoothGain_ = variShape_ * variShape_ * 50.0f;
        level_ = patchSettings->getFloatValue((PatchSetting)(OSC_1_LEVEL + oscNumber_));
        if (oscNumber_ > 0) {
            int oscMod = oscNumber_ - 1;
            level_ = patchSettings->getFloatValue((PatchSetting)(OSC_2_LEVEL + oscMod));
            fineTune_ = patchSettings->getFloatValueLinear((PatchSetting)(OSC_2_FINE_TUNE + oscMod), -1.0f, 1.0);
            interval_ = (float)(patchSettings->getIntValue((PatchSetting)(OSC_2_INTERVAL + oscMod)) - 11);
            octave_ = (float)((patchSettings->getIntValue((PatchSetting)(OSC_2_OCTAVE + oscMod)) - 2) * 12);
        }
    }
}



void Oscillator::Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float ampMod)
{
    if (__builtin_expect(isOn_, 1)) {
        float masterTune = patchSettings->getFloatValueLinear(PatchSetting::OSC_MASTER_TUNE, -1.0f, 1.0f);
        playingNote_ = midiNote_ + octave_ + interval_ + fineTune_ + masterTune + paraOffset_;
        float waveSample = 0.0f;
        float freq = std::fmax(mtof(playingNote_ + mod * 12.0f), 0.0f);
        switch(waveform_) {
            case WAVEFORM_SQUARE:
                squareOsc_.SetFreq(freq);
                squareOsc_.SetPw(pulseWidth_ + pwMod);
                waveSample = squareOsc_.Process();
                isEoc_ = squareOsc_.IsEOC();
                break;
            case WAVEFORM_SAWTOOTH:
                sawOsc_.SetFreq(freq);
                waveSample = sawOsc_.Process();
                waveSample = fclamp(waveSample * (sawtoothGain_ + pwMod * 50.0f + 1.0f), -1.0f, 1.0f);
                isEoc_ = sawOsc_.IsEOC();
                break;
            case WAVEFORM_TRIANGLE:
                triangleOsc_.SetFreq(freq);
                waveSample = triangleOsc_.Process();
                wavefolder_.SetGain(std::fmax(waveFolderGain_ + pwMod * 27.0f, 1.0f));
                waveSample = fclamp(wavefolder_.Process(waveSample), -1.0f, 1.0f);
                isEoc_ = triangleOsc_.IsEOC();
                break;
            case WAVEFORM_VARISHAPE:
                variOsc_.SetSyncFreq(freq);
                variOsc_.SetPW((variShape_ + pwMod) * 0.25f);
                waveSample = variOsc_.Process();
                break;
            case WAVEFORM_VARISAW:
                variSaw_.SetFreq(freq);
                variSaw_.SetPW(1.0f - variShape_ + pwMod);
                waveSample = variSaw_.Process();
                break;
            case WAVEFORM_SINE:
                sineOsc_.SetFreq(freq);
                waveSample = sineOsc_.Process();
                wavefolder_.SetGain(std::fmax(waveFolderGain_ + pwMod * 27.0f, 1.0f));
                waveSample = fclamp(wavefolder_.Process(waveSample), -1.0f, 1.0f);
                isEoc_ = sineOsc_.IsEOC();
                break;
            case WAVEFORM_WAVEFOLDED_SAWTOOTH:
                sawOsc_.SetFreq(freq);
                waveSample = sawOsc_.Process();
                wavefolder_.SetGain(std::fmax(waveFolderGain_ + pwMod * 27.0f, 1.0f));
                waveSample = fclamp(wavefolder_.Process(waveSample), -1.0f, 1.0f);
                isEoc_ = sawOsc_.IsEOC();
        }

        *sample = waveSample * std::fmax(level_ + ampMod, 0.0f);
    }
    else {
        *sample = 0.0f;
    }
}



void Oscillator::Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float ampMod, float phaseMod)
{
    if (__builtin_expect(isOn_, 1)) {
        float masterTune = patchSettings->getFloatValueLinear(PatchSetting::OSC_MASTER_TUNE, -1.0f, 1.0f);
        playingNote_ = midiNote_ + octave_ + interval_ + fineTune_ + masterTune + paraOffset_;
        float waveSample = 0.0f;
        float freq = std::fmax(mtof(playingNote_ + mod * 12.0f), 0.0f);
        switch(waveform_) {
            case WAVEFORM_SQUARE:
                squareOsc_.SetFreq(freq);
                squareOsc_.SetPw(pulseWidth_ + pwMod);
                waveSample = squareOsc_.Process(phaseMod);
                isEoc_ = squareOsc_.IsEOC();
                break;
            case WAVEFORM_SAWTOOTH:
                sawOsc_.SetFreq(freq);
                waveSample = sawOsc_.Process(phaseMod);
                waveSample = fclamp(waveSample * (sawtoothGain_ + pwMod * 50.0f + 1.0f), -1.0f, 1.0f);
                isEoc_ = sawOsc_.IsEOC();
                break;
            case WAVEFORM_TRIANGLE:
                triangleOsc_.SetFreq(freq);
                waveSample = triangleOsc_.Process(phaseMod);
                wavefolder_.SetGain(std::fmax(waveFolderGain_ + pwMod * 27.0f, 1.0f));
                waveSample = fclamp(wavefolder_.Process(waveSample), -1.0f, 1.0f);
                isEoc_ = triangleOsc_.IsEOC();
                break;
            case WAVEFORM_VARISHAPE:
                variOsc_.SetSyncFreq(freq);
                variOsc_.SetPW((variShape_ + pwMod) * 0.25f);
                waveSample = variOsc_.Process();
                break;
            case WAVEFORM_VARISAW:
                variSaw_.SetFreq(freq);
                variSaw_.SetPW(1.0f - variShape_ + pwMod);
                waveSample = variSaw_.Process();
                break;
            case WAVEFORM_SINE:
                sineOsc_.SetFreq(freq);
                waveSample = sineOsc_.Process(phaseMod);
                wavefolder_.SetGain(std::fmax(waveFolderGain_ + pwMod * 27.0f, 1.0f));
                waveSample = fclamp(wavefolder_.Process(waveSample), -1.0f, 1.0f);
                isEoc_ = sineOsc_.IsEOC();
                break;
            case WAVEFORM_WAVEFOLDED_SAWTOOTH:
                sawOsc_.SetFreq(freq);
                waveSample = sawOsc_.Process(phaseMod);
                wavefolder_.SetGain(std::fmax(waveFolderGain_ + pwMod * 27.0f, 1.0f));
                waveSample = fclamp(wavefolder_.Process(waveSample), -1.0f, 1.0f);
                isEoc_ = sawOsc_.IsEOC();
        }

        *sample = waveSample * std::fmax(level_ + ampMod, 0.0f);
    }
    else {
        *sample = 0.0f;
    }
}



float Oscillator::GetPhaseRatio()
{
    switch(waveform_) {
        case WAVEFORM_SQUARE:
            return squareOsc_.GetPhaseRatio();
        case WAVEFORM_SAWTOOTH:
            return sawOsc_.GetPhaseRatio();
        case WAVEFORM_TRIANGLE:
            return triangleOsc_.GetPhaseRatio();
        case WAVEFORM_SINE:
            return sineOsc_.GetPhaseRatio();
        case WAVEFORM_WAVEFOLDED_SAWTOOTH:
            return sawOsc_.GetPhaseRatio();
        default:
        case WAVEFORM_VARISHAPE:
        case WAVEFORM_VARISAW:
            return 0.0f;
    }
}



void Oscillator::HardSync(float phaseRatio)
{
    switch(waveform_) {
        case WAVEFORM_SQUARE:
            squareOsc_.HardSync(phaseRatio);
            break;
        case WAVEFORM_SAWTOOTH:
            sawOsc_.HardSync(phaseRatio);
            break;
        case WAVEFORM_TRIANGLE:
            triangleOsc_.HardSync(phaseRatio);
            break;
        case WAVEFORM_SINE:
            sineOsc_.HardSync(phaseRatio);
            break;
        case WAVEFORM_WAVEFOLDED_SAWTOOTH:
            sawOsc_.HardSync(phaseRatio);
            break;
        case WAVEFORM_VARISHAPE:
        case WAVEFORM_VARISAW:
            break;
    }
}



} // namespace kiwisynth
