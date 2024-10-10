#include "EffectsEngine.h"

namespace kiwi_synth
{
    const float FREQ_DEBOUNCE_THRESHOLD = 0.05f;

    void EffectsEngine::Init(Patch* patch, float sampleRate)
    {
        this->patch = patch;

        distortion.Init();
        delay.Init();
        chorusL.Init(sampleRate, 0);
        chorusR.Init(sampleRate, 1);
        phaserL.Init(sampleRate, 2);
        phaserR.Init(sampleRate, 3);
        phaserL.SetPoles(2);
        phaserR.SetPoles(2);
        phaserL.SetFeedback(0.0);
        phaserR.SetFeedback(0.0);
        phaserL.SetFreq(150.0f);
        phaserR.SetFreq(150.0f);
        flangerL.Init(sampleRate, 4);
        flangerR.Init(sampleRate, 5);
        decimatorL.Init();
        decimatorR.Init();
        decimatorL.SetSmoothCrushing(true);
        decimatorR.SetSmoothCrushing(true);

        reverb.init(sampleRate);
    }

    void EffectsEngine::UpdateSettings()
    {
        float reverbLevel = patch->voice1Settings->getFloatValue(PatchSetting::FX_REVERB);
        if (reverbLevel > 0.002f) {
            reverb.set_opmix(reverbLevel);
        } else {
            reverb.set_opmix(0.0f);
        }

        distortion.SetGain(patch->voice1Settings->getFloatValueLogLookup(PatchSetting::FX_1));
        distortion.SetLevel(patch->voice1Settings->getFloatValueLinear(PatchSetting::FX_2, 0.02f, 0.25f));

        float bitcrushFactor = patch->voice1Settings->getFloatValueLinear(PatchSetting::FX_3, 0.0f, 0.8f);
        float downsampleFactor = patch->voice1Settings->getFloatValue(PatchSetting::FX_4);
        float decimatorLevel = patch->voice1Settings->getFloatValueLinear(PatchSetting::FX_5, 0.5f, 1.0f);
        decimatorL.SetBitcrushFactor(bitcrushFactor);
        decimatorR.SetBitcrushFactor(bitcrushFactor);
        decimatorL.SetDownsampleFactor(downsampleFactor);
        decimatorR.SetDownsampleFactor(downsampleFactor);
        decimatorL.SetLevel(decimatorLevel);
        decimatorR.SetLevel(decimatorLevel);

        float freq = patch->voice1Settings->getFloatValueLogLookup(PatchSetting::MOD_1_DEPTH);   // Using MOD_1_DEPTH as a hacky fill in for alternate FX_1 value
        float feedback = patch->voice1Settings->getFloatValue(PatchSetting::FX_2) * 0.9f;
        float chorusDepth = feedback;
        float phaserDepth = patch->voice1Settings->getFloatValueLinear(PatchSetting::FX_2, 0.2f, 1.0f);

        chorusL.SetLfoFreq(freq);
        chorusR.SetLfoFreq(freq * 0.95f);
        chorusL.SetLfoDepth(chorusDepth);
        chorusR.SetLfoDepth(chorusDepth);

        phaserL.SetLfoFreq(freq);
        phaserR.SetLfoFreq(freq);
        phaserL.SetLfoDepth(phaserDepth);
        phaserR.SetLfoDepth(phaserDepth);

        flangerL.SetLfoFreq(freq);
        flangerR.SetLfoFreq(freq);
        flangerL.SetFeedback(feedback);
        flangerR.SetFeedback(feedback);

        float delayLevel = patch->voice1Settings->getFloatValue(PatchSetting::FX_3);
        delay.SetLevel(delayLevel);
        int delaySamples = (int)patch->voice1Settings->getFloatValueLinear(PatchSetting::FX_4, MIN_DELAY_SAMPLES, MAX_DELAY_SAMPLES);
        delay.SetDelaySamples(delaySamples);
        float delayFeedback = patch->voice1Settings->getFloatValueLinear(PatchSetting::FX_5, 0.0f, 0.9f);
        delay.SetFeedback(delayFeedback);

        effectsMode = patch->GetEffectsMode();
        if (patch->GetReverbMode() != reverbMode) {
            UpdateReverbMode(patch->GetReverbMode());
        }
    }

