#include "PlayScreen.h"

namespace kiwisynth
{

    void PlayScreen::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
    }

    void PlayScreen::Display()
    {
        char value[32];

        display->Fill(false);

	    sprintf(buffer, "Kiwi Synth");
        display->SetCursor(8, 0);
        display->WriteString(buffer, Font_11x18, true);

        display->SetCursor(18, 20);
	    sprintf(buffer, "Bombastic Audio");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(30, 28);
	    sprintf(buffer, "Version 1.0");
        display->WriteString(buffer, Font_6x8, true);

        if (patch->GetLiveMode()) {
            display->SetCursor(36, 46);
    	    sprintf(buffer, "Live Mode");
            display->WriteString(buffer, Font_6x8, true);
        } else {
            display->SetCursor(0, 40);
            patch->GetName(value);
            sprintf(buffer, "%c.%03d %s", 'A' + patch->GetBankNumber(), patch->GetPatchNumber() + 1, value);
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 48);
            EnumToText::GetPatchType(value, patch->GetPatchType());
            sprintf(buffer, "Type: %s", value);
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 56);
            EnumToText::GetVoiceMode(value, (VoiceMode)patch->voice1Settings_->getIntValue(OSC_VOICES));
            sprintf(buffer, "Voices: %s", value);
            display->WriteString(buffer, Font_6x8, true);
        }

        display->Update();
    }
} // namespace kiwisynth
