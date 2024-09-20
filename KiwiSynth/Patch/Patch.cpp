#include "Patch.h"

namespace kiwi_synth
{
    void Patch::Init(MultiPots* multiPots, GpioExpansion* ge)
    {
        this->multiPots = multiPots;
        this->ge = ge;

        settings1.Init(multiPots, ge);
        settings2.Init(multiPots, ge);

        SetVoiceMode(VOICE_MODE_POLY);
        strcpy(name, "Kiwi Sound");
        effectsMode = FX_DISTORTION_DELAY;
        reverbMode = REVERB_HALL;

        activeSettings = &settings1;
        voice1Settings = &settings1;
        multiPots->RegisterControlListener(activeSettings);
        ge->RegisterControlListener(activeSettings);
    }

    void Patch::SetVoiceMode(VoiceMode voiceMode) {
        if (voiceMode == VOICE_MODE_MULTI) {
            settings2.Copy(&settings1);
            voice2Settings = &settings2;
        } else {
            voice2Settings = &settings1;
        }

        this->voiceMode = voiceMode;
    }

    void Patch::DefaultSettings() {
        voiceMode = VOICE_MODE_POLY;
        splitNote = 0;
        strcpy(name, "New");
        effectsMode = FX_DISTORTION_DELAY;
        reverbMode = REVERB_HALL;

        settings1.DefaultSettings();
        settings2.DefaultSettings();
    }

    void Patch::Load(SavedPatch savedPatch) {
        voiceMode = savedPatch.voiceMode;
        splitNote = savedPatch.splitNote;
        strcpy(name, savedPatch.name);
        effectsMode = savedPatch.effectsMode;
        reverbMode = savedPatch.reverbMode;

        settings1.Copy((bool*)savedPatch.voice1BoolValues, (int*)savedPatch.voice1IntValues, (float*)savedPatch.voice1FloatValues);
        settings2.Copy((bool*)savedPatch.voice2BoolValues, (int*)savedPatch.voice2IntValues, (float*)savedPatch.voice2FloatValues);
    }
}
