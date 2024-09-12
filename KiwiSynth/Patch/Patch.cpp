#include "Patch.h"

namespace kiwi_synth
{
    void Patch::Init(PatchSettings* activeSettings, MultiPots* multiPots, GpioExpansion* ge)
    {
        this->activeSettings = activeSettings;
        this->multiPots = multiPots;
        this->ge = ge;

        multiPots->RegisterControlListener(activeSettings);
        ge->RegisterControlListener(activeSettings);
        //patchSettings[0].Init(multiPots, ge);
        //patchSettings[1].Init(multiPots, ge);
        //SetActivePatchSettings(0);
    }

    /*void Patch::SetActivePatchSettings(int voiceNumber) {
        activeSettings = &(patchSettings[voiceNumber]);
        multiPots->RegisterControlListener(activeSettings);
        ge->RegisterControlListener(activeSettings);
    }*/

    PatchSettings* Patch::getActiveSettings()
    {
        return activeSettings;
    }

    void Patch::UpdateSettings()
    {
        voiceMode = (VoiceMode)activeSettings->getIntValue(VCO_VOICES);
        if (voice1Settings != activeSettings) {
            voice1Settings->setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        if (voice2Settings != activeSettings) {
            voice2Settings->setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        #ifdef __FUNCTIONALITY_OPTION__
        if (voice3Settings != activeSettings) {
            voice3Settings->setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        #endif // __FUNCTIONALITY_OPTION__
    }

}
