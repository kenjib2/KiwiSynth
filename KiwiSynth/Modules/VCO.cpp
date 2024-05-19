#include "VCO.h"

namespace kiwi_synth
{
    VCO::VCO(PatchSettings* patchSettings, float sampleRate) : patchSettings(patchSettings)
    {
    }

    void VCO::CalculateBaseFreq()
    {
        float masterTune = patchSettings->getFloatValue(PatchSetting::VCO_MASTER_TUNE, -1.0f, 1.0f, Scale::OCTAVE);
        float tuneHz = pow(2, masterTune * 2.0f - 1.0f);

        baseFreq = mtof(midiNote) * tuneHz;
    }

    void VCO::Process(float * out, size_t size)
    {
        CalculateBaseFreq();
        osc.SetFreq(baseFreq);

        float result;
        for (size_t i = 0; i < size; i++) {
            result = osc.Process();
            out[i] = result;
        }
    }
}
