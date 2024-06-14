#include "VCA.h"

namespace kiwi_synth
{
    void VCA::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        noteTriggered = false;
    }

    void VCA::Process(const float* in, float* out, size_t size)
    {
        //float envAmount = env.Process(noteTriggered);
        //float lfoAmount = lfo.Process();
        //out[0] = in[0] * envAmount * (1 - lfoAmount);
        out[0] = in[0];
    }

    bool VCA::IsPlaying()
    {
        //return env.IsRunning();
        return false;
    }


    bool VCA::IsReleasing()
    {
        //return env.GetCurrentSegment() == ADSR_SEG_RELEASE;
        return false;
    }
}
