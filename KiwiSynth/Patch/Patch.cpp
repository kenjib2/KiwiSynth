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
    }

    void Patch::SetVoiceMode(VoiceMode voiceMode) {
        if (voiceMode == VOICE_MODE_MULTI) {
            voice1Settings = &settings1;
            voice2Settings = &settings2;
            activeSettings = &settings2;
        } else {
            voice1Settings = &settings1;
            voice2Settings = &settings1;
            activeSettings = &settings1;
        }
        multiPots->RegisterControlListener(activeSettings);
        ge->RegisterControlListener(activeSettings);
    }

    void Patch::UpdateSettings()
    {
        voiceMode = (VoiceMode)getActiveSettings()->getIntValue(VCO_VOICES);
    }

}
