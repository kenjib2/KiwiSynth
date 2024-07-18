#include "VCF.h"

namespace kiwi_synth
{
    void VCF::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        filter.Init(sampleRate);
    }

    void VCF::UpdateSettings()
    {
        //float envAmount = env.Process(noteTriggered) * envDepth;
        //float lfoAmount = 1.0f - lfo.Process();
        //float computedFrequency = frequency * lfoAmount + (18000 - frequency) * envAmount ;

        frequency = patchSettings->getFloatValue(PatchSetting::VCF_CUTOFF, VCF_MIN_FREQUENCY, VCF_MAX_FREQUENCY, kiwi_synth::LOGARHITHMIC);
        resonance = patchSettings->getFloatValue(PatchSetting::VCF_RESONANCE);
        keyboardTracking = patchSettings->getFloatValue(PatchSetting::VCF_TRACKING);
        env1Depth = patchSettings->getFloatValue(PatchSetting::VCF_ENV_1_DEPTH);
        env2Depth = patchSettings->getFloatValue(PatchSetting::VCF_ENV_2_DEPTH);
        lfo2Depth = patchSettings->getFloatValue(PatchSetting::LFO_2_TO_VCF_CUTOFF, -0.001F, 1.0F);

        filter.SetRes(resonance);
    }

    void VCF::Process(float* sample, float* mods, uint8_t numMods)
    {
        float computedFrequency = frequency;
        for (int i = 0; i < numMods; i++) {
            switch (i)
            {
                case 0:
                    computedFrequency = std::min(computedFrequency + mods[i] * keyboardTracking * 0.8F, VCF_MAX_FREQUENCY) ;
                    break;
                case 1:
                    computedFrequency = std::min(computedFrequency + (VCF_MAX_FREQUENCY - VCF_MIN_FREQUENCY - computedFrequency) * mods[i] * env1Depth, VCF_MAX_FREQUENCY) ;
                    break;
                case 2:
                    computedFrequency = std::min(computedFrequency + (VCF_MAX_FREQUENCY - VCF_MIN_FREQUENCY - computedFrequency) * mods[i] * env2Depth, VCF_MAX_FREQUENCY) ;
                    break;
                case 3:
                    if (lfo2Depth > 0.0F) {
                        computedFrequency = std::min(computedFrequency * (1.0F + mods[i] * 2.0F * lfo2Depth), VCF_MAX_FREQUENCY) ;
                    }
                    break;
                default:
                    computedFrequency = std::min(computedFrequency + (VCF_MAX_FREQUENCY - computedFrequency) * mods[i], VCF_MAX_FREQUENCY) ;
                    break;
            }
        }
        filter.SetFreq(computedFrequency);

        float output = filter.Process(*sample);
        *sample = output;
    }
}
