#include "PatchScreen.h"

namespace kiwi_synth
{

    void PatchScreen::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
    }

    void PatchScreen::Display()
    {
        display->Fill(false);

        display->SetCursor(0, 0);
        char nameString[MAX_PATCH_NAME_LENGTH + 1];
        patch->getName(nameString);
        sprintf(buffer, "Name: %s", nameString);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 10);
        GetFxType(value);
        sprintf(buffer, "FX: %s", value);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 20);
        GetReverbType(value);
        sprintf(buffer, "Reverb: %s", value);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 30);
        sprintf(buffer, "Save Patch");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        sprintf(buffer, "Load Patch");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 50);
        sprintf(buffer, "Manual Mode");
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

    // 17 character value
    void PatchScreen::GetFxType(char* buffer) {
        switch (patch->getEffectsMode()) {
            case FX_DISTORTION_DELAY:
                strcpy(buffer, "Distortion-Delay");
                break;
            case FX_CHORUS_DELAY:
                strcpy(buffer, "Chorus-Delay");
                break;
            case FX_PHASER_DELAY:
                strcpy(buffer, "Phaser-Delay");
                break;
            case FX_FLANGER_DELAY:
                strcpy(buffer, "Flanger-Delay");
                break;
        }
    }

    // 13 character value
    void PatchScreen::GetReverbType(char* buffer) {
        switch (patch->getReverbMode()) {
            case REVERB_ROOM:
                strcpy(buffer, "Room");
                break;
            case REVERB_HALL:
                strcpy(buffer, "Hall");
                break;
            case REVERB_CHAMBER:
                strcpy(buffer, "Chamber");
                break;
            case REVERB_CATHEDRAL:
                strcpy(buffer, "Cathedral");
                break;
            case REVERB_BLOOM:
                strcpy(buffer, "Bloom");
                break;
        }
    }

} // namespace kiwi_synth
