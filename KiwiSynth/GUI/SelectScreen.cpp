#include "SelectScreen.h"

namespace kiwi_synth
{
    void SelectScreen::Init(KiwiDisplay* display, KiwiSynth* kiwiSynth) {
        this->kiwiSynth = kiwiSynth;
        this->display = display;
        saving = false;
    }

    void SelectScreen::Display() {
        display->Fill(false);
        display->Update();
    }

    SelectScreenResponse SelectScreen::Click() {
        if (saving) {
            return SELECT_SCREEN_RESPONSE_CANCEL;
        } else {
            return SELECT_SCREEN_RESPONSE_PLAY;
        }
    }

} // namespace kiwi_synth
