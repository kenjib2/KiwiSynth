#include "SettingsScreen4.h"

namespace kiwi_synth
{

    void SettingsScreen4::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        voiceNumber = 0;
    }

    void SettingsScreen4::Display()
    {
        int intVal1, intVal2;
        char charVal1[22], charVal2[12];
        PatchSettings* settings;

        // Effects settings always come from voice1Settings
        settings = patch->voice1Settings;

        display->Fill(false);

        display->SetCursor(0, 0);
        EnumToText::GetEffect1(charVal1, patch->GetEffectsMode());
        sprintf(buffer, "%s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        EnumToText::GetEffectSetting(charVal1, patch->GetEffectsMode(), 1);
        EnumToText::GetEffectSetting(charVal2, patch->GetEffectsMode(), 2);
        intVal1 = settings->getFloatValue(FX_1) * 1000;
        intVal2 = settings->getFloatValue(FX_2) * 1000;
        sprintf(buffer, "%s %03d %s %03d", charVal1, intVal1, charVal2, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        EnumToText::GetEffect2(charVal1, patch->GetEffectsMode());
        sprintf(buffer, "%s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        EnumToText::GetEffectSetting(charVal1, patch->GetEffectsMode(), 3);
        EnumToText::GetEffectSetting(charVal2, patch->GetEffectsMode(), 4);
        intVal1 = settings->getFloatValue(FX_3) * 1000;
        intVal2 = settings->getFloatValue(FX_4) * 1000;
        sprintf(buffer, "%s %03d %s %03d", charVal1, intVal1, charVal2, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        EnumToText::GetEffectSetting(charVal1, patch->GetEffectsMode(), 5);
        intVal1 = settings->getFloatValue(FX_5) * 1000;
        sprintf(buffer, "%s %03d", charVal1, intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        EnumToText::GetReverbModePadded(charVal1, patch->GetReverbMode());
        sprintf(buffer, "%s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        EnumToText::GetReverbModePadded(charVal1, patch->GetReverbMode());
        intVal1 = settings->getFloatValue(FX_REVERB) * 1000;
        sprintf(buffer, "Amount %03d", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

} // namespace kiwi_synth
