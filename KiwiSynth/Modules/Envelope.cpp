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

        env.SetAttackTime(0.002F);
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
        if (envNumber == 0) {
            env.SetAttackTime(std::fmax((patchSettings->getFloatValue(ENV_1_ATTACK) + prevAttackMod), 0.0f) * 3.0f);
            env.SetDecayTime(std::fmax((patchSettings->getFloatValue(ENV_1_DECAY) + prevDecayMod), 0.0f) * 3.0f);
            env.SetSustainLevel(fclamp(patchSettings->getFloatValue(ENV_1_SUSTAIN) + prevSustainMod, 0.0f, 1.0f));
            releaseValue = std::fmax((patchSettings->getFloatValue(ENV_1_RELEASE) + prevReleaseMod), 0.0f) * 3.0F;
            reversed = patchSettings->getBoolValue(ENV_1_REVERSE_PHASE_ON);
        } else { // envNumber == 1
            env.SetAttackTime(std::fmax((patchSettings->getFloatValue(ENV_2_ATTACK) + prevAttackMod), 0.0f) * 3.0f);
            env.SetDecayTime(std::fmax((patchSettings->getFloatValue(ENV_2_DECAY) + prevDecayMod), 0.0f) * 3.0f);
            env.SetSustainLevel(fclamp(patchSettings->getFloatValue(ENV_2_SUSTAIN) + prevSustainMod, 0.0f, 1.0f));
            releaseValue = std::fmax((patchSettings->getFloatValue(ENV_2_RELEASE) + prevReleaseMod), 0.0f) * 3.0F;
            reversed = patchSettings->getBoolValue(ENV_2_REVERSE_PHASE_ON);
        }
        if (!quickRelease) {
            env.SetReleaseTime(releaseValue);
        }
    }

    void Envelope::Process(float* sample, float attackMod, float decayMod, float sustainMod, float releaseMod, bool fullFunctionality)
    {
        if (fullFunctionality) {
            if (attackMod != prevAttackMod) {
                if (envNumber == 0) {
                    env.SetAttackTime(std::fmax((patchSettings->getFloatValue(PatchSetting::ENV_1_ATTACK) + attackMod), 0.0f) * 3.0f);
                } else { // envNumber == 1
                    env.SetAttackTime(std::fmax((patchSettings->getFloatValue(PatchSetting::ENV_2_ATTACK) + attackMod), 0.0f) * 3.0f);
                }
                prevAttackMod = attackMod;
            }

            if (decayMod != prevDecayMod) {
                if (envNumber == 0) {
                    env.SetDecayTime(std::fmax((patchSettings->getFloatValue(ENV_1_DECAY) + prevDecayMod), 0.0f) * 3.0f);
                } else { // envNumber == 1
                    env.SetDecayTime(std::fmax((patchSettings->getFloatValue(ENV_2_DECAY) + prevDecayMod), 0.0f) * 3.0f);
                }
                prevDecayMod = decayMod;
            }

            if (sustainMod != prevSustainMod) {
                if (envNumber == 0) {
                    env.SetSustainLevel(fclamp(patchSettings->getFloatValue(ENV_1_SUSTAIN) + prevSustainMod, 0.0f, 1.0f));
                } else { // envNumber == 1
                    env.SetSustainLevel(fclamp(patchSettings->getFloatValue(ENV_2_SUSTAIN) + prevSustainMod, 0.0f, 1.0f));
                }
                prevSustainMod = sustainMod;
            }

            if (releaseMod != prevReleaseMod) {
                float releaseValue;
                if (envNumber == 0) {
                    releaseValue = std::fmax((patchSettings->getFloatValue(ENV_1_RELEASE) + prevReleaseMod), 0.0f) * 3.0F;
                } else { // envNumber == 1
                    releaseValue = std::fmax((patchSettings->getFloatValue(ENV_2_RELEASE) + prevReleaseMod), 0.0f) * 3.0F;
                }

                if (!quickRelease) {
                    env.SetReleaseTime(releaseValue);
                }
                prevReleaseMod = releaseMod;
            }
        }

        if (reversed) {
            *sample = 1.0F - *sample * env.Process(noteTriggered);
            if (!env.IsRunning()) { // We have to still process it first to keep it moving, despite *sample getting overwritten.
                *sample = 0.0F;
            }
        } else
        {
            *sample = *sample * env.Process(noteTriggered);
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
            env.SetReleaseTime(0.0015F);
        } else {
            env.SetReleaseTime(releaseValue);
        }
    }
}
