#include "VCO.h"

namespace kiwi_synth
{
    void VCO::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        midiNote = 60;
    	osc.Init(sampleRate);
        osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
        osc.SetFreq(220);
    }

    void VCO::CalculateBaseFreq()
    {
        float masterTune = patchSettings->getFloatValue(PatchSetting::VCO_MASTER_TUNE, -1.0f, 1.0f, Scale::OCTAVE);
        float tuneHz = pow(2, masterTune * 2.0f - 1.0f);

        baseFreq = mtof(midiNote) * tuneHz;
    }

    void VCO::SetFreq(float frequency)
    {
        osc.SetFreq(frequency);
    }

    void VCO::UpdateSettings()
    {
        //CalculateBaseFreq();
        //osc.SetFreq(baseFreq);
    }

    void VCO::Process(float* sample, float* mods, uint8_t numMods)
    {
        *sample = osc.Process();
    }
}
