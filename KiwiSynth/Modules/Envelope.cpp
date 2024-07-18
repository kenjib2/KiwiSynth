#include "Envelope.h"

namespace kiwi_synth
{
    void Envelope::Init(PatchSettings* patchSettings, float sampleRate, uint8_t envNumber)
    {
        this->patchSettings = patchSettings;
        this->envNumber = envNumber;
        noteTriggered = false;
        env.Init(sampleRate);

        env.SetAttackTime(0.002F);
        env.SetDecayTime(0.0f);
        env.SetSustainLevel(1.0f);
        env.SetReleaseTime(0.002f);
    }

    void Envelope::UpdateSettings()
    {
        PatchSetting attack = PatchSetting::ENV_1_ATTACK;
        PatchSetting decay = PatchSetting::ENV_1_DECAY;
        PatchSetting sustain = PatchSetting::ENV_1_SUSTAIN;
        PatchSetting release = PatchSetting::ENV_1_RELEASE;

        switch(envNumber) {
            case 0:
                attack = PatchSetting::ENV_1_ATTACK;
                decay = PatchSetting::ENV_1_DECAY;
                sustain = PatchSetting::ENV_1_SUSTAIN;
                release = PatchSetting::ENV_1_RELEASE;
                break;
            case 1:
                attack = PatchSetting::ENV_2_ATTACK;
                decay = PatchSetting::ENV_2_DECAY;
                sustain = PatchSetting::ENV_2_SUSTAIN;
                release = PatchSetting::ENV_2_RELEASE;
                break;
        }
        env.SetAttackTime(patchSettings->getFloatValue(attack, 0.0F, 3.0F));
        env.SetDecayTime(patchSettings->getFloatValue(decay, 0.0F, 3.0F));
        env.SetSustainLevel(patchSettings->getFloatValue(sustain, 0.0F, 0.99999F));
        env.SetReleaseTime(patchSettings->getFloatValue(release, 0.0F, 3.0F));
    }

    void Envelope::Process(float* sample)
    {
        *sample = *sample * env.Process(noteTriggered);

        /*float envAmount;
        for(size_t i = 0; i < size; i += 2)
        {
            // Process
            envAmount =  env.Process(noteTriggered);
            out[i] = out[i] * envAmount;
            out[i + 1] = out[i + 1] * envAmount;
            //out[i] = out[i] * 0.08f;
            //out[i + 1] = out[i + 1] * 0.08f;
        }*/
    }

    void Envelope::NoteOn(bool retrigger)
    {
        noteTriggered = true;
        if (env.IsRunning()) {
            env.Retrigger(retrigger);
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
