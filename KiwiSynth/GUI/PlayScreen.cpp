#include "PlayScreen.h"



using namespace kiwisynth;



void PlayScreen::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
}



void PlayScreen::Display()
{
    char value[32];

    display_->Fill(false);

    sprintf(buffer_, "Kiwi Synth");
    display_->SetCursor(8, 0);
    display_->WriteString(buffer_, Font_11x18, true);

    display_->SetCursor(18, 20);
    sprintf(buffer_, "Bombastic Audio");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(30, 28);
    sprintf(buffer_, "Version 1.0");
    display_->WriteString(buffer_, Font_6x8, true);

    if (patch_->GetLiveMode()) {
        display_->SetCursor(36, 46);
        sprintf(buffer_, "Live Mode");
        display_->WriteString(buffer_, Font_6x8, true);
    } else {
        display_->SetCursor(0, 40);
        patch_->GetName(value);
        sprintf(buffer_, "%c.%03d %s", 'A' + patch_->GetBankNumber(), patch_->GetPatchNumber() + 1, value);
        display_->WriteString(buffer_, Font_6x8, true);

        display_->SetCursor(0, 48);
        EnumToText::GetPatchType(value, patch_->GetPatchType());
        sprintf(buffer_, "Type: %s", value);
        display_->WriteString(buffer_, Font_6x8, true);

        display_->SetCursor(0, 56);
        EnumToText::GetVoiceMode(value, (VoiceMode)patch_->voice1Settings_->getIntValue(OSC_VOICES));
        sprintf(buffer_, "Voices: %s", value);
        display_->WriteString(buffer_, Font_6x8, true);
    }

    display_->Update();
}
