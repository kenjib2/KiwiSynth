#include "Patch.h"

namespace kiwi_synth
{
    #ifdef __FUNCTIONALITY_OPTION__
    void Patch::Init(PatchSettings* voice1Settings, PatchSettings* voice2Settings, PatchSettings* voice3Settings, MultiPots* multiPots, GpioExpansion* ge)
    #else
    void Patch::Init(PatchSettings* voice1Settings, PatchSettings* voice2Settings, MultiPots* multiPots, GpioExpansion* ge)
    #endif // __FUNCTIONALITY_OPTION__
    {
        this->multiPots = multiPots;
        this->ge = ge;
        this->voice1Settings = voice1Settings;
        this->voice2Settings = voice2Settings;
        #ifdef __FUNCTIONALITY_OPTION__
        this->voice3Settings = voice3Settings;
        #endif // __FUNCTIONALITY_OPTION__

        SetActivePatchSettings(0);
    }

    void Patch::SetActivePatchSettings(int voiceNumber) {
        switch(voiceNumber) {
            case (0):
                activeSettings = &voice1Settings;
                break;
            case (1):
                activeSettings = &voice2Settings;
                break;
            #ifdef __FUNCTIONALITY_OPTION__
            case (2):
                activeSettings = &voice2Settings;
                break;
            #endif // __FUNCTIONALITY_OPTION__
        }
        multiPots->RegisterControlListener(*activeSettings);
        ge->RegisterControlListener(*activeSettings);
    }

    void Patch::UpdateSettings()
    {
        voiceMode = (VoiceMode)voice1Settings->getIntValue(VCO_VOICES);
        if (voice1Settings != *activeSettings) {
            voice1Settings->setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        if (voice2Settings != *activeSettings) {
            voice2Settings->setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        #ifdef __FUNCTIONALITY_OPTION__
        if (voice3Settings != *activeSettings) {
            voice3Settings->setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        #endif // __FUNCTIONALITY_OPTION__
    }

}
