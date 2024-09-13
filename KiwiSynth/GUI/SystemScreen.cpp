#include "SystemScreen.h"

namespace kiwi_synth
{

    void SystemScreen::Init(KiwiDisplay* display)
    {
        this->display = display;
    }

    void SystemScreen::Display()
    {
        display->Fill(false);

        display->SetCursor(0, 0);
        sprintf(buffer, "System Version: 1.0");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        sprintf(buffer, "Update System");
        display->WriteString(buffer, Font_6x8, false);

        display->Update();
    }
} // namespace kiwi_synth
