#include "SettingsScreen7.h"

namespace kiwisynth
{

    void SettingsScreen7::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        selected = SETTINGS_SCREEN_7_NONE;
    }

    void SettingsScreen7::Display()
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
        display->WriteString(buffer, Font_6x8, selected != SETTINGS_SCREEN_7_EFFECTS);

        display->SetCursor(0, 8);
        EnumToText::GetEffectSetting(charVal1, patch->GetEffectsMode(), 1);
        EnumToText::GetEffectSetting(charVal2, patch->GetEffectsMode(), 2);
        intVal1 = settings->getFloatValue(FX_1) * 1000;
        intVal2 = settings->getFloatValue(FX_2) * 1000;
        sprintf(buffer, "%s %3d %s %3d", charVal1, intVal1, charVal2, intVal2);
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
        sprintf(buffer, "%s %3d %s %3d", charVal1, intVal1, charVal2, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        EnumToText::GetEffectSetting(charVal1, patch->GetEffectsMode(), 5);
        intVal1 = settings->getFloatValue(FX_5) * 1000;
        sprintf(buffer, "%s %3d", charVal1, intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        EnumToText::GetReverbModePadded(charVal1, patch->GetReverbMode());
        sprintf(buffer, "%s", charVal1);
        display->WriteString(buffer, Font_6x8, selected != SETTINGS_SCREEN_7_REVERB);

        display->SetCursor(0, 56);
        EnumToText::GetReverbModePadded(charVal1, patch->GetReverbMode());
        intVal1 = settings->getFloatValue(FX_REVERB) * 1000;
        sprintf(buffer, "Amount %3d", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        if (selected != SETTINGS_SCREEN_7_NONE) {
            display->SetCursor(102, 56);
            sprintf(buffer, "<--");
            display->WriteString(buffer, Font_6x8, selected != SETTINGS_SCREEN_7_RETURN);
        }

        display->Update();
    }

    void SettingsScreen7::Increment() {
        selected = (SettingsScreen7Selection)((selected + 1) % SETTINGS_SCREEN_7_OPTIONS);
    }

    void SettingsScreen7::Decrement() {
        selected = (SettingsScreen7Selection)((selected - 1 + SETTINGS_SCREEN_7_OPTIONS) % SETTINGS_SCREEN_7_OPTIONS);
    }

    SettingsScreen7Response SettingsScreen7::Click() {
        switch (selected) {
            case SETTINGS_SCREEN_7_NONE:
                selected = SETTINGS_SCREEN_7_EFFECTS;
                return SETTINGS_SCREEN_7_RESPONSE_EDIT;

            case SETTINGS_SCREEN_7_EFFECTS:
                patch->SetEffectsMode((EffectsMode)(patch->GetEffectsMode() + 1)); // This function handles wrapping around overflow
                return SETTINGS_SCREEN_7_RESPONSE_EDIT;

            case SETTINGS_SCREEN_7_REVERB:
                patch->SetReverbMode((ReverbMode)(patch->GetReverbMode() + 1)); // This function handles wrapping around overflow
                return SETTINGS_SCREEN_7_RESPONSE_EDIT;

            default:
            case SETTINGS_SCREEN_7_RETURN:
                selected = SETTINGS_SCREEN_7_NONE;
                return SETTINGS_SCREEN_7_RESPONSE_NOEDIT;
        }
    }

} // namespace kiwisynth
