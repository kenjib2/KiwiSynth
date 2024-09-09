#include "PatchScreen.h"

namespace kiwi_synth
{

    void PatchScreen::Init(KiwiDisplay* display, PatchSettings* patchSettings)
    {
        this->display = display;
        this->patchSettings = patchSettings;
    }

    void PatchScreen::Display()
    {
        display->Fill(false);

        display->SetCursor(0, 0);
        sprintf(buffer, "Name: %s", patchSettings->getStringValue(GEN_NAME));
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        sprintf(buffer, "Save Patch");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        sprintf(buffer, "Load Patch");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        sprintf(buffer, "Manual Mode");
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }
} // namespace kiwi_synth
