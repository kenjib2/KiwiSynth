#include "LFO.h"

namespace kiwi_synth
{
    void LFO::Init(PatchSettings* patchSettings, float sampleRate, uint8_t lfoNumber)
    {
        this->patchSettings = patchSettings;
        this->lfoNumber = lfoNumber;
        noteOnReset = false;
        pulseWidth = 0.5f;
    	osc.Init(sampleRate);
        osc.SetWaveform(Oscillator::WAVE_TRI);
        osc.SetFreq(2.0f);
        osc.PhaseAdd(0.0f);
        wavefolder.Init();
        wavefolder.SetGain(1.0f);
    }

    void LFO::SetFreq(float frequency)
    {
        osc.SetFreq(frequency);
    }

    void LFO::UpdateSettings()
    {
        PatchSetting waveformSetting = PatchSetting::LFO_1_WAVEFORM;
        PatchSetting rateSetting = PatchSetting::LFO_1_RATE;
        PatchSetting triggerPhaseSetting = PatchSetting::LFO_1_TRIGGER_PHASE;
        PatchSetting triggerResetSetting = PatchSetting::LFO_1_TRIGGER_RESET_ON;
        PatchSetting pulseWidthSetting = PatchSetting::LFO_1_PULSE_WIDTH;

        switch(lfoNumber) {
            case 0:
                waveformSetting = PatchSetting::LFO_1_WAVEFORM;
                rateSetting = PatchSetting::LFO_1_RATE;
                triggerPhaseSetting = PatchSetting::LFO_1_TRIGGER_PHASE;
                triggerResetSetting = PatchSetting::LFO_1_TRIGGER_RESET_ON;
                pulseWidthSetting = PatchSetting::LFO_1_PULSE_WIDTH;
                break;
            case 1:
                waveformSetting = PatchSetting::LFO_2_WAVEFORM;
                rateSetting = PatchSetting::LFO_2_RATE;
                triggerPhaseSetting = PatchSetting::LFO_2_TRIGGER_PHASE;
                triggerResetSetting = PatchSetting::LFO_2_TRIGGER_RESET_ON;
                pulseWidthSetting = PatchSetting::LFO_2_PULSE_WIDTH;
                break;
        }
        waveform = patchSettings->getIntValue(waveformSetting);
        switch (waveform) {
            case 0:
                osc.SetWaveform(Oscillator::WAVE_TRI);
                break;
            case 1:
                osc.SetWaveform(Oscillator::WAVE_SQUARE);
                break;
            case 2:
                osc.SetWaveform(Oscillator::WAVE_RAMP);
                break;
            case 3:
                osc.SetWaveform(Oscillator::WAVE_SAW);
                break;
        }
        freq = patchSettings->getFloatValue(rateSetting, LFO_MIN_FREQUENCY, LFO_MAX_FREQUENCY, LOGARHITHMIC);
        phase = patchSettings->getFloatValue(triggerPhaseSetting);
        noteOnReset = patchSettings->getBoolValue(triggerResetSetting);
        pulseWidth = patchSettings->getFloatValue(pulseWidthSetting, 0.03F, 0.97F);
        if (pulseWidth < 0.032F) {
            waveFolderGain = 1.0f;
        } else {
            waveFolderGain = 0.97f + pulseWidth * 27;
        }
    }

    void LFO::Process(float* sample, float mod, float pwMod, bool fold)
    {
        osc.SetPw(std::max(std::fmin(pulseWidth + pwMod, 0.97f), 0.03f));

        float computedFreq = freq;
        if (mod != 0.0F) {
            computedFreq = std::fmax(std::fmin(computedFreq * (1.0F + mod * 10.0F), LFO_MAX_FREQUENCY), LFO_MIN_FREQUENCY);
        }

        osc.SetFreq(computedFreq);

        float waveSample = osc.Process();
        if (waveform == 0 && fold) { // Triangle
            wavefolder.SetGain(std::max(std::fmin(waveFolderGain + pwMod * 27, 28.0f), 1.0f));
            waveSample = wavefolder.Process(waveSample);
        } else if (waveform > 1) { // Sawtooth or Ramp
            waveSample = std::fmax(std::fmin(waveSample * (waveFolderGain + 1.0f) / 2, 1.0F), -1.0F);
        }
        *sample = waveSample;
    }

    void LFO::NoteOn()
    {
        if (noteOnReset) {
            osc.Reset(phase);
        }
    }

}
