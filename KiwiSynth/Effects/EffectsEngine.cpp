#include "EffectsEngine.h"

namespace kiwi_synth
{
    const float ATAN_DEBOUNCE_THRESHOLD = 5.1f;
    const float FREQ_DEBOUNCE_THRESHOLD = 0.05f;

    void EffectsEngine::Init(Patch* patch, float sampleRate)
    {
        this->patch = patch;
        chorusL.Init(sampleRate, 0);
        chorusR.Init(sampleRate, 1);
        chorusL.SetPan(0.0f);
        chorusR.SetPan(0.0f);
        phaserL.Init(sampleRate, 2);
        phaserR.Init(sampleRate, 3);
        phaserL.SetPoles(6);
        phaserR.SetPoles(6);
        phaserL.SetFeedback(0.1);
        phaserR.SetFeedback(0.1);
        flangerL.Init(sampleRate, 4);
        flangerR.Init(sampleRate, 5);
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

        freq = patch->activeSettings->getFloatValueLogLookup(PatchSetting::MOD_1_DEPTH);   // Using MOD_1_DEPTH as a hacky fill in for alternate FX_1 value
        chorusL.SetLfoFreq(freq);
        chorusR.SetLfoFreq(freq * 0.95f);
        phaserL.SetLfoFreq(freq);
        phaserR.SetLfoFreq(freq);
        flangerL.SetLfoFreq(freq);
        flangerR.SetLfoFreq(freq);
        feedback = chorusDepth = patch->activeSettings->getFloatValue(PatchSetting::FX_2);
        phaserDepth = patch->activeSettings->getFloatValueLinear(PatchSetting::FX_2, 0.2f, 1.0f);
        chorusL.SetLfoDepth(chorusDepth);
        chorusR.SetLfoDepth(chorusDepth);
        phaserL.SetLfoDepth(phaserDepth);
        phaserR.SetLfoDepth(phaserDepth);
        flangerL.SetFeedback(feedback);
        flangerR.SetFeedback(feedback);

        delay.SetLevel(patch->activeSettings->getFloatValue(PatchSetting::FX_3));
        delay.SetDelaySamples((int)patch->activeSettings->getFloatValueLinear(PatchSetting::FX_4, MIN_DELAY_SAMPLES, MAX_DELAY_SAMPLES));
        delay.SetFeedback(patch->activeSettings->getFloatValueLinear(PatchSetting::FX_5, 0.0f, 0.9f));

        effectsMode = patch->getEffectsMode();
        if (patch->getReverbMode() != reverbMode) {
            UpdateReverbMode(patch->getReverbMode());
        }
    }

    void EffectsEngine::UpdateReverbMode(ReverbMode newMode) {
        reverbMode = newMode;
        switch (reverbMode) {
            case REVERB_ROOM:
                reverb.set_delay(0.02f);
                reverb.set_rtlow(1.8f);
                reverb.set_rtmid(1.5f);
                break;
            case REVERB_HALL:
                reverb.set_delay(0.04f);
                //reverb.set_xover(200.0f);
                reverb.set_rtlow(3.0f);
                reverb.set_rtmid(2.0f);
                //reverb.set_fdamp(3e3f);
                //reverb.set_eq1 (1e3f, 0.0f);
                //reverb.set_eq2 (1e3f, 0.0f);
                break;
            case REVERB_CHAMBER:
                reverb.set_delay(0.05f);
                reverb.set_rtlow(4.5f);
                reverb.set_rtmid(3.0f);
                break;
            case REVERB_CATHEDRAL:
                //reverb.set_delay(0.15f); Need to update Zita for larger than 0.1f
                reverb.set_delay(0.06f);
                //reverb.set_rtlow(10.0f); Need to update Zita for larger than 8 sec
                reverb.set_rtlow(8.0f);
                reverb.set_rtmid(6.0f);
                break;
            case REVERB_BLOOM:
                //reverb.set_delay(0.75f); Need to update Zita for larger than 0.1f
                reverb.set_delay(0.1f);
                reverb.set_rtlow(2.0f);
                reverb.set_rtmid(5.0f);
                break;
        }
    }

    void EffectsEngine::Process(float* sample)
    {
        // PROCESSING DISTORTION
        if (effectsMode == FX_DISTORTION_DELAY && gain > ATAN_DEBOUNCE_THRESHOLD) {
            sample[0] = atan(sample[0] * gain) * level; // atan has a limit of pi/2 or @1.57. The max of level is 0.22 so total output will never exceed +/- 0.346
            sample[1] = atan(sample[1] * gain) * level;
        }

        // PROCESSING CHORUS
        if (effectsMode == FX_CHORUS_DELAY && freq > FREQ_DEBOUNCE_THRESHOLD) {
            sample[0] = chorusL.Process(sample[0]);
            sample[1] = chorusR.Process(sample[1]);
        }

        //PROCESSING DELAY
        delay.Process(sample);

        // PROCESSING FLANGER
        if (effectsMode == FX_FLANGER_DELAY && freq > FREQ_DEBOUNCE_THRESHOLD) {
            sample[0] = flangerL.Process(sample[0]);
            sample[1] = flangerR.Process(sample[1]);
        }

        // PROCESSING PHASER
        if (effectsMode == FX_PHASER_DELAY && freq > FREQ_DEBOUNCE_THRESHOLD) {
            sample[0] = phaserL.Process(sample[0]);
            sample[1] = phaserR.Process(sample[1]);
        }

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
