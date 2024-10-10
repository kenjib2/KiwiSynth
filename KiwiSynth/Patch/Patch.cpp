#include "Patch.h"

namespace kiwi_synth
{
    void Patch::Init(MultiPots* multiPots, GpioExpansion* ge)
    {
        this->multiPots = multiPots;
        this->ge = ge;

        settings1.Init(multiPots, ge);
        settings2.Init(multiPots, ge);

        liveMode = true;
        bankNumber = -1;
        patchNumber = -1;
        SetVoiceMode(VOICE_MODE_POLY);
        strcpy(name, "New            ");
        effectsMode = FX_DISTORTION_DELAY;
        reverbMode = REVERB_HALL;
        splitNote = 60;

        activeSettings = &settings1;
        voice1Settings = &settings1;
        voice2Settings = &settings1;
        multiPots->RegisterControlListener(activeSettings);
        ge->RegisterControlListener(activeSettings);
    }

    void Patch::SetVoiceMode(VoiceMode voiceMode) {
        VoiceMode calculatedVoiceMode = voiceMode;
        if (calculatedVoiceMode < 0) calculatedVoiceMode = (VoiceMode)(calculatedVoiceMode + VOICE_MODE_MAX);
            else calculatedVoiceMode = (VoiceMode)(calculatedVoiceMode % VOICE_MODE_MAX);

        if ((calculatedVoiceMode == VOICE_MODE_MULTI && this->voiceMode != VOICE_MODE_SPLIT) ||
            (calculatedVoiceMode == VOICE_MODE_SPLIT && this->voiceMode != VOICE_MODE_MULTI)) {
            settings2.Copy(&settings1);
            voice2Settings = &settings2;
        } else if ((calculatedVoiceMode == VOICE_MODE_SPLIT && this->voiceMode == VOICE_MODE_MULTI) ||
                   (calculatedVoiceMode == VOICE_MODE_MULTI && this->voiceMode == VOICE_MODE_SPLIT)) {
            // Do nothing when switching between MULTI and SPLIT. Only NoteOn and NoteOff change.
        } else {
            voice2Settings = &settings1;
        }

        activeSettings->setValue(VCO_VOICES, (int8_t)calculatedVoiceMode);
        this->voiceMode = calculatedVoiceMode;
    }

    void Patch::DefaultSettings() {
        voiceMode = VOICE_MODE_POLY;
        splitNote = 60;
        strcpy(name, "New            ");
        type = PATCH_INIT;
        effectsMode = FX_DISTORTION_DELAY;
        reverbMode = REVERB_HALL;

        settings1.DefaultSettings();
        settings2.DefaultSettings();
    }

    void Patch::SetLiveMode(bool liveModeActive, int bankNumber, int patchNumber) {
        if (!liveMode && liveModeActive) {
            DefaultSettings();
        } else if (liveMode && !liveModeActive) {
            voice1Settings = &loadedPatchSettings1;
            voice2Settings = &loadedPatchSettings2;
        }

        liveMode = liveModeActive;
        this->bankNumber = bankNumber;
        this->patchNumber = patchNumber;
    }

    void Patch::Load(SavedPatch savedPatch) {
        // Saving a copy of the loaded data in case we want to re-save the same patch
        memcpy(&loadedPatchData, &savedPatch, sizeof(SavedPatch));

        voiceMode = savedPatch.voiceMode;
        if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
            voice2Settings = &settings2;
        } else {
            voice2Settings = &settings1;
        }
        splitNote = savedPatch.splitNote;
        strcpy(name, savedPatch.name);
        type = savedPatch.type;
        effectsMode = savedPatch.effectsMode;
        reverbMode = savedPatch.reverbMode;

        settings1.Copy(&savedPatch, 0);
        settings2.Copy(&savedPatch, 1);
        loadedPatchSettings1.Copy(&savedPatch, 0);
        loadedPatchSettings2.Copy(&savedPatch, 1);
    }

    void Patch::Save(SavedPatch* savedPatch) {
        savedPatch->voiceMode = voiceMode;
        savedPatch->splitNote = splitNote;
        strcpy(savedPatch->name, name);
        savedPatch->type = type;
        savedPatch->effectsMode = effectsMode;
        savedPatch->reverbMode = reverbMode;

        settings1.Load(savedPatch, 0);
        settings2.Load(savedPatch, 1);
    }

    PatchHeader Patch::GetPatchHeader() {
        PatchHeader patchHeader;
        strcpy(patchHeader.name, name);
        patchHeader.bankNumber = bankNumber;
        patchHeader.patchNumber = patchNumber;
        patchHeader.type = type;
        patchHeader.voiceMode = voiceMode;

        return patchHeader;
    }

} // namespace kiwi_synth
