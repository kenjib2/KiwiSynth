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

        display->SetCursor(0, 12);
        GetFxType(value);
        sprintf(buffer, "FX: %s", value);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        sprintf(buffer, "Save Patch");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 36);
        sprintf(buffer, "Load Patch");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        sprintf(buffer, "Manual Mode");
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

    // 17 character value
    void PatchScreen::GetFxType(char* buffer) {
        switch (patch->getEffectsMode()) {
            case FX_DIST_DLY_HLRVB:
                strcpy(buffer, "Dist-Dly-HallRvb");
                break;
            case FX_DIST_DLY_CHRVB:
                strcpy(buffer, "Dist-Dly-ChambRvb");
                break;
            case FX_DIST_DLY_CARVB:
                strcpy(buffer, "Dist-Dly-CathdRvb");
                break;
            case FX_DIST_DLY_RMRVB:
                strcpy(buffer, "Dist-Dly-RoomRvb");
                break;
            case FX_DIST_DLY_BLRVB:
                strcpy(buffer, "Dist-Dly-BloomRvb");
                break;
        }
    }

} // namespace kiwi_synth
