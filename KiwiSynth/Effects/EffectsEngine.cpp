#include "EffectsEngine.h"

namespace kiwi_synth
{
    const float ATAN_DEBOUNCE_THRESHOLD = 5.1f;

    void EffectsEngine::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        reverb.init(sampleRate, false);
        delay.Init();
    }

    void EffectsEngine::UpdateSettings()
    {
        reverbLevel = patchSettings->getFloatValue(PatchSetting::FX_REVERB);
        if (reverbLevel > 0.002f) {
            reverb.set_opmix(reverbLevel);
        } else {
            reverb.set_opmix(0.0f);
        }
        gain = patchSettings->getFloatValueLogLookup(PatchSetting::FX_1);
        level = patchSettings->getFloatValueLinear(PatchSetting::FX_2, 0.04f, 0.22f);

        delay.SetLevel(patchSettings->getFloatValue(PatchSetting::FX_3));
        delay.SetDelaySamples((int)patchSettings->getFloatValueLinear(PatchSetting::FX_4, MIN_DELAY_SAMPLES, MAX_DELAY_SAMPLES));
        delay.SetFeedback(patchSettings->getFloatValueLinear(PatchSetting::FX_5, 0.0f, 0.9f));
    }

    #ifdef __FUNCTIONALITY_OPTION__
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
    #endif // __FUNCTIONALITY_OPTION__

    void EffectsEngine::Process(float* sample)
    {
        // PROCESSING DISTORTION
        if (gain > ATAN_DEBOUNCE_THRESHOLD) {
            sample[0] = fclamp(atan(sample[0] * gain) * level, -0.9999999f, 0.9999999f);
            sample[1] = fclamp(atan(sample[1] * gain) * level, -0.9999999f, 0.9999999f);
        }

        //PROCESSING DELAY
        delay.Process(sample);

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
