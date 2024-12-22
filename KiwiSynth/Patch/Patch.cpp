#include "Patch.h"



using namespace kiwisynth;



void Patch::Init(MultiPots* multiPots, GpioExpansion* ge)
{
    multiPots_ = multiPots;
    ge_ = ge;

    settings1_.Init(multiPots, ge);
    settings2_.Init(multiPots, ge);

    isLiveModeActive_ = true;
    bankNumber_ = -1;
    patchNumber_ = -1;
    SetVoiceMode(VOICE_MODE_POLY);
    strcpy(name_, "New            ");
    effectsMode_ = FX_DISTORTION_DELAY;
    reverbMode_ = REVERB_HALL;
    splitNote_ = 60;

    voice1Settings_ = &settings1_;
    voice2Settings_ = &settings1_;
    multiPots->RegisterControlListener(voice1Settings_);
    ge->RegisterControlListener(voice1Settings_);
}



void Patch::SetVoiceMode(VoiceMode voiceMode) {
    VoiceMode calculatedVoiceMode = voiceMode;
    if (calculatedVoiceMode < 0) calculatedVoiceMode = (VoiceMode)(calculatedVoiceMode + VOICE_MODE_MAX);
        else calculatedVoiceMode = (VoiceMode)(calculatedVoiceMode % VOICE_MODE_MAX);

    if ((calculatedVoiceMode == VOICE_MODE_MULTI && voiceMode_ != VOICE_MODE_SPLIT) ||
        (calculatedVoiceMode == VOICE_MODE_SPLIT && voiceMode_ != VOICE_MODE_MULTI)) {
        settings2_.Copy(&settings1_);
        voice2Settings_ = &settings2_;
    } else if ((calculatedVoiceMode == VOICE_MODE_SPLIT && voiceMode_ == VOICE_MODE_MULTI) ||
                (calculatedVoiceMode == VOICE_MODE_MULTI && voiceMode_ == VOICE_MODE_SPLIT)) {
        // Do nothing when switching between MULTI and SPLIT. Only NoteOn and NoteOff change.
    } else {
        voice2Settings_ = &settings1_;
    }

    voice1Settings_->setValue(OSC_VOICES, (int8_t)calculatedVoiceMode);
    voiceMode_ = calculatedVoiceMode;
}



void Patch::DefaultSettings() {
    voiceMode_ = VOICE_MODE_POLY;
    splitNote_ = 60;
    strcpy(name_, "New            ");
    type_ = PATCH_INIT;
    effectsMode_ = FX_DISTORTION_DELAY;
    reverbMode_ = REVERB_HALL;

    settings1_.DefaultSettings();
    settings2_.DefaultSettings();
}



void Patch::SetLiveMode(bool isLive, int bankNumber, int patchNumber) {
    if (!isLiveModeActive_ && isLive) {
        voice2Settings_ = &settings1_;
        settings1_.SetControlsLive(true);
        settings2_.SetControlsLive(true);
        DefaultSettings();
        ge_->Process(true); // Due to only reading on change triggered interrupts, we need to force a full read
    } else if (!isLive) {
        settings1_.SetControlsLive(false);
        settings2_.SetControlsLive(false);
    }

    isLiveModeActive_ = isLive;
    bankNumber_ = bankNumber;
    patchNumber_ = patchNumber;
}



void Patch::ReadFromSavedPatch(SavedPatch savedPatch) {
    voiceMode_ = savedPatch.voiceMode;
    if (voiceMode_ == VOICE_MODE_MULTI || voiceMode_ == VOICE_MODE_SPLIT) {
        voice2Settings_ = &settings2_;
    } else {
        voice2Settings_ = &settings1_;
    }
    splitNote_ = savedPatch.splitNote;
    strcpy(name_, savedPatch.name);
    type_ = savedPatch.type;
    effectsMode_ = savedPatch.effectsMode;
    reverbMode_ = savedPatch.reverbMode;

    settings1_.Copy(&savedPatch, 0);
    settings2_.Copy(&savedPatch, 1);
}



void Patch::WriteToSavedPatch(SavedPatch* savedPatch) {
    savedPatch->voiceMode = voiceMode_;
    savedPatch->splitNote = splitNote_;
    strcpy(savedPatch->name, name_);
    savedPatch->type = type_;
    savedPatch->effectsMode = effectsMode_;
    savedPatch->reverbMode = reverbMode_;

    settings1_.Load(savedPatch, 0);
    settings2_.Load(savedPatch, 1);
}



void Patch::GetPatchHeader(PatchHeader* patchHeader) {
    strcpy(patchHeader->name, name_);
    patchHeader->bankNumber = bankNumber_;
    patchHeader->patchNumber = patchNumber_;
    patchHeader->type = type_;
    patchHeader->voiceMode = voiceMode_;
}



void Patch::SwapLayers() {
    if (voiceMode_ == VOICE_MODE_SPLIT || voiceMode_ ==  VOICE_MODE_MULTI) {
        settings1_.SetControlsLive(false);
        settings2_.SetControlsLive(false);

        // voice1Settings is used for modulation, fx, and system control settings exclusively, so when
        // we swap them we need to only copy the other settings.
        PatchSettings temp;
        temp.CopyVoiceSettings(voice1Settings_);
        voice1Settings_->CopyVoiceSettings(voice2Settings_);
        voice2Settings_->CopyVoiceSettings(&temp);
    }
}
