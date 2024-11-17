#include "Envelope.h"

namespace kiwisynth
{
    void Envelope::Init(float sampleRate, uint8_t envNumber)
    {
        envNumber_ = envNumber;
        isNoteTriggered_ = false;
        isQuickRelease_ = false;
        isReversed_ = false;
        env_.Init(sampleRate);

        env_.SetAttackTime(0.002f, 1.0f);
        env_.SetDecayTime(0.0f);
        env_.SetSustainLevel(1.0f);
        env_.SetReleaseTime(0.002f);

        prevAttackMod_ = 0.0f;
        prevDecayMod_ = 0.0f;
        prevSustainMod_ = 0.0f;
        prevReleaseMod_ = 0.0f;
    }

    void Envelope::UpdateSettings(PatchSettings* patchSettings)
    {
        if (!isQuickRelease_) {
            env_.SetAttackTime(std::fmax((patchSettings->getFloatValueExponential((PatchSetting)(ENV_1_ATTACK + envNumber_)) + prevAttackMod_), 0.0f) * 8.0f, 1.0f);
            env_.SetDecayTime(std::fmax((patchSettings->getFloatValueExponential((PatchSetting)(ENV_1_DECAY + envNumber_)) + prevDecayMod_), 0.0f) * 3.0f);
            env_.SetSustainLevel(fclamp(patchSettings->getFloatValue((PatchSetting)(ENV_1_SUSTAIN + envNumber_)) + prevSustainMod_, 0.0f, 1.0f));
            releaseValue_ = std::fmax((patchSettings->getFloatValueExponential((PatchSetting)(ENV_1_RELEASE + envNumber_)) + prevReleaseMod_), 0.0f) * 3.0f;
            isReversed_ = patchSettings->getBoolValue((PatchSetting)(ENV_1_REVERSE_PHASE_ON + envNumber_));

            env_.SetReleaseTime(releaseValue_);
        }
    }

    void Envelope::Process(float* sample, PatchSettings* patchSettings, float attackMod, float decayMod, float sustainMod, float releaseMod)
    {
        if (__builtin_expect(attackMod != prevAttackMod_, 1)) {
            env_.SetAttackTime(std::fmax((patchSettings->getFloatValueExponential((PatchSetting)(ENV_1_ATTACK + envNumber_)) + attackMod), 0.0f) * 8.0f, 1.0f);
            prevAttackMod_ = attackMod;
        }

        if (__builtin_expect(decayMod != prevDecayMod_, 1)) {
            env_.SetDecayTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_DECAY + envNumber_)) + prevDecayMod_), 0.0f) * 3.0f);
            prevDecayMod_ = decayMod;
        }

        if (__builtin_expect(sustainMod != prevSustainMod_, 1)) {
            env_.SetSustainLevel(fclamp(patchSettings->getFloatValue((PatchSetting)(ENV_1_SUSTAIN + envNumber_)) + prevSustainMod_, 0.0f, 1.0f));
            prevSustainMod_ = sustainMod;
        }

        if (__builtin_expect(releaseMod != prevReleaseMod_, 1)) {
            if (!isQuickRelease_) {
                env_.SetReleaseTime(std::fmax((patchSettings->getFloatValue((PatchSetting)(ENV_1_RELEASE + envNumber_)) + prevReleaseMod_), 0.0f) * 3.0f);
            }
            prevReleaseMod_ = releaseMod;
        }

        *sample = *sample * env_.Process(isNoteTriggered_);
        if (__builtin_expect(isReversed_, 0)) {
            *sample = 1.0f - *sample;
            if (!env_.IsRunning()) { // We have to still process it first to keep it moving, despite *sample getting overwritten.
                *sample = 0.0f;
            }
        }
    }

} // namespace kiwisynth
