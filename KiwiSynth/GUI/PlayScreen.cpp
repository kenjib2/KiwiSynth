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

	    /*sprintf(buffer, "Kiwi Synth");
        display->SetCursor(0, 0);
        display->WriteString(buffer, Font_11x18, true);

        display->SetCursor(0, 22);
	    sprintf(buffer, "Version 1.0");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 36);
	    sprintf(buffer, "Free Mode");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(36, 56);
	    sprintf(buffer, "Bombastic Audio");
        display->WriteString(buffer, Font_6x8, true);*/

        display->Update();
    }
} // namespace kiwi_synth
