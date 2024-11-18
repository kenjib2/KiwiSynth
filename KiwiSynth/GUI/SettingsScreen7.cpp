#include "SettingsScreen7.h"



using namespace kiwisynth;



void SettingsScreen7::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
    selected_ = SETTINGS_SCREEN_7_NONE;
}



void SettingsScreen7::Display()
{
    int intVal1, intVal2;
    char charVal1[22], charVal2[12];
    PatchSettings* settings;

    // Effects settings always come from voice1Settings
    settings = patch_->voice1Settings_;

    display_->Fill(false);

    display_->SetCursor(0, 0);
    EnumToText::GetEffect1(charVal1, patch_->GetEffectsMode());
    sprintf(buffer_, "%s", charVal1);
    display_->WriteString(buffer_, Font_6x8, selected_ != SETTINGS_SCREEN_7_EFFECTS);

    display_->SetCursor(0, 8);
    EnumToText::GetEffectSetting(charVal1, patch_->GetEffectsMode(), 1);
    EnumToText::GetEffectSetting(charVal2, patch_->GetEffectsMode(), 2);
    intVal1 = settings->getFloatValue(FX_1) * 1000;
    intVal2 = settings->getFloatValue(FX_2) * 1000;
    sprintf(buffer_, "%s %3d %s %3d", charVal1, intVal1, charVal2, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 16);
    EnumToText::GetEffect2(charVal1, patch_->GetEffectsMode());
    sprintf(buffer_, "%s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 24);
    EnumToText::GetEffectSetting(charVal1, patch_->GetEffectsMode(), 3);
    EnumToText::GetEffectSetting(charVal2, patch_->GetEffectsMode(), 4);
    intVal1 = settings->getFloatValue(FX_3) * 1000;
    intVal2 = settings->getFloatValue(FX_4) * 1000;
    sprintf(buffer_, "%s %3d %s %3d", charVal1, intVal1, charVal2, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 32);
    EnumToText::GetEffectSetting(charVal1, patch_->GetEffectsMode(), 5);
    intVal1 = settings->getFloatValue(FX_5) * 1000;
    sprintf(buffer_, "%s %3d", charVal1, intVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 48);
    EnumToText::GetReverbModePadded(charVal1, patch_->GetReverbMode());
    sprintf(buffer_, "%s", charVal1);
    display_->WriteString(buffer_, Font_6x8, selected_ != SETTINGS_SCREEN_7_REVERB);

    display_->SetCursor(0, 56);
    EnumToText::GetReverbModePadded(charVal1, patch_->GetReverbMode());
    intVal1 = settings->getFloatValue(FX_REVERB) * 1000;
    sprintf(buffer_, "Amount %3d", intVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    if (selected_ != SETTINGS_SCREEN_7_NONE) {
        display_->SetCursor(102, 56);
        sprintf(buffer_, "<--");
        display_->WriteString(buffer_, Font_6x8, selected_ != SETTINGS_SCREEN_7_RETURN);
    }

    display_->Update();
}



void SettingsScreen7::Increment() {
    selected_ = (SettingsScreen7Selection)((selected_ + 1) % SETTINGS_SCREEN_7_OPTIONS);
}



void SettingsScreen7::Decrement() {
    selected_ = (SettingsScreen7Selection)((selected_ - 1 + SETTINGS_SCREEN_7_OPTIONS) % SETTINGS_SCREEN_7_OPTIONS);
}



SettingsScreen7Response SettingsScreen7::Click() {
    switch (selected_) {
        case SETTINGS_SCREEN_7_NONE:
            selected_ = SETTINGS_SCREEN_7_EFFECTS;
            return SETTINGS_SCREEN_7_RESPONSE_EDIT;

        case SETTINGS_SCREEN_7_EFFECTS:
            patch_->SetEffectsMode((EffectsMode)(patch_->GetEffectsMode() + 1)); // This function handles wrapping around overflow
            return SETTINGS_SCREEN_7_RESPONSE_EDIT;

        case SETTINGS_SCREEN_7_REVERB:
            patch_->SetReverbMode((ReverbMode)(patch_->GetReverbMode() + 1)); // This function handles wrapping around overflow
            return SETTINGS_SCREEN_7_RESPONSE_EDIT;

        default:
        case SETTINGS_SCREEN_7_RETURN:
            selected_ = SETTINGS_SCREEN_7_NONE;
            return SETTINGS_SCREEN_7_RESPONSE_NOEDIT;
    }
}
