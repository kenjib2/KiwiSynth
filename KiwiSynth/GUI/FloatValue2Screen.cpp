#include "FloatValue2Screen.h"

namespace kiwi_synth
{

    void FloatValue2Screen::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        this->voiceNumber = 0;
    }

    void FloatValue2Screen::Display()
    {
        int intVal1, intVal2, intVal3;
        PatchSettings* settings;
        PatchSettings* modSettings;
        VoiceMode voiceMode;

        if (patch->GetLiveMode()) {
            voiceMode = (VoiceMode)patch->activeSettings->getIntValue(VCO_VOICES);
            modSettings = patch->voice1Settings;
            if (voiceNumber == 0 || (voiceMode != VOICE_MODE_MULTI && voiceMode != VOICE_MODE_SPLIT)) {
                settings = patch->voice1Settings;
            } else {
                settings = patch->voice2Settings;
            }
        } else {
            voiceMode = (VoiceMode)patch->loadedPatchSettings1.getIntValue(VCO_VOICES);
            modSettings = &(patch->loadedPatchSettings1);
            if (voiceNumber == 0 || (voiceMode != VOICE_MODE_MULTI && voiceMode != VOICE_MODE_SPLIT)) {
                settings = &(patch->loadedPatchSettings1);
            } else {
                settings = &(patch->loadedPatchSettings2);
            }
        }

        display->Fill(false);

        display->SetCursor(0, 0);
        sprintf(buffer, "VCF                V%d", voiceNumber + 1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        intVal1 = settings->getFloatValue(VCF_CUTOFF) * 1000;
        intVal2 = settings->getFloatValue(VCF_RESONANCE) * 1000;
        sprintf(buffer, "FreqCut %03d  Reso %03d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        intVal1 = settings->getFloatValue(VCF_TRACKING) * 1000;
        intVal2 = settings->getFloatValue(VCF_ENV_1_DEPTH) * 1000;
        intVal3 = settings->getFloatValue(VCF_ENV_2_DEPTH) * 1000;
        sprintf(buffer, "KT %03d  E1 %03d E2 %03d", intVal1, intVal2, intVal3);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        sprintf(buffer, "VCA");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        intVal1 = settings->getFloatValue(VCA_LEVEL) * 1000;
        intVal2 = settings->getFloatValue(VCA_ENV_1_DEPTH) * 1000;
        sprintf(buffer, "Level %03d    Env1 %03d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        intVal1 = settings->getFloatValue(SH_TO_VCF_CUTOFF) * 1000;
        intVal2 = settings->getFloatValue(SH_RATE) * 1000;
        sprintf(buffer, "SH  FrCt %03d Rate %03d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        intVal1 = modSettings->getFloatValue(MOD_1_DEPTH) * 1000;
        sprintf(buffer, "Modulation     D1 %03d", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        intVal1 = modSettings->getFloatValue(MOD_2_DEPTH) * 1000;
        intVal2 = modSettings->getFloatValue(MOD_3_DEPTH) * 1000;
        intVal3 = modSettings->getFloatValue(MOD_4_DEPTH) * 1000;
        sprintf(buffer, "D2  %03d D3 %03d D4 %03d", intVal1, intVal2, intVal3);
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

    void FloatValue2Screen::Click() {
        voiceNumber = voiceNumber ^ 1;
    }

} // namespace kiwi_synth
