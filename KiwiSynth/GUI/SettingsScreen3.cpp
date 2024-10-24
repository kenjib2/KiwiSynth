#include "SettingsScreen3.h"

namespace kiwi_synth
{

    void SettingsScreen3::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        voiceNumber = 0;
    }

    void SettingsScreen3::Display()
    {
        int intVal1, intVal2;
        char charVal1[12];
        bool boolVal1;
        PatchSettings* settings;
        VoiceMode voiceMode;

        voiceMode = (VoiceMode)patch->voice1Settings->getIntValue(OSC_VOICES);
        if (voiceNumber == 0 || (voiceMode != VOICE_MODE_MULTI && voiceMode != VOICE_MODE_SPLIT)) {
            settings = patch->voice1Settings;
        } else {
            settings = patch->voice2Settings;
        }

        display->Fill(false);

        display->SetCursor(0, 0);
        if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
            sprintf(buffer, "Amplifier--------- V%d", voiceNumber + 1);
        } else {
            sprintf(buffer, "Amplifier------------");
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        intVal1 = settings->getFloatValue(AMP_LEVEL) * 1000;
        intVal2 = settings->getFloatValue(AMP_ENV_1_DEPTH) * 1000;
        sprintf(buffer, "Level %03d    Env1 %03d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        sprintf(buffer, "General--------------");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        EnumToText::GetVoiceMode(charVal1, voiceMode);
        sprintf(buffer, "Mode: %s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        intVal1 = (int)(settings->getFloatValue(GEN_BALANCE) * 1000.f);
        intVal2 = (int)(settings->getFloatValue(OSC_MASTER_TUNE) * 1000.f);
        sprintf(buffer, "Balance %03d  Tune %03d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        boolVal1 = settings->getBoolValue(OSC_PORTAMENTO_ON);
        intVal1 = (int)(settings->getFloatValue(OSC_PORTAMENTO_SPEED) * 1000.f);
        if (boolVal1) {
            sprintf(buffer, "Portamento On  Rt %03d", intVal1);
        } else {
            sprintf(buffer, "Portamento Off");
        }
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

    void SettingsScreen3::Click() {
        if (patch->GetVoiceMode() == VOICE_MODE_MULTI || patch->GetVoiceMode() == VOICE_MODE_SPLIT) {
            voiceNumber = voiceNumber ^ 1;
        } else {
            voiceNumber = 0;
        }
    }

} // namespace kiwi_synth
