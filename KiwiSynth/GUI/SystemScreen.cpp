#include "SystemScreen.h"

namespace kiwi_synth
{

    void SystemScreen::Init(KiwiDisplay* display, Performance* performance)
    {
        this->display = display;
        this->performance = performance;
    }

    void SystemScreen::Display()
    {
        display->Fill(false);

        display->SetCursor(0, 0);
        sprintf(buffer, "System Version: 1.0");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        sprintf(buffer, "CPU Max: %d  Avg: %d", (int)(performance->Max() * 100), (int)(performance->Avg() * 100));
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        sprintf(buffer, "Updates/Second: %d", performance->UpdatesPerSec());
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        sprintf(buffer, "Update System");
        display->WriteString(buffer, Font_6x8, false);

        display->Update();
    }
} // namespace kiwi_synth
