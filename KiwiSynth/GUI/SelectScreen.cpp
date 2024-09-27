#include "SelectScreen.h"

namespace kiwi_synth
{
    void SelectScreen::Init(KiwiDisplay* display, KiwiSynth* kiwiSynth) {
        this->kiwiSynth = kiwiSynth;
        this->display = display;
        saving = false;
        selection = 3;
        numSelections = 35;
        currentPage = SELECT_PAGE_BANKS;
    }

    void SelectScreen::Display() {
        display->Fill(false);
        switch (currentPage) {
            default:
            case SELECT_PAGE_PATCH_TYPES:
                numSelections = 16;

                display->SetCursor(0, 0);
                display->WriteString("Lead", Font_6x8, selection != 3);
                display->SetCursor(0, 8);
                display->WriteString("Pad", Font_6x8, selection != 4);
                display->SetCursor(0, 16);
                display->WriteString("Synth", Font_6x8, selection != 5);
                display->SetCursor(0, 24);
                display->WriteString("Drone", Font_6x8, selection != 6);
                display->SetCursor(0, 32);
                display->WriteString("Bass", Font_6x8, selection != 7);
                display->SetCursor(0, 40);
                display->WriteString("Pluck", Font_6x8, selection != 8);
                display->SetCursor(0, 48);
                display->WriteString("Key", Font_6x8, selection != 9);
                display->SetCursor(0, 56);
                display->WriteString("Brass", Font_6x8, selection != 10);
                display->SetCursor(66, 0);
                display->WriteString("String", Font_6x8, selection != 11);
                display->SetCursor(66, 8);
                display->WriteString("Arp", Font_6x8, selection != 12);
                display->SetCursor(66, 16);
                display->WriteString("Percussion", Font_6x8, selection != 13);
                display->SetCursor(66, 24);
                display->WriteString("Effect", Font_6x8, selection != 14);
                display->SetCursor(66, 32);
                display->WriteString("Other", Font_6x8, selection != 15);

                display->SetCursor(120, 0);
                display->WriteString("^", Font_6x8, selection != 0);

                display->SetCursor(120, 28);
                display->WriteString("X", Font_6x8, selection != 1);

                display->SetCursor(120, 56);
                display->WriteString("v", Font_6x8, selection != 2);
                break;
            case SELECT_PAGE_BANKS:
                numSelections = 35;
                for (int i = 0; i < 8; i++) {
                    display->SetCursor(0, i * 8);
                    sprintf(buffer, "%d: ", i + 1);
                    display->WriteString(buffer, Font_6x8, true);

                    for (int j = 0; j < 4; j++) {
                        display->SetCursor(18 + j * 4 * 6, i * 8);
                        sprintf(buffer, "%03d", (j) * 32 + 1);
                        display->WriteString(buffer, Font_6x8, selection != 4 * i + j + 3);
                    }
                }

                display->SetCursor(120, 0);
                display->WriteString("^", Font_6x8, selection != 0);

                display->SetCursor(120, 28);
                display->WriteString("X", Font_6x8, selection != 1);

                display->SetCursor(120, 56);
                display->WriteString("v", Font_6x8, selection != 2);
                break;
        }
        display->Update();
    }

    // Patchnumber should be a multiple of 8 between 0 and 120
    void SelectScreen::DisplayPatches(int bankNumber, int patchNumber) {
        for (int i = 0; i < 8; i++) {
            display->SetCursor(0, i * 8);
            sprintf(buffer, "%d.%03d %s", bankNumber + 1, patchNumber + i + 1, "Patch Name 1234");
            display->WriteString(buffer, Font_6x8, i != selection);
        }
    }

    void SelectScreen::Increment() {
        selection = (selection + 1) % numSelections;
    }

    void SelectScreen::Decrement() {
        selection = (selection - 1 + numSelections) % numSelections;
    }

    SelectScreenResponse SelectScreen::Click() {
        if (selection == 1) {
            if (fromPlay) {
                return SELECT_SCREEN_RESPONSE_PLAY;
            } else {
                return SELECT_SCREEN_RESPONSE_CANCEL;
            }
        } else if (selection == 0) {
            currentPage = (SelectScreenPage)((currentPage + 1) % maxSelectScreenPage);
        } else if (selection == 2) {
            currentPage = (SelectScreenPage)((currentPage - 1  + maxSelectScreenPage) % maxSelectScreenPage);
        } else {
            if (saving) {
                return SELECT_SCREEN_RESPONSE_CANCEL;
            } else {
                return SELECT_SCREEN_RESPONSE_PLAY;
            }
        }

        return SELECT_SCREEN_RESPONSE_REFRESH;
    }

} // namespace kiwi_synth
