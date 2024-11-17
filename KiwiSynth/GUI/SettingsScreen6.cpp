#include "SettingsScreen6.h"

namespace kiwisynth
{

    void SettingsScreen6::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        selected = SETTINGS_SCREEN_6_NONE;
    }

    void SettingsScreen6::Display()
    {
        int intVal1;
        char modSign;
        char charVal1[12];
        PatchSettings* settings;

        // Modulation settings always come from voice1Settings
        settings = patch->voice1Settings;

        display->Fill(false);

        display->SetCursor(0, 0);
        EnumToText::GetModSource(charVal1, (ModulationSource)settings->getIntValue(MOD_1_SOURCE));
        sprintf(buffer, "Src1 %s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(66, 0);
        EnumToText::GetModDestination(charVal1, (ModulationDestination)settings->getIntValue(MOD_1_DESTINATION));
        sprintf(buffer, "Dst1 %s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        intVal1 = std::fabs(settings->getFloatValue(MOD_1_DEPTH) * 1000);
        modSign = ' ';
        if (settings->modSigns[0] < 0.0f) {
            modSign = '-';
        }
        sprintf(buffer, "Depth1 %c%3d", modSign, intVal1);
        display->WriteString(buffer, Font_6x8, selected != SETTINGS_SCREEN_6_SRC1);

        display->SetCursor(0, 16);
        EnumToText::GetModSource(charVal1, (ModulationSource)settings->getIntValue(MOD_2_SOURCE));
        sprintf(buffer, "Src2 %s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(66, 16);
        EnumToText::GetModDestination(charVal1, (ModulationDestination)settings->getIntValue(MOD_2_DESTINATION));
        sprintf(buffer, "Dst2 %s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        intVal1 = std::fabs(settings->getFloatValue(MOD_2_DEPTH) * 1000);
        modSign = ' ';
        if (settings->modSigns[1] < 0.0f) {
            modSign = '-';
        }
        sprintf(buffer, "Depth2 %c%3d", modSign, intVal1);
        display->WriteString(buffer, Font_6x8, selected != SETTINGS_SCREEN_6_SRC2);

        display->SetCursor(0, 32);
        EnumToText::GetModSource(charVal1, (ModulationSource)settings->getIntValue(MOD_3_SOURCE));
        sprintf(buffer, "Src3 %s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(66, 32);
        EnumToText::GetModDestination(charVal1, (ModulationDestination)settings->getIntValue(MOD_3_DESTINATION));
        sprintf(buffer, "Dst3 %s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        intVal1 = std::fabs(settings->getFloatValue(MOD_3_DEPTH) * 1000);
        modSign = ' ';
        if (settings->modSigns[2] < 0.0f) {
            modSign = '-';
        }
        sprintf(buffer, "Depth3 %c%3d", modSign, intVal1);
        display->WriteString(buffer, Font_6x8, selected != SETTINGS_SCREEN_6_SRC3);

        display->SetCursor(0, 48);
        EnumToText::GetModSource(charVal1, (ModulationSource)settings->getIntValue(MOD_4_SOURCE));
        sprintf(buffer, "Src4 %s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(66, 48);
        EnumToText::GetModDestination(charVal1, (ModulationDestination)settings->getIntValue(MOD_4_DESTINATION));
        sprintf(buffer, "Dst4 %s", charVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        intVal1 = std::fabs(settings->getFloatValue(MOD_4_DEPTH) * 1000);
        modSign = ' ';
        if (settings->modSigns[3] < 0.0f) {
            modSign = '-';
        }
        sprintf(buffer, "Depth4 %c%3d", modSign, intVal1);
        display->WriteString(buffer, Font_6x8, selected != SETTINGS_SCREEN_6_SRC4);

        if (selected != SETTINGS_SCREEN_6_NONE) {
            display->SetCursor(102, 56);
            sprintf(buffer, "<--");
            display->WriteString(buffer, Font_6x8, selected != SETTINGS_SCREEN_6_RETURN);
        }

        display->Update();
    }

    void SettingsScreen6::Increment() {
        selected = (SettingsScreen6Selection)((selected + 1) % SETTINGS_SCREEN_6_OPTIONS);
    }

    void SettingsScreen6::Decrement() {
        selected = (SettingsScreen6Selection)((selected - 1 + SETTINGS_SCREEN_6_OPTIONS) % SETTINGS_SCREEN_6_OPTIONS);
    }

    SettingsScreen6Response SettingsScreen6::Click() {
        switch (selected) {
            case SETTINGS_SCREEN_6_NONE:
                selected = SETTINGS_SCREEN_6_SRC1;
                return SETTINGS_SCREEN_6_RESPONSE_EDIT;

            case SETTINGS_SCREEN_6_SRC1:
                patch->voice1Settings->modSigns[0] *= -1.0f;
                return SETTINGS_SCREEN_6_RESPONSE_EDIT;

            case SETTINGS_SCREEN_6_SRC2:
                patch->voice1Settings->modSigns[1] *= -1.0f;
                return SETTINGS_SCREEN_6_RESPONSE_EDIT;

            case SETTINGS_SCREEN_6_SRC3:
                patch->voice1Settings->modSigns[2] *= -1.0f;
                return SETTINGS_SCREEN_6_RESPONSE_EDIT;

            case SETTINGS_SCREEN_6_SRC4:
                patch->voice1Settings->modSigns[3] *= -1.0f;
                return SETTINGS_SCREEN_6_RESPONSE_EDIT;

            default:
            case SETTINGS_SCREEN_6_RETURN:
                selected = SETTINGS_SCREEN_6_NONE;
                return SETTINGS_SCREEN_6_RESPONSE_NOEDIT;
        }
    }

} // namespace kiwisynth
