#include "EffectsEngine.h"



using namespace kiwisynth;



const float FREQ_DEBOUNCE_THRESHOLD = 0.05f;



void EffectsEngine::Init(Patch* patch, float sampleRate)
{
    patch_ = patch;

    distortion_.Init();
    delay_.Init();
    chorusL_.Init(sampleRate, 0);
    chorusR_.Init(sampleRate, 1);
    phaserL_.Feedback(0.3f);
    phaserR_.Feedback(0.3f);
    phaserL_.Range(440.f, 1600.f);
    phaserR_.Range(440.f, 1600.f);
    flangerL_.Init(sampleRate, 4);
    flangerR_.Init(sampleRate, 5);
    decimatorL_.Init();
    decimatorR_.Init();
    decimatorL_.SetSmoothCrushing(true);
    decimatorR_.SetSmoothCrushing(true);

    reverb_.init(sampleRate);
}



void EffectsEngine::UpdateSettings()
{
    float reverbLevel = patch_->voice1Settings_->getFloatValue(PatchSetting::FX_REVERB);
    if (reverbLevel > 0.002f) {
        reverb_.set_opmix(reverbLevel);
    } else {
        reverb_.set_opmix(0.0f);
    }

    distortion_.SetGain(patch_->voice1Settings_->getFloatValueLogLookup(PatchSetting::FX_1));
    distortion_.SetLevel(patch_->voice1Settings_->getFloatValueLinear(PatchSetting::FX_2, 0.02f, 0.25f));

    float bitcrushFactor = patch_->voice1Settings_->getFloatValueLinear(PatchSetting::FX_3, 0.0f, 0.8f);
    float downsampleFactor = patch_->voice1Settings_->getFloatValue(PatchSetting::FX_4);
    float decimatorLevel = patch_->voice1Settings_->getFloatValueLinear(PatchSetting::FX_5, 0.5f, 1.0f);
    decimatorL_.SetBitcrushFactor(bitcrushFactor);
    decimatorR_.SetBitcrushFactor(bitcrushFactor);
    decimatorL_.SetDownsampleFactor(downsampleFactor);
    decimatorR_.SetDownsampleFactor(downsampleFactor);
    decimatorL_.SetLevel(decimatorLevel);
    decimatorR_.SetLevel(decimatorLevel);

    float freq = patch_->voice1Settings_->getFloatValueLogLookup(PatchSetting::MOD_1_DEPTH);   // Using MOD_1_DEPTH as a hacky fill in for alternate FX_1 value
    float feedback = patch_->voice1Settings_->getFloatValue(PatchSetting::FX_2) * 0.9f;
    float chorusDepth = feedback;
    float phaserDepth = patch_->voice1Settings_->getFloatValueLinear(PatchSetting::FX_2, 0.2f, 1.0f);

    chorusL_.SetLfoFreq(freq);
    chorusR_.SetLfoFreq(freq * 0.95f);
    chorusL_.SetLfoDepth(chorusDepth);
    chorusR_.SetLfoDepth(chorusDepth);

    phaserL_.Rate(freq);
    phaserR_.Rate(freq);
    phaserL_.Depth(phaserDepth);
    phaserR_.Depth(phaserDepth);
    phaserL_.Feedback(phaserDepth * 0.75f);
    phaserR_.Feedback(phaserDepth * 0.75f);

    flangerL_.SetLfoFreq(freq);
    flangerR_.SetLfoFreq(freq);
    flangerL_.SetFeedback(feedback);
    flangerR_.SetFeedback(feedback);

    float delayLevel = patch_->voice1Settings_->getFloatValue(PatchSetting::FX_3);
    delay_.SetLevel(delayLevel);
    int delaySamples = (int)patch_->voice1Settings_->getFloatValueLinear(PatchSetting::FX_4, MIN_DELAY_SAMPLES, MAX_DELAY_SAMPLES);
    delay_.SetDelaySamples(delaySamples);
    float delayFeedback = patch_->voice1Settings_->getFloatValueLinear(PatchSetting::FX_5, 0.0f, 0.9f);
    delay_.SetFeedback(delayFeedback);

    effectsMode_ = patch_->GetEffectsMode();
    if (patch_->GetReverbMode() != reverbMode_) {
        UpdateReverbMode(patch_->GetReverbMode());
    }
}



