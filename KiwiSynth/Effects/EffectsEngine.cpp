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

        //float output[2] = { 0.0F, 0.0F };
        //reverb.prepare(1);

        //reverb.process(1, &sample, (float**)&output);
        //sample[0] = output[0];
        //sample[1] = output[1];

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

        /*float fxSample[2] = { 0.0F, 0.0F };
        if (reverbLevel > 0.000f) {
            fxSample[0] = 0.0F * reverbLevel;
            fxSample[1] = 0.0F * reverbLevel;
        }
        sample[0] = sample[0] + fxSample[0];
        sample[1] = sample[1] + fxSample[1];*/
    }

}
