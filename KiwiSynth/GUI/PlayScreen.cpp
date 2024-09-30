#include "PlayScreen.h"

namespace kiwi_synth
{

    void PlayScreen::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
    }

    void PlayScreen::Display()
    {
        //char value[32];

        display->Fill(false);

        display->SetCursor(0, 0);
        if (patch->GetLiveMode()) {
    	    sprintf(buffer, "Live Mode");
        } else {
            char name[MAX_PATCH_NAME_LENGTH + 1];
            patch->GetName(name);
            sprintf(buffer, "%c.%03d %s", 'A' + patch->GetBankNumber(), patch->GetPatchNumber() + 1, name);
        }
        display->WriteString(buffer, Font_6x8, true);

        // The amount of audio interference seems to be dependent on how much pixels are enabled on the screen.
        // If we can fix the interference we can bring back this more attractive display format.
	    /*sprintf(buffer, "Kiwi Synth");
        display->SetCursor(0, 0);
        display->WriteString(buffer, Font_11x18, true);

        display->SetCursor(0, 22);
	    sprintf(buffer, "Version 1.0");
        display->WriteString(buffer, Font_6x8, true);

        if (patch->GetLiveMode()) {
            display->SetCursor(0, 36);
    	    sprintf(buffer, "Live Mode");
            display->WriteString(buffer, Font_6x8, true);
        } else {
            display->SetCursor(0, 36);
            patch->GetName(value);
            sprintf(buffer, "%c.%03d %s", 'A' + patch->GetBankNumber(), patch->GetPatchNumber() + 1, value);
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 44);
            GetPatchType(value); // Needs to be implemented similar to PatchScreen.cpp
            sprintf(buffer, "Type: %s", value);
            display->WriteString(buffer, Font_6x8, true);
        }

        display->SetCursor(36, 56);
	    sprintf(buffer, "Bombastic Audio");
        display->WriteString(buffer, Font_6x8, true);*/

        display->Update();
    }
} // namespace kiwi_synth
