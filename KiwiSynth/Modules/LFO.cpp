#include "LFO.h"

namespace kiwi_synth
{
    void LFO::Init(float sampleRate, uint8_t lfoNumber)
    {
        this->lfoNumber = lfoNumber;
        noteOnReset = false;
        pulseWidth = 0.5f;
    	osc.Init(sampleRate);
        osc.SetWaveform(Oscillator::WAVE_TRI);
        osc.SetFreq(2.0f);
        osc.PhaseAdd(0.0f);
        osc.SetAmp(1.0f);
        wavefolder.Init();
        wavefolder.SetGain(1.0f);
    }

    void LFO::UpdateSettings(PatchSettings* patchSettings)
    {
        waveform = patchSettings->getIntValue((PatchSetting)(LFO_1_WAVEFORM + lfoNumber));
        osc.SetWaveform(waveform + 1); // WAVE_TRI = 1, WAVE_SAW = 2, WAVE_RAMP = 3, WAVE_SQUARE = 4
        freq = patchSettings->getFloatValueLogLookup((PatchSetting)(LFO_1_RATE + lfoNumber));
        basePhase = patchSettings->getFloatValue((PatchSetting)(PatchSetting::LFO_1_TRIGGER_PHASE + lfoNumber));
        noteOnReset = patchSettings->getBoolValue((PatchSetting)(PatchSetting::LFO_1_TRIGGER_RESET_ON + lfoNumber));
        pulseWidth = patchSettings->getFloatValueLinear((PatchSetting)(PatchSetting::LFO_1_PULSE_WIDTH + lfoNumber), 0.03f, 0.97f);
        if (pulseWidth < 0.032f) {
            waveFolderGain = 1.0f;
        } else {
            waveFolderGain = 0.97f + pulseWidth * 5.0f;
        }
    }

    void LFO::Process(float* sample, PatchSettings* patchSettings, float mod, float pwMod, float tphaseMod)
    {
        phase = fmod(basePhase + tphaseMod, 1.0f);
        osc.SetPw(pulseWidth + pwMod);
        osc.SetFreq(freq * (1.0f + mod * 10.0f));

        float waveSample = osc.Process();

        if (waveform == 0) { // Triangle
            wavefolder.SetGain(std::fmax(waveFolderGain + pwMod * 5.0f, 1.0f));
            waveSample = wavefolder.Process(waveSample);
        } else if (waveform > 1) { // Sawtooth or Ramp
            waveSample = fclamp(waveSample * (waveFolderGain + 1.0f) * 0.5f, -1.0f, 1.0f);
        }

        *sample = waveSample;
    }

}
