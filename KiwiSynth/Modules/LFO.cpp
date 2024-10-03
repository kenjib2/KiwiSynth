#include "LFO.h"

namespace kiwi_synth
{
    void LFO::Init(float sampleRate, uint8_t lfoNumber)
    {
        this->lfoNumber = lfoNumber;
        noteOnReset = false;
        pulseWidth = 0.5f;
    	squareOsc.Init(sampleRate);
        squareOsc.SetAmp(1.0f);
    	sawOsc.Init(sampleRate);
        sawOsc.SetAmp(1.0f);
    	rampOsc.Init(sampleRate);
        rampOsc.SetAmp(1.0f);
    	triangleOsc.Init(sampleRate);
        triangleOsc.SetAmp(1.0f);
        wavefolder.Init();
        wavefolder.SetGain(1.0f);
    }

    void LFO::UpdateSettings(PatchSettings* patchSettings)
    {
        waveform = patchSettings->getIntValue((PatchSetting)(LFO_1_WAVEFORM + lfoNumber));
        freq = patchSettings->getFloatValueLogLookup((PatchSetting)(LFO_1_RATE + lfoNumber));
        basePhase = patchSettings->getFloatValue((PatchSetting)(PatchSetting::LFO_1_TRIGGER_PHASE + lfoNumber));
        noteOnReset = patchSettings->getBoolValue((PatchSetting)(PatchSetting::LFO_1_TRIGGER_RESET_ON + lfoNumber));
        pulseWidth = patchSettings->getFloatValueLinear((PatchSetting)(PatchSetting::LFO_1_PULSE_WIDTH + lfoNumber), 0.005f, 0.995f);
        if (pulseWidth < 0.032f) {
            sawtoothGain = waveFolderGain = 1.0f;
        } else {
            waveFolderGain = 0.84f + pulseWidth * 5.0f;
            sawtoothGain = 0.999606784f + pulseWidth * pulseWidth * pulseWidth * 12.0f;
        }
    }

    void LFO::Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float tphaseMod)
    {
        // WAVE_TRI = 0, WAVE_SAW = 1, WAVE_RAMP = 2, WAVE_SQUARE = 3
        phase = fmod(basePhase + tphaseMod, 1.0f); // This is used for note retriggers

        float waveSample;
        switch (waveform) {
            case LFO_WAVEFORM_TRIANGLE:
                triangleOsc.SetFreq(freq * (1.0f + mod * 10.0f));
                waveSample = triangleOsc.Process();
                wavefolder.SetGain(std::fmax(waveFolderGain + pwMod * 5.0f, 1.0f));
                waveSample = wavefolder.Process(waveSample);
                break;
            case LFO_WAVEFORM_SAWTOOTH:
                sawOsc.SetFreq(freq * (1.0f + mod * 10.0f));
                waveSample = sawOsc.Process();
                waveSample = fclamp(waveSample * sawtoothGain, -1.0f, 1.0f);
                break;
            case LFO_WAVEFORM_RAMP:
                rampOsc.SetFreq(freq * (1.0f + mod * 10.0f));
                waveSample = rampOsc.Process();
                waveSample = fclamp(waveSample * sawtoothGain, -1.0f, 1.0f);
                break;
            case LFO_WAVEFORM_SQUARE:
            default:
                squareOsc.SetPw(pulseWidth + pwMod);
                squareOsc.SetFreq(freq * (1.0f + mod * 10.0f));
                waveSample = squareOsc.Process();
                break;
        }

        *sample = waveSample;
    }

}
