#include "Envelope.h"

namespace kiwi_synth
{
    void Envelope::Init(PatchSettings* patchSettings, float sampleRate)
    {
        this->patchSettings = patchSettings;
        noteTriggered = false;
        env.Init(sampleRate);

        env.SetAttackTime(0.002F);
        env.SetDecayTime(0.0f);
        env.SetSustainLevel(1.0f);
        env.SetReleaseTime(0.002f);
    }

    void Envelope::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
    {
        // Why are these times not matching seconds at all?
        env.SetAttackTime(patchSettings->getFloatValue(PatchSetting::ENV_1_ATTACK, 0.0F, 0.2F));
        env.SetDecayTime(patchSettings->getFloatValue(PatchSetting::ENV_1_DECAY, 0.0F, 0.03F));
        env.SetSustainLevel(patchSettings->getFloatValue(PatchSetting::ENV_1_SUSTAIN, 0.0F, 0.99999F));
        env.SetReleaseTime(patchSettings->getFloatValue(PatchSetting::ENV_1_RELEASE, 0.0F, 0.03F));
        float envAmount;
        for(size_t i = 0; i < size; i += 2)
        {
            envAmount = env.Process(noteTriggered);
            out[i] = envAmount;
            out[i + 1] = envAmount;
        }
    }

    void Envelope::NoteOn()
    {
        noteTriggered = true;
        if (env.IsRunning()) {
            env.Retrigger(false);
        }
    }

    void Envelope::NoteOff()
    {
        noteTriggered = false;
    }

    bool Envelope::IsPlaying()
    {
        return env.IsRunning();
        return false;
    }


    bool Envelope::IsReleasing()
    {
        return env.GetCurrentSegment() == ADSR_SEG_RELEASE;
        return false;
    }
}
