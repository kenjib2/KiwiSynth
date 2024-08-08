#include "EffectsEngine.h"

namespace kiwi_synth
{
    void EffectsEngine::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        reverb.init(sampleRate, false);
    }

    void EffectsEngine::UpdateSettings()
    {
        reverbLevel = patchSettings->getFloatValue(PatchSetting::FX_REVERB, -0.001F, 1.0F); // Start in negative to deal with low level pot noise
        if (reverbLevel > 0.0F) {
            reverb.set_opmix(std::fmin(reverbLevel, 1.0F));
        }
    }

    void EffectsEngine::Process(float* sample)
    {
        float xl = sample[0];
        float xr = sample[1];

        // Pack input variables in array.
        float *reverbIn[2] = {&xl, &xr};

        // The variables will contain the output of the reverb.
        float yl, yr;
        float *reverbOut[2] = {&yl, &yr};

        // Prepare to process one sample.
        reverb.prepare(1);

        // Process one sample (output can afterwards be read from yl and yr).
        reverb.process(1, reverbIn, reverbOut);

        sample[0] = yl;
        sample[1] = yr;
    }

}
