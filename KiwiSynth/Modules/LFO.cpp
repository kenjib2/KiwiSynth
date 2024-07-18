#include "LFO.h"

namespace kiwi_synth
{
    void LFO::Init(PatchSettings* patchSettings, float sampleRate, uint8_t lfoNumber)
    {
        this->patchSettings = patchSettings;
        this->lfoNumber = lfoNumber;
        retriggerPhase = false;
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
        PatchSetting rate = PatchSetting::LFO_1_RATE;
        PatchSetting triggerPhase = PatchSetting::LFO_1_TRIGGER_PHASE;

        switch(lfoNumber) {
            case 0:
                rate = PatchSetting::LFO_1_RATE;
                triggerPhase = PatchSetting::LFO_1_TRIGGER_PHASE;
                break;
            case 1:
                rate = PatchSetting::LFO_2_RATE;
                triggerPhase = PatchSetting::LFO_2_TRIGGER_PHASE;
                break;
        }
        freq = patchSettings->getFloatValue(rate, LFO_MIN_FREQUENCY, LFO_MAX_FREQUENCY, LOGARHITHMIC);
        phase = patchSettings->getFloatValue(triggerPhase);

        osc.SetFreq(freq);
    }

    void LFO::Process(float* sample)
    {
        *sample = osc.Process();
    }

    void LFO::NoteOn()
    {
        if (retriggerPhase) {
            osc.Reset(phase);
        }
    }

}
