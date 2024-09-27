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
        SetVoiceMode(VOICE_MODE_POLY);
        strcpy(name, "Kiwi Sound");
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
        type = PATCH_SYNTH;
        effectsMode = FX_DISTORTION_DELAY;
        reverbMode = REVERB_HALL;

        settings1.DefaultSettings();
        settings2.DefaultSettings();
    }

    void Patch::SetLiveMode(bool liveModeActive) {
        liveMode = liveModeActive;
        if (liveModeActive) {
            DefaultSettings();
        }
    }

    void Patch::Load(SavedPatch savedPatch) {
        voiceMode = savedPatch.voiceMode;
        splitNote = savedPatch.splitNote;
        strcpy(name, savedPatch.name);
        type = savedPatch.type;
        effectsMode = savedPatch.effectsMode;
        reverbMode = savedPatch.reverbMode;

        settings1.Copy((bool*)savedPatch.voice1BoolValues, (int*)savedPatch.voice1IntValues, (float*)savedPatch.voice1FloatValues);
        settings2.Copy((bool*)savedPatch.voice2BoolValues, (int*)savedPatch.voice2IntValues, (float*)savedPatch.voice2FloatValues);
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

} // namespace kiwi_synth
