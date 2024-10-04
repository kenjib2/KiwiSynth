#include "SystemScreen.h"

namespace kiwi_synth
{

    void SystemScreen::Init(KiwiDisplay* display, Performance* performance)
    {
        this->display = display;
        this->performance = performance;
        selected = SYSTEM_SCREEN_NONE;
    }

    void SystemScreen::Display()
    {
        display->Fill(false);

        display->SetCursor(0, 0);
        sprintf(buffer, "System Version: 1.0");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 12);
        sprintf(buffer, "Reads/Second: %d", performance->ReadsPerSec());
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        sprintf(buffer, "Updates/Second: %d", performance->UpdatesPerSec());
        display->WriteString(buffer, Font_6x8, true);

	    #ifdef __CPU_LOAD__
            display->SetCursor(0, 36);
            sprintf(buffer, "CPU Max: %d  Avg: %d", (int)(performance->Max() * 100), (int)(performance->Avg() * 100));
            display->WriteString(buffer, Font_6x8, true);
    	#endif // __CPU_LOAD__

        display->SetCursor(0, 46);
        sprintf(buffer, "Panic!!!");
        display->WriteString(buffer, Font_6x8, selected != SYSTEM_SCREEN_PANIC);

        display->SetCursor(0, 56);
        sprintf(buffer, "Flash BIOS");
        display->WriteString(buffer, Font_6x8, selected != SYSTEM_SCREEN_UPDATE);

        if (selected != SYSTEM_SCREEN_NONE) {
            display->SetCursor(108, 56);
            sprintf(buffer, "<--");
            display->WriteString(buffer, Font_6x8, selected != SYSTEM_SCREEN_RETURN);
        }

        display->Update();
    }

    void SystemScreen::Increment() {
        selected = (SystemScreenSelection)((selected + 1) % SYSTEM_SCREEN_OPTIONS);
    }

    void SystemScreen::Decrement() {
        selected = (SystemScreenSelection)((selected - 1 + SYSTEM_SCREEN_OPTIONS) % SYSTEM_SCREEN_OPTIONS);
    }

    SystemScreenResponse SystemScreen::Click() {
        switch (selected) {
            case SYSTEM_SCREEN_NONE:
                selected = SYSTEM_SCREEN_UPDATE;
                return SYSTEM_SCREEN_RESPONSE_EDIT;

            case SYSTEM_SCREEN_UPDATE:
                return SYSTEM_SCREEN_RESPONSE_UPDATE;

            case SYSTEM_SCREEN_PANIC:
                selected = SYSTEM_SCREEN_NONE;
                return SYSTEM_SCREEN_RESPONSE_PANIC;

            default:
            case SYSTEM_SCREEN_RETURN:
                selected = SYSTEM_SCREEN_NONE;
                return SYSTEM_SCREEN_RESPONSE_NOEDIT;
        }
    }

} // namespace kiwi_synth
