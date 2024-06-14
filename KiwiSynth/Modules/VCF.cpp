#include "VCF.h"

namespace kiwi_synth
{
    void VCF::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        filter.Init(sampleRate);
    }

    void VCF::Process(const float* in, float* out, size_t size)
    {
        //float envAmount = env.Process(noteTriggered) * envDepth;
        //float lfoAmount = 1.0f - lfo.Process();
        //float computedFrequency = frequency * lfoAmount + (18000 - frequency) * envAmount ;
        float computedFrequency = frequency;
        filter.SetFreq(computedFrequency);
        for (size_t i = 0; i < size; i++) {

            out[i] = filter.Process(in[i]);
        }
    }
}
