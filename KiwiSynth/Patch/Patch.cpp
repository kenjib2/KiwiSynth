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

}
