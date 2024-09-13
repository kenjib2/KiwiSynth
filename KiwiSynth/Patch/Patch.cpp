#include "Patch.h"

namespace kiwi_synth
{
    void Patch::Init(PatchSettings* settings1, PatchSettings* settings2, MultiPots* multiPots, GpioExpansion* ge)
    {
        this->multiPots = multiPots;
        this->ge = ge;
        this->settings1 = settings1;
        this->settings2 = settings2;

        SetVoiceMode(VOICE_MODE_POLY);
    }

    void Patch::SetVoiceMode(VoiceMode voiceMode) {
        /*if (voiceMode == VOICE_MODE_MULTI) {
            voice1Settings = &settings1;
            voice2Settings = &settings2;
            activeSettings = &settings2;
        } else {*/
            voice1Settings = &settings1;
            voice2Settings = &settings1;
            activeSettings = &settings1;
        //}
        /*switch(voiceMode) {
            case VOICE_MODE_POLY:
            #ifdef __FUNCTIONALITY_OPTION__
            case VOICE_MODE_3V:
            #endif // __FUNCTIONALITY_OPTION__
            case VOICE_MODE_MONO:
                voice1Settings = &settings1;
                voice2Settings = &settings1;
                activeSettings = &settings1;
                break;
            case VOICE_MODE_MULTI:
                voice1Settings = &settings1;
                voice2Settings = &settings2;
                activeSettings = &settings2;
                break;
        }*/
        multiPots->RegisterControlListener(*activeSettings);
        ge->RegisterControlListener(*activeSettings);
    }

    void Patch::UpdateSettings()
    {
        voiceMode = (VoiceMode)getActiveSettings()->getIntValue(VCO_VOICES);
        if (settings1 != *activeSettings) {
            settings1->setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        if (settings2 != *activeSettings) {
            settings2->setValue(VCO_VOICES, (int8_t)voiceMode);
        }
    }

}
