#include "Envelope.h"

namespace kiwi_synth
{
    void Envelope::Init(PatchSettings* patchSettings, float sampleRate, uint8_t envNumber)
    {
        this->patchSettings = patchSettings;
        this->envNumber = envNumber;
        noteTriggered = false;
        quickRelease = false;
        reversed = false;
        env.Init(sampleRate);

        env.SetAttackTime(0.002f);
        env.SetDecayTime(0.0f);
        env.SetSustainLevel(1.0f);
        env.SetReleaseTime(0.002f);

        prevAttackMod = 0.0f;
        prevDecayMod = 0.0f;
        prevSustainMod = 0.0f;
        prevReleaseMod = 0.0f;
    }

    void Envelope::UpdateSettings()
    {
        if (!quickRelease) {
            env.SetAttackTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_ATTACK + envNumber)) + prevAttackMod), 0.0f) * 3.0f);
            env.SetDecayTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_DECAY + envNumber)) + prevDecayMod), 0.0f) * 3.0f);
            env.SetSustainLevel(fclamp(patchSettings->getFloatValue((PatchSetting)(ENV_1_SUSTAIN + envNumber)) + prevSustainMod, 0.0f, 1.0f));
            releaseValue = std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_RELEASE + envNumber)) + prevReleaseMod), 0.0f) * 3.0f;
            reversed = patchSettings->getBoolValue((PatchSetting)(ENV_1_REVERSE_PHASE_ON + envNumber));

            env.SetReleaseTime(releaseValue);
        }
    }

    #ifdef __FUNCTIONALITY_OPTION__
    void Envelope::Process(float* sample, float attackMod, float decayMod, float sustainMod, float releaseMod, bool fullFunctionality)
    #else
    void Envelope::Process(float* sample, float attackMod, float decayMod, float sustainMod, float releaseMod)
    #endif // __FUNCTIONALITY_OPTION__
    {
        #ifdef __FUNCTIONALITY_OPTION__
        if (fullFunctionality) {
        #endif // __FUNCTIONALITY_OPTION__
            if (attackMod != prevAttackMod) {
                env.SetAttackTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_ATTACK + envNumber)) + attackMod), 0.0f) * 3.0f);
                prevAttackMod = attackMod;
            }

            if (decayMod != prevDecayMod) {
                env.SetDecayTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_DECAY + envNumber)) + prevDecayMod), 0.0f) * 3.0f);
                prevDecayMod = decayMod;
            }

            if (sustainMod != prevSustainMod) {
                env.SetSustainLevel(fclamp(patchSettings->getFloatValue((PatchSetting)(ENV_1_SUSTAIN + envNumber)) + prevSustainMod, 0.0f, 1.0f));
                prevSustainMod = sustainMod;
            }

            if (releaseMod != prevReleaseMod) {
                if (!quickRelease) {
                    env.SetReleaseTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_RELEASE + envNumber)) + prevReleaseMod), 0.0f) * 3.0f);
                }
                prevReleaseMod = releaseMod;
            }
        #ifdef __FUNCTIONALITY_OPTION__
        }
        #endif // __FUNCTIONALITY_OPTION__

        *sample = *sample * env.Process(noteTriggered);
        if (reversed) {
            *sample = 1.0f - *sample;
            if (!env.IsRunning()) { // We have to still process it first to keep it moving, despite *sample getting overwritten.
                *sample = 0.0f;
            }
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
    }

    bool Envelope::IsReleasing()
    {
        return env.GetCurrentSegment() == ADSR_SEG_RELEASE;
    }

    void Envelope::SetQuickRelease(bool quickRelease)
    {
        this->quickRelease = quickRelease;
        if (quickRelease) {
            env.SetReleaseTime(0.0015f);
        } else {
            env.SetReleaseTime(releaseValue);
        }
    }
}
