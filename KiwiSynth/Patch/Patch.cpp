#include "Patch.h"

namespace kiwi_synth
{
    void Patch::Init(MultiPots* multiPots, GpioExpansion* ge)
    {
        patchSettings[0].Init(multiPots, ge);
        patchSettings[1].Init(multiPots, ge);
        //SetActivePatchSettings(0);
    }

    void Patch::SetActivePatchSettings(int voiceNumber) {
        activeSettings = &(patchSettings[voiceNumber]);
        multiPots->RegisterControlListener(activeSettings);
        ge->RegisterControlListener(activeSettings);
    }

    void Patch::UpdateSettings()
    {
        voiceMode = (VoiceMode)activeSettings->getIntValue(VCO_VOICES);
        if (&patchSettings[0] != activeSettings) {
            patchSettings[0].setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        if (&patchSettings[1] != activeSettings) {
            patchSettings[1].setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        #ifdef __FUNCTIONALITY_OPTION__
        if (&patchSettings[2] != activeSettings) {
            patchSettings[2].setValue(VCO_VOICES, (int8_t)voiceMode);
        }
        #endif // __FUNCTIONALITY_OPTION__
    }

}
