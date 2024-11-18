#include "SettingsScreen6.h"



using namespace kiwisynth;



void SettingsScreen6::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
    selected_ = SETTINGS_SCREEN_6_NONE;
}



void SettingsScreen6::Display()
{
    int intVal1;
    char modSign;
    char charVal1[12];
    PatchSettings* settings;

    // Modulation settings always come from voice1Settings
    settings = patch_->voice1Settings_;

    display_->Fill(false);

    display_->SetCursor(0, 0);
    EnumToText::GetModSource(charVal1, (ModulationSource)settings->getIntValue(MOD_1_SOURCE));
    sprintf(buffer_, "Src1 %s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(66, 0);
    EnumToText::GetModDestination(charVal1, (ModulationDestination)settings->getIntValue(MOD_1_DESTINATION));
    sprintf(buffer_, "Dst1 %s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 8);
    intVal1 = std::fabs(settings->getFloatValue(MOD_1_DEPTH) * 1000);
    modSign = ' ';
    if (settings->modSigns_[0] < 0.0f) {
        modSign = '-';
    }
    sprintf(buffer_, "Depth1 %c%3d", modSign, intVal1);
    display_->WriteString(buffer_, Font_6x8, selected_ != SETTINGS_SCREEN_6_SRC1);

    display_->SetCursor(0, 16);
    EnumToText::GetModSource(charVal1, (ModulationSource)settings->getIntValue(MOD_2_SOURCE));
    sprintf(buffer_, "Src2 %s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(66, 16);
    EnumToText::GetModDestination(charVal1, (ModulationDestination)settings->getIntValue(MOD_2_DESTINATION));
    sprintf(buffer_, "Dst2 %s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 24);
    intVal1 = std::fabs(settings->getFloatValue(MOD_2_DEPTH) * 1000);
    modSign = ' ';
    if (settings->modSigns_[1] < 0.0f) {
        modSign = '-';
    }
    sprintf(buffer_, "Depth2 %c%3d", modSign, intVal1);
    display_->WriteString(buffer_, Font_6x8, selected_ != SETTINGS_SCREEN_6_SRC2);

    display_->SetCursor(0, 32);
    EnumToText::GetModSource(charVal1, (ModulationSource)settings->getIntValue(MOD_3_SOURCE));
    sprintf(buffer_, "Src3 %s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(66, 32);
    EnumToText::GetModDestination(charVal1, (ModulationDestination)settings->getIntValue(MOD_3_DESTINATION));
    sprintf(buffer_, "Dst3 %s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 40);
    intVal1 = std::fabs(settings->getFloatValue(MOD_3_DEPTH) * 1000);
    modSign = ' ';
    if (settings->modSigns_[2] < 0.0f) {
        modSign = '-';
    }
    sprintf(buffer_, "Depth3 %c%3d", modSign, intVal1);
    display_->WriteString(buffer_, Font_6x8, selected_ != SETTINGS_SCREEN_6_SRC3);

    display_->SetCursor(0, 48);
    EnumToText::GetModSource(charVal1, (ModulationSource)settings->getIntValue(MOD_4_SOURCE));
    sprintf(buffer_, "Src4 %s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(66, 48);
    EnumToText::GetModDestination(charVal1, (ModulationDestination)settings->getIntValue(MOD_4_DESTINATION));
    sprintf(buffer_, "Dst4 %s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 56);
    intVal1 = std::fabs(settings->getFloatValue(MOD_4_DEPTH) * 1000);
    modSign = ' ';
    if (settings->modSigns_[3] < 0.0f) {
        modSign = '-';
    }
    sprintf(buffer_, "Depth4 %c%3d", modSign, intVal1);
    display_->WriteString(buffer_, Font_6x8, selected_ != SETTINGS_SCREEN_6_SRC4);

    if (selected_ != SETTINGS_SCREEN_6_NONE) {
        display_->SetCursor(102, 56);
        sprintf(buffer_, "<--");
        display_->WriteString(buffer_, Font_6x8, selected_ != SETTINGS_SCREEN_6_RETURN);
    }

    display_->Update();
}



void SettingsScreen6::Increment() {
    selected_ = (SettingsScreen6Selection)((selected_ + 1) % SETTINGS_SCREEN_6_OPTIONS);
}



void SettingsScreen6::Decrement() {
    selected_ = (SettingsScreen6Selection)((selected_ - 1 + SETTINGS_SCREEN_6_OPTIONS) % SETTINGS_SCREEN_6_OPTIONS);
}



SettingsScreen6Response SettingsScreen6::Click() {
    switch (selected_) {
        case SETTINGS_SCREEN_6_NONE:
            selected_ = SETTINGS_SCREEN_6_SRC1;
            return SETTINGS_SCREEN_6_RESPONSE_EDIT;

        case SETTINGS_SCREEN_6_SRC1:
            patch_->voice1Settings_->modSigns_[0] *= -1.0f;
            return SETTINGS_SCREEN_6_RESPONSE_EDIT;

        case SETTINGS_SCREEN_6_SRC2:
            patch_->voice1Settings_->modSigns_[1] *= -1.0f;
            return SETTINGS_SCREEN_6_RESPONSE_EDIT;

        case SETTINGS_SCREEN_6_SRC3:
            patch_->voice1Settings_->modSigns_[2] *= -1.0f;
            return SETTINGS_SCREEN_6_RESPONSE_EDIT;

        case SETTINGS_SCREEN_6_SRC4:
            patch_->voice1Settings_->modSigns_[3] *= -1.0f;
            return SETTINGS_SCREEN_6_RESPONSE_EDIT;

        default:
        case SETTINGS_SCREEN_6_RETURN:
            selected_ = SETTINGS_SCREEN_6_NONE;
            return SETTINGS_SCREEN_6_RESPONSE_NOEDIT;
    }
}
