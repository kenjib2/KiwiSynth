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

    void VCO::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
    {
        //CalculateBaseFreq();
        //osc.SetFreq(baseFreq);

        float result;
        for(size_t i = 0; i < size; i += 2)
        {
            // Process
            result = osc.Process() * 0.05f;
            out[i] = result;
            out[i + 1] = result;
        }
    }
}
