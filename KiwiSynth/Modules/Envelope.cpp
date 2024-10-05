#include "Envelope.h"

namespace kiwi_synth
{
    void Envelope::Init(float sampleRate, uint8_t envNumber)
    {
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

    void Envelope::UpdateSettings(PatchSettings* patchSettings)
    {
        if (!quickRelease) {
            env.SetAttackTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_ATTACK + envNumber)) + prevAttackMod), 0.0f) * 4.0f);
            env.SetDecayTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_DECAY + envNumber)) + prevDecayMod), 0.0f) * 3.0f);
            env.SetSustainLevel(fclamp(patchSettings->getFloatValue((PatchSetting)(ENV_1_SUSTAIN + envNumber)) + prevSustainMod, 0.0f, 1.0f));
            releaseValue = std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_RELEASE + envNumber)) + prevReleaseMod), 0.0f) * 3.0f;
            reversed = patchSettings->getBoolValue((PatchSetting)(ENV_1_REVERSE_PHASE_ON + envNumber));

            env.SetReleaseTime(releaseValue);
        }
    }

    void Envelope::Process(float* sample, PatchSettings* patchSettings, float attackMod, float decayMod, float sustainMod, float releaseMod)
    {
        if (__builtin_expect(attackMod != prevAttackMod, 1)) {
            env.SetAttackTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_ATTACK + envNumber)) + attackMod), 0.0f) * 3.0f);
            prevAttackMod = attackMod;
        }

        if (__builtin_expect(decayMod != prevDecayMod, 1)) {
            env.SetDecayTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_DECAY + envNumber)) + prevDecayMod), 0.0f) * 3.0f);
            prevDecayMod = decayMod;
        }

        if (__builtin_expect(sustainMod != prevSustainMod, 1)) {
            env.SetSustainLevel(fclamp(patchSettings->getFloatValue((PatchSetting)(ENV_1_SUSTAIN + envNumber)) + prevSustainMod, 0.0f, 1.0f));
            prevSustainMod = sustainMod;
        }

        if (__builtin_expect(releaseMod != prevReleaseMod, 1)) {
            if (!quickRelease) {
                env.SetReleaseTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_RELEASE + envNumber)) + prevReleaseMod), 0.0f) * 3.0f);
            }
            prevReleaseMod = releaseMod;
        }

        *sample = *sample * env.Process(noteTriggered);
        if (__builtin_expect(reversed, 0)) {
            *sample = 1.0f - *sample;
            if (!env.IsRunning()) { // We have to still process it first to keep it moving, despite *sample getting overwritten.
                *sample = 0.0f;
            }
        }
    }
}
