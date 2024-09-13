#include "EffectsEngine.h"

namespace kiwi_synth
{
    const float ATAN_DEBOUNCE_THRESHOLD = 5.1f;

    void EffectsEngine::Init(Patch* patch, float sampleRate)
    {
        this->patch = patch;
        reverb.init(sampleRate, false);
        delay.Init();
    }

    void EffectsEngine::UpdateSettings()
    {
        reverbLevel = patch->activeSettings->getFloatValue(PatchSetting::FX_REVERB);
        if (reverbLevel > 0.002f) {
            reverb.set_opmix(reverbLevel);
        } else {
            reverb.set_opmix(0.0f);
        }
        gain = patch->activeSettings->getFloatValueLogLookup(PatchSetting::FX_1);
        level = patch->activeSettings->getFloatValueLinear(PatchSetting::FX_2, 0.02f, 0.22f);

        delay.SetLevel(patch->activeSettings->getFloatValue(PatchSetting::FX_3));
        delay.SetDelaySamples((int)patch->activeSettings->getFloatValueLinear(PatchSetting::FX_4, MIN_DELAY_SAMPLES, MAX_DELAY_SAMPLES));
        delay.SetFeedback(patch->activeSettings->getFloatValueLinear(PatchSetting::FX_5, 0.0f, 0.9f));
    }

    void EffectsEngine::Process(float* sample)
    {
        // PROCESSING DISTORTION
        if (gain > ATAN_DEBOUNCE_THRESHOLD) {
            sample[0] = atan(sample[0] * gain) * level; // atan has a limit of pi/2 or @1.57. The max of level is 0.22 so total output will never exceed +/- 0.346
            sample[1] = atan(sample[1] * gain) * level;
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
