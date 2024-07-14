#include "VCA.h"

namespace kiwi_synth
{
    void VCA::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        noteTriggered = false;
        env.Init(sampleRate);

        env.SetAttackTime(0.002F);
        env.SetDecayTime(0.0f);
        env.SetSustainLevel(1.0f);
        env.SetReleaseTime(0.002f);
    }

    void VCA::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
    {
        // Why are these times not matching seconds at all?
        env.SetAttackTime(patchSettings->getFloatValue(PatchSetting::ENV_1_ATTACK, 0.0F, 0.2F));
        env.SetDecayTime(patchSettings->getFloatValue(PatchSetting::ENV_1_DECAY, 0.0F, 0.03F));
        env.SetSustainLevel(patchSettings->getFloatValue(PatchSetting::ENV_1_SUSTAIN, 0.0F, 0.99999F));
        env.SetReleaseTime(patchSettings->getFloatValue(PatchSetting::ENV_1_RELEASE, 0.0F, 0.03F));
        float envAmount = env.Process(noteTriggered);
        //out[0] = in[0] * envAmount * (1 - lfoAmount);
        //out[0] = in[0];
        for(size_t i = 0; i < size; i += 2)
        {
            // Process
            out[i] = out[i] * envAmount;
            out[i + 1] = out[i + 1] * envAmount;
        }
    }

    void VCA::NoteOn()
    {
        noteTriggered = true;
        if (env.IsRunning()) {
            env.Retrigger(false);
        }
    }

    void VCA::NoteOff()
    {
        noteTriggered = false;
    }

    bool VCA::IsPlaying()
    {
        return env.IsRunning();
        return false;
    }


    bool VCA::IsReleasing()
    {
        return env.GetCurrentSegment() == ADSR_SEG_RELEASE;
        return false;
    }
}
