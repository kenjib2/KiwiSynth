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

        filter.SetFreq(patchSettings->getFloatValue(PatchSetting::VCF_CUTOFF, 40.0F, 22000.0F, kiwi_synth::LOGARHITHMIC));
        //filter.SetFreq(patchSettings->getFloatValue(PatchSetting::VCF_CUTOFF) * 18000.0F);
        filter.SetRes(patchSettings->getFloatValue(PatchSetting::VCF_RESONANCE, 0.0F, 1.0F));
        //filter.SetFreq(200.0F);
        //filter.SetRes(1.0F);
    }

    void VCF::Process(float* sample)
    {
        float output = filter.Process(*sample);
        *sample = output;
    }
}
