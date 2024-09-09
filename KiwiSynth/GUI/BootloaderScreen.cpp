#include "BootloaderScreen.h"

namespace kiwi_synth
{

    void BootloaderScreen::Init(KiwiDisplay* display, PatchSettings* patchSettings)
    {
        this->display = display;
        this->patchSettings = patchSettings;
    }

    void BootloaderScreen::Display()
    {
        display->Fill(false);

		sprintf(buffer, "Ready for Update...");
        display->SetCursor(0, 0);
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }
} // namespace kiwi_synth