void EffectsEngine::UpdateReverbMode(ReverbMode newMode) {
    reverbMode_ = newMode;
    switch (reverbMode_) {
        case REVERB_ROOM:
            reverb_.set_delay(0.02f);
            reverb_.set_rtlow(1.8f);
            reverb_.set_xover(200.0f);
            reverb_.set_rtmid(1.5f);
            reverb_.set_fdamp(3e3f);
            reverb_.set_eq1 (1e3f, 0.0f);
            reverb_.set_eq2 (1e3f, 0.0f);
            break;
        case REVERB_HALL:
            reverb_.set_delay(0.04f);
            reverb_.set_rtlow(3.0f);
            reverb_.set_xover(200.0f);
            reverb_.set_rtmid(2.0f);
            reverb_.set_fdamp(3e3f);
            reverb_.set_eq1 (1e3f, 0.0f);
            reverb_.set_eq2 (1e3f, 0.0f);
            break;
        case REVERB_CHAMBER:
            reverb_.set_delay(0.05f);
            reverb_.set_rtlow(4.5f);
            reverb_.set_xover(200.0f);
            reverb_.set_rtmid(3.0f);
            reverb_.set_fdamp(3e3f);
            reverb_.set_eq1 (1e3f, 0.0f);
            reverb_.set_eq2 (1e3f, 0.0f);
            break;
        case REVERB_CATHEDRAL:
            reverb_.set_delay(0.15f);
            reverb_.set_rtlow(8.0f);
            reverb_.set_xover(200.0f);
            reverb_.set_rtmid(6.0f);
            reverb_.set_fdamp(3e3f);
            reverb_.set_eq1 (1e3f, 0.0f);
            reverb_.set_eq2 (1e3f, 0.0f);
            break;
        case REVERB_BLOOM:
            reverb_.set_delay(0.75f);
            reverb_.set_rtlow(3.0f);
            reverb_.set_xover(200.0f);
            reverb_.set_rtmid(5.0f);
            reverb_.set_fdamp(4e3f);
            reverb_.set_eq1 (1e3f, 0.0f);
            reverb_.set_eq2 (1e3f, 0.0f);
        case REVERB_BRIGHT:
            reverb_.set_delay(0.02f);
            reverb_.set_rtlow(2.5f);
            reverb_.set_xover(350.0f);
            reverb_.set_rtmid(5.0f);
            reverb_.set_fdamp(4.8e3f);
            reverb_.set_eq1 (7e3f, 2.0f);
            reverb_.set_eq2 (200.0f, -2.0f);
            break;
    }
}



void EffectsEngine::Process(float* sample)
{
    switch (effectsMode_) {
        case FX_PHASER_DELAY:
            sample[0] = phaserL_.Process(sample[0]);
            sample[1] = phaserR_.Process(sample[1]);
            delay_.Process(sample);
            break;
        case FX_DISTORTION_DELAY:
            sample[0] = distortion_.Process(sample[0]);
            sample[1] = distortion_.Process(sample[1]);
            delay_.Process(sample);
            break;
        case FX_CHORUS_DELAY:
            sample[0] = chorusL_.Process(sample[0]);
            sample[1] = chorusR_.Process(sample[1]);
            delay_.Process(sample);
            break;
        case FX_FLANGER_DELAY:
            sample[0] = flangerL_.Process(sample[0]);
            sample[1] = flangerR_.Process(sample[1]);
            delay_.Process(sample);
            break;
        case FX_DISTORTION_BITCRUSH:
            sample[0] = decimatorL_.Process(sample[0]);
            sample[1] = decimatorR_.Process(sample[1]);
            sample[0] = distortion_.Process(sample[0]);
            sample[1] = distortion_.Process(sample[1]);
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
    reverb_.prepare();

    // Process one sample (output can afterwards be read from yl and yr). We pass in the normal number of frames. NOT the inverse this time.
    reverb_.process(reverbIn, reverbOut);

    sample[0] = yl;
    sample[1] = yr;
}
