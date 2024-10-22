#include "Oscillator.h"

namespace kiwi_synth
{
    void Oscillator::Init(float sampleRate, uint8_t oscNumber)
    {
        this->oscNumber = oscNumber;
        isOn = true;
        waveform = 0;
        pulseWidth = 0.5f;
        variShape = 0.5f;
        level = 1.0f;
        fineTune = 0.0f;
        interval = 0.0f;
        octave = 0.0f;
        midiNote = 60;
        squareOsc.Init(sampleRate);
        squareOsc.SetFreq(220.0f);
        squareOsc.SetAmp(1.0f);
        sawOsc.Init(sampleRate);
        sawOsc.SetFreq(220.0f);
        sawOsc.SetAmp(1.0f);
        triangleOsc.Init(sampleRate);
        triangleOsc.SetFreq(220.0f);
        triangleOsc.SetAmp(1.0f);
        variOsc.Init(sampleRate);
        variOsc.SetWaveshape(0.0f);
        variOsc.SetSync(false);
        variSaw.Init(sampleRate);
        variSaw.SetWaveshape(0.0f);
        sineOsc.Init(sampleRate);
        sineOsc.SetFreq(220.0f);
        sineOsc.SetAmp(1.0f);
        wavefolder.Init();
        wavefolder.SetGain(1.0f);
    }

    void Oscillator::UpdateSettings(PatchSettings* patchSettings)
    {
        isOn = patchSettings->getBoolValue((PatchSetting)(OSC_1_ON + oscNumber));
        if (isOn) {
            waveform = patchSettings->getIntValue((PatchSetting)(OSC_1_WAVEFORM + oscNumber));
            pulseWidth = 0.53f - patchSettings->getFloatValueLinear((PatchSetting)(OSC_1_PULSE_WIDTH + oscNumber), 0.03f, 0.5f);
            variShape = patchSettings->getFloatValue((PatchSetting)(OSC_1_PULSE_WIDTH + oscNumber));
            variShape = variShape > 0.003 ? variShape - 0.003 : 0.0f;
            waveFolderGain = 1.0f + variShape * 27.0f;
            sawtoothGain = variShape * variShape * 50.0f;
            level = patchSettings->getFloatValue((PatchSetting)(OSC_1_LEVEL + oscNumber));
            if (oscNumber > 0) {
                int oscMod = oscNumber - 1;
                level = patchSettings->getFloatValue((PatchSetting)(OSC_2_LEVEL + oscMod));
                fineTune = patchSettings->getFloatValueLinear((PatchSetting)(OSC_2_FINE_TUNE + oscMod), -1.0f, 1.0);
                interval = (float)(patchSettings->getIntValue((PatchSetting)(OSC_2_INTERVAL + oscMod)) - 11);
                octave = (float)((patchSettings->getIntValue((PatchSetting)(OSC_2_OCTAVE + oscMod)) - 2) * 12);
            }
        }
    }

    void Oscillator::Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float ampMod)
    {
        if (__builtin_expect(isOn, 1)) {
            float masterTune = patchSettings->getFloatValueLinear(PatchSetting::OSC_MASTER_TUNE, -1.0f, 1.0f);
            playingNote = midiNote + octave + interval + fineTune + masterTune + paraOffset;
            float waveSample = 0.0f;
            float freq = std::fmax(mtof(playingNote + mod * 12.0f), 0.0f);
            switch(waveform) {
                case WAVEFORM_SQUARE:
                    squareOsc.SetFreq(freq);
                    squareOsc.SetPw(pulseWidth + pwMod);
                    waveSample = squareOsc.Process();
                    eoc = squareOsc.IsEOC();
                    break;
                case WAVEFORM_SAWTOOTH:
                    sawOsc.SetFreq(freq);
                    waveSample = sawOsc.Process();
                    waveSample = fclamp(waveSample * (sawtoothGain + pwMod * 50.0f + 1.0f), -1.0f, 1.0f);
                    eoc = sawOsc.IsEOC();
                    break;
                case WAVEFORM_TRIANGLE:
                    triangleOsc.SetFreq(freq);
                    waveSample = triangleOsc.Process();
                    wavefolder.SetGain(std::fmax(waveFolderGain + pwMod * 27.0f, 1.0f));
                    waveSample = fclamp(wavefolder.Process(waveSample), -1.0f, 1.0f);
                    eoc = triangleOsc.IsEOC();
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
                case WAVEFORM_SINE:
                    sineOsc.SetFreq(freq);
                    waveSample = sineOsc.Process();
                    wavefolder.SetGain(std::fmax(waveFolderGain + pwMod * 27.0f, 1.0f));
                    waveSample = fclamp(wavefolder.Process(waveSample), -1.0f, 1.0f);
                    eoc = sineOsc.IsEOC();
                    break;
                case WAVEFORM_WAVEFOLDED_SAWTOOTH:
                    sawOsc.SetFreq(freq);
                    waveSample = sawOsc.Process();
                    wavefolder.SetGain(std::fmax(waveFolderGain + pwMod * 27.0f, 1.0f));
                    waveSample = fclamp(wavefolder.Process(waveSample), -1.0f, 1.0f);
                    eoc = sawOsc.IsEOC();
            }

            *sample = waveSample * std::fmax(level + ampMod, 0.0f);
        }
        else {
            *sample = 0.0f;
        }
    }

    float Oscillator::GetPhaseRatio()
    {
        switch(waveform) {
            case WAVEFORM_SQUARE:
                return squareOsc.GetPhaseRatio();
            case WAVEFORM_SAWTOOTH:
                return sawOsc.GetPhaseRatio();
            case WAVEFORM_TRIANGLE:
                return triangleOsc.GetPhaseRatio();
            case WAVEFORM_SINE:
                return sineOsc.GetPhaseRatio();
            case WAVEFORM_WAVEFOLDED_SAWTOOTH:
                return sawOsc.GetPhaseRatio();
            default:
            case WAVEFORM_VARISHAPE:
            case WAVEFORM_VARISAW:
                return 0.0f;
        }
    }

    void Oscillator::HardSync(float phaseRatio)
    {
        switch(waveform) {
            case WAVEFORM_SQUARE:
                squareOsc.HardSync(phaseRatio);
                break;
            case WAVEFORM_SAWTOOTH:
                sawOsc.HardSync(phaseRatio);
                break;
            case WAVEFORM_TRIANGLE:
                triangleOsc.HardSync(phaseRatio);
                break;
            case WAVEFORM_SINE:
                sineOsc.HardSync(phaseRatio);
                break;
            case WAVEFORM_WAVEFOLDED_SAWTOOTH:
                sawOsc.HardSync(phaseRatio);
                break;
            case WAVEFORM_VARISHAPE:
            case WAVEFORM_VARISAW:
                break;
        }
    }

    void Oscillator::PhaseAdd(float phase)
    {
        squareOsc.PhaseAdd(phase);
        sawOsc.PhaseAdd(phase);
        triangleOsc.PhaseAdd(phase);
        sineOsc.PhaseAdd(phase);
    }

    void Oscillator::PhaseReset()
    {
        squareOsc.PhaseReset();
        sawOsc.PhaseReset();
        triangleOsc.PhaseReset();
        sineOsc.PhaseReset();
    }

}
