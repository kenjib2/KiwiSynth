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
    }

    void LFO::SetFreq(float frequency)
    {
        osc.SetFreq(frequency);
    }

    void LFO::UpdateSettings()
    {
        PatchSetting rateSetting = PatchSetting::LFO_1_RATE;
        PatchSetting triggerPhaseSetting = PatchSetting::LFO_1_TRIGGER_PHASE;
        PatchSetting triggerResetSetting = PatchSetting::LFO_1_TRIGGER_RESET_ON;
        PatchSetting pulseWidthSetting = PatchSetting::LFO_1_PULSE_WIDTH;

        switch(lfoNumber) {
            case 0:
                rateSetting = PatchSetting::LFO_1_RATE;
                triggerPhaseSetting = PatchSetting::LFO_1_TRIGGER_PHASE;
                triggerResetSetting = PatchSetting::LFO_1_TRIGGER_RESET_ON;
                pulseWidthSetting = PatchSetting::LFO_1_PULSE_WIDTH;
                break;
            case 1:
                rateSetting = PatchSetting::LFO_2_RATE;
                triggerPhaseSetting = PatchSetting::LFO_2_TRIGGER_PHASE;
                triggerResetSetting = PatchSetting::LFO_2_TRIGGER_RESET_ON;
                pulseWidthSetting = PatchSetting::LFO_2_PULSE_WIDTH;
                break;
        }
        freq = patchSettings->getFloatValue(rateSetting, LFO_MIN_FREQUENCY, LFO_MAX_FREQUENCY, LOGARHITHMIC);
        phase = patchSettings->getFloatValue(triggerPhaseSetting);
        noteOnReset = patchSettings->getBoolValue(triggerResetSetting);
        pulseWidth = patchSettings->getFloatValue(pulseWidthSetting);

        osc.SetFreq(freq);
    }

    void LFO::Process(float* sample)
    {
        *sample = osc.Process();
    }

    void LFO::NoteOn()
    {
        if (noteOnReset) {
            osc.Reset(phase);
        }
    }

}
