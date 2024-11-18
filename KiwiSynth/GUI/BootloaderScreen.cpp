#include "BootloaderScreen.h"



using namespace kiwisynth;



void BootloaderScreen::Init(KiwiDisplay* display)
{
    display_ = display;
}



void BootloaderScreen::Display()
{
    display_->Fill(false);

    sprintf(buffer_, "Ready for Update...");
    display_->SetCursor(0, 0);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->Update();
}
