#include "SystemScreen.h"



using namespace kiwisynth;



void SystemScreen::Init(KiwiDisplay* display, Performance* performance)
{
    display_ = display;
    performance_ = performance;
    selected_ = SYSTEM_SCREEN_NONE;
}



void SystemScreen::Display()
{
    display_->Fill(false);

    display_->SetCursor(0, 0);
    sprintf(buffer_, "System Version: 1.0");
    display_->WriteString(buffer_, Font_6x8, true);

    #ifdef __CPU_LOAD__
        display_->SetCursor(0, 12);
        sprintf(buffer_, "Reads/Second: %d", performance_->ReadsPerSec());
        display_->WriteString(buffer_, Font_6x8, true);

        display_->SetCursor(0, 24);
        sprintf(buffer_, "CPU Max: %d  Avg: %d", (int)(performance_->Max() * 100), (int)(performance_->Avg() * 100));
        display_->WriteString(buffer_, Font_6x8, true);
    #endif // __CPU_LOAD__

    display_->SetCursor(0, 32);
    sprintf(buffer_, "Panic!!!");
    display_->WriteString(buffer_, Font_6x8, selected_ != SYSTEM_SCREEN_PANIC);

    display_->SetCursor(0, 40);
    sprintf(buffer_, "Sysex Send");
    display_->WriteString(buffer_, Font_6x8, selected_ != SYSTEM_SCREEN_SYSEX_SEND);

    display_->SetCursor(0, 48);
    sprintf(buffer_, "Sysex Receive");
    display_->WriteString(buffer_, Font_6x8, selected_ != SYSTEM_SCREEN_SYSEX_READ);

    display_->SetCursor(0, 56);
    sprintf(buffer_, "Flash BIOS");
    display_->WriteString(buffer_, Font_6x8, selected_ != SYSTEM_SCREEN_UPDATE);

    if (selected_ != SYSTEM_SCREEN_NONE) {
        display_->SetCursor(108, 56);
        sprintf(buffer_, "<--");
        display_->WriteString(buffer_, Font_6x8, selected_ != SYSTEM_SCREEN_RETURN);
    }

    display_->Update();
}



void SystemScreen::DisplaySending()
{
    display_->Fill(false);

    display_->SetCursor(0, 0);
    sprintf(buffer_, "Sending Sysex...");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->Update();
}



void SystemScreen::DisplayReceiving()
{
    display_->Fill(false);

    display_->SetCursor(0, 0);
    sprintf(buffer_, "Ready for Sysex");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 10);
    sprintf(buffer_, "Listening...");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 56);
    sprintf(buffer_, "Cancel");
    display_->WriteString(buffer_, Font_6x8, false);

    display_->Update();
}



void SystemScreen::Increment() {
    selected_ = (SystemScreenSelection)((selected_ + 1) % SYSTEM_SCREEN_OPTIONS);
}



void SystemScreen::Decrement() {
    selected_ = (SystemScreenSelection)((selected_ - 1 + SYSTEM_SCREEN_OPTIONS) % SYSTEM_SCREEN_OPTIONS);
}



SystemScreenResponse SystemScreen::Click() {
    switch (selected_) {
        case SYSTEM_SCREEN_NONE:
            selected_ = SYSTEM_SCREEN_PANIC;
            return SYSTEM_SCREEN_RESPONSE_EDIT;

        case SYSTEM_SCREEN_UPDATE:
            return SYSTEM_SCREEN_RESPONSE_UPDATE;

        case SYSTEM_SCREEN_PANIC:
            selected_ = SYSTEM_SCREEN_NONE;
            return SYSTEM_SCREEN_RESPONSE_PANIC;

        case SYSTEM_SCREEN_SYSEX_SEND:
            selected_ = SYSTEM_SCREEN_NONE;
            return SYSTEM_SCREEN_RESPONSE_SYSEX_SEND;

        case SYSTEM_SCREEN_SYSEX_READ:
            selected_ = SYSTEM_SCREEN_NONE;
            return SYSTEM_SCREEN_RESPONSE_SYSEX_READ;

        default:
        case SYSTEM_SCREEN_RETURN:
            selected_ = SYSTEM_SCREEN_NONE;
            return SYSTEM_SCREEN_RESPONSE_NOEDIT;
    }
}
