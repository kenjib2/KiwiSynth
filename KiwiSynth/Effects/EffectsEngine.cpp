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

        if (patch->getEffectsMode() != mode) {
            UpdateMode(patch->getEffectsMode());
        }
    }

    void EffectsEngine::UpdateMode(EffectsMode newMode) {
        mode = newMode;
        switch (mode) {
            case FX_DIST_DLY_HLRVB: // Hall Reverb
                reverb.set_delay(0.04f);
                //reverb.set_xover(200.0f);
                reverb.set_rtlow(3.0f);
                reverb.set_rtmid(2.0f);
                //reverb.set_fdamp(3e3f);
                //reverb.set_eq1 (1e3f, 0.0f);
                //reverb.set_eq2 (1e3f, 0.0f);
                break;
            case FX_DIST_DLY_CHRVB: // Chamber Reverb
                reverb.set_delay(0.08f);
                reverb.set_rtlow(4.5f);
                reverb.set_rtmid(3.0f);
                break;
            case FX_DIST_DLY_CARVB: // Cathedral Reverb
                reverb.set_delay(0.15f);
                reverb.set_rtlow(10.0f);
                reverb.set_rtmid(6.0f);
                break;
            case FX_DIST_DLY_RMRVB: // Room Reverb
                reverb.set_delay(0.02f);
                reverb.set_rtlow(1.8f);
                reverb.set_rtmid(1.5f);
                break;
            case FX_DIST_DLY_BLRVB: // Bloom Reverb
                reverb.set_delay(0.75f);
                reverb.set_rtlow(4.0f);
                reverb.set_rtmid(6.0f);
                break;
        }
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

        // Prepare to process one sample. Due to changes in Zita we need to pass in the inverse of the number of frames. The inverse of 1 is 1.
        reverb.prepare(1);

        // Process one sample (output can afterwards be read from yl and yr). We pass in the normal number of frames. NOT the inverse this time.
        reverb.process(1, reverbIn, reverbOut);

        sample[0] = yl;
        sample[1] = yr;
    }

}