    void EffectsEngine::UpdateReverbMode(ReverbMode newMode) {
        reverbMode = newMode;
        switch (reverbMode) {
            case REVERB_ROOM:
                reverb.set_delay(0.02f);
                reverb.set_rtlow(1.8f);
                reverb.set_xover(200.0f);
                reverb.set_rtmid(1.5f);
                reverb.set_fdamp(3e3f);
                reverb.set_eq1 (1e3f, 0.0f);
                reverb.set_eq2 (1e3f, 0.0f);
                break;
            case REVERB_HALL:
                reverb.set_delay(0.04f);
                reverb.set_rtlow(3.0f);
                reverb.set_xover(200.0f);
                reverb.set_rtmid(2.0f);
                reverb.set_fdamp(3e3f);
                reverb.set_eq1 (1e3f, 0.0f);
                reverb.set_eq2 (1e3f, 0.0f);
                break;
            case REVERB_CHAMBER:
                reverb.set_delay(0.05f);
                reverb.set_rtlow(4.5f);
                reverb.set_xover(200.0f);
                reverb.set_rtmid(3.0f);
                reverb.set_fdamp(3e3f);
                reverb.set_eq1 (1e3f, 0.0f);
                reverb.set_eq2 (1e3f, 0.0f);
                break;
            case REVERB_CATHEDRAL:
                reverb.set_delay(0.15f);
                //reverb.set_rtlow(10.0f); Need to update Zita for larger than 8 sec
                reverb.set_rtlow(8.0f);
                reverb.set_xover(200.0f);
                reverb.set_rtmid(6.0f);
                reverb.set_fdamp(3e3f);
                reverb.set_eq1 (1e3f, 0.0f);
                reverb.set_eq2 (1e3f, 0.0f);
                break;
            case REVERB_BLOOM:
                reverb.set_delay(0.75f);
                reverb.set_rtlow(3.0f);
                reverb.set_xover(200.0f);
                reverb.set_rtmid(5.0f);
                reverb.set_fdamp(4e3f);
                reverb.set_eq1 (1e3f, 0.0f);
                reverb.set_eq2 (1e3f, 0.0f);
            case REVERB_BRIGHT:
                reverb.set_delay(0.02f);
                reverb.set_rtlow(2.5f);
                reverb.set_xover(350.0f);
                reverb.set_rtmid(5.0f);
                reverb.set_fdamp(4.8e3f);
                reverb.set_eq1 (7e3f, 2.0f);
                reverb.set_eq2 (200.0f, -2.0f);
                break;
        }
    }

    void EffectsEngine::Process(float* sample)
    {
        switch (effectsMode) {
            case FX_PHASER_DELAY:
                sample[0] = phaserL.Process(sample[0]);
                sample[1] = phaserR.Process(sample[1]);
                delay.Process(sample);
                break;
            case FX_DISTORTION_DELAY:
                sample[0] = distortion.Process(sample[0]);
                sample[1] = distortion.Process(sample[1]);
                delay.Process(sample);
                break;
            case FX_CHORUS_DELAY:
                sample[0] = chorusL.Process(sample[0]);
                sample[1] = chorusR.Process(sample[1]);
                delay.Process(sample);
                break;
            case FX_FLANGER_DELAY:
                sample[0] = flangerL.Process(sample[0]);
                sample[1] = flangerR.Process(sample[1]);
                delay.Process(sample);
                break;
            case FX_DISTORTION_BITCRUSH:
                sample[0] = decimatorL.Process(sample[0]);
                sample[1] = decimatorR.Process(sample[1]);
                sample[0] = distortion.Process(sample[0]);
                sample[1] = distortion.Process(sample[1]);
                break;
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
        reverb.prepare();

        // Process one sample (output can afterwards be read from yl and yr). We pass in the normal number of frames. NOT the inverse this time.
        reverb.process(reverbIn, reverbOut);

        sample[0] = yl;
        sample[1] = yr;
    }

}
