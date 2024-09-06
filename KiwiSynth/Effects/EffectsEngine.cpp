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
        reverbLevel = patchSettings->getFloatValue(PatchSetting::FX_REVERB);
        if (reverbLevel > 0.002f) {
            reverb.set_opmix(reverbLevel);
        } else {
            reverb.set_opmix(0.0f);
        }
        gain = patchSettings->getFloatValue(PatchSetting::FX_1, 6.0f, 150.0f, LOGARHITHMIC);
        level = patchSettings->getFloatValue(PatchSetting::FX_2, 0.04f, 0.19f);
    }

    void EffectsEngine::ProcessReverbOnly(float* sample)
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

    void EffectsEngine::Process(float* sample)
    {
        // PROCESSING DISTORTION
        if (gain > 6.08f) {
            sample[0] = fclamp(atan(sample[0] * gain) * level, -1.0f, 1.0f);
            sample[1] = fclamp(atan(sample[1] * gain) * level, -1.0f, 1.0f);
        }

        // PROCESSING REVERB
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
