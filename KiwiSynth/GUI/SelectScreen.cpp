#include "SelectScreen.h"

namespace kiwisynth
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
        char value[22];

        display->Fill(false);
        switch (currentPage) {
            default:
            case SELECT_PAGE_TYPES:
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
                    sprintf(buffer, "%c: ", 'A' + i);
                    display->WriteString(buffer, Font_6x8, true);

                    for (int j = 0; j < 4; j++) {
                        display->SetCursor(18 + j * 4 * 6, i * 8);
                        sprintf(buffer, "%03d", (j) * 32 + 1);
                        display->WriteString(buffer, Font_6x8, selection != 4 * i + j + 3);
                    }
                }

                if (!saving) {
                    display->SetCursor(120, 0);
                    display->WriteString("^", Font_6x8, selection != 0);
                }
                display->SetCursor(120, 28);
                display->WriteString("X", Font_6x8, selection != 1);
                if (!saving) {
                    display->SetCursor(120, 56);
                    display->WriteString("v", Font_6x8, selection != 2);
                }
                break;

            case SELECT_PAGE_BANK_PATCHES:
                numSelections = 11;

                for (int i = 0; i < 8; i++) {
                    display->SetCursor(0, i * 8);
                    sprintf(buffer, "%c.%03d %s", 'A' + bankNumber, patchNumber + i + 1, kiwiSynth->patchBanks[bankNumber][patchNumber + i].name);
                    display->WriteString(buffer, Font_6x8, i + 3 != selection);
                }

                display->SetCursor(120, 0);
                display->WriteString("^", Font_6x8, selection != 0);
                display->SetCursor(120, 28);
                display->WriteString("X", Font_6x8, selection != 1);
                display->SetCursor(120, 56);
                display->WriteString("v", Font_6x8, selection != 2);
                break;

            case SELECT_PAGE_TYPE_PATCHES:
                numSelections = 3 + std::min(patchTypeMax - (patchTypePage * 8), 8);

                for (int i = 0; i < numSelections - 3; i++) {
                    display->SetCursor(0, i * 8);
                    sprintf(buffer, "%c.%03d %s",
                        'A' + kiwiSynth->patchTypes[patchType][i + patchTypePage * 8]->bankNumber,
                        kiwiSynth->patchTypes[patchType][i + patchTypePage * 8]->patchNumber + 1,
                        kiwiSynth->patchTypes[patchType][i + patchTypePage * 8]->name);
                    display->WriteString(buffer, Font_6x8, i + 3 != selection);
                }

                display->SetCursor(120, 0);
                display->WriteString("^", Font_6x8, selection != 0);
                display->SetCursor(120, 28);
                display->WriteString("X", Font_6x8, selection != 1);
                display->SetCursor(120, 56);
                display->WriteString("v", Font_6x8, selection != 2);
                break;

            case SELECT_PAGE_SAVE_CONFIRM:
                numSelections = 2;

                display->SetCursor(0, 0);
                sprintf(buffer, "Overwrite patch?");
                display->WriteString(buffer, Font_6x8, true);

                display->SetCursor(0, 8);
                sprintf(buffer, "Data will be lost.");
                display->WriteString(buffer, Font_6x8, true);

                display->SetCursor(0, 24);
                sprintf(buffer, "%c.%03d %s", 'A' + saveBank, savePatch + 1, kiwiSynth->patchBanks[saveBank][savePatch].name);
                display->WriteString(buffer, Font_6x8, true);

                display->SetCursor(0, 32);
                EnumToText::GetPatchType(value, kiwiSynth->patchBanks[saveBank][savePatch].type);
                sprintf(buffer, "Type: %s", value);
                display->WriteString(buffer, Font_6x8, true);

                display->SetCursor(0, 48);
                sprintf(buffer, "Yes");
                display->WriteString(buffer, Font_6x8, selection != 0);

                display->SetCursor(66, 48);
                sprintf(buffer, "No");
                display->WriteString(buffer, Font_6x8, selection != 1);
                break;
        }
        display->Update();
    }

    void SelectScreen::Increment() {
        selection = (selection + 1) % numSelections;
        if (saving && currentPage == SELECT_PAGE_BANKS && (selection == 0 || selection == 2)) {
            // We can't page up and down when saving on bank selection because we don't want the type screen
            Increment();
        }
   }

    void SelectScreen::Decrement() {
        selection = (selection - 1 + numSelections) % numSelections;
        if (saving && currentPage == SELECT_PAGE_BANKS && (selection == 0 || selection == 2)) {
            // We can't page up and down when saving on bank selection because we don't want the type screen
            Decrement();
        }
    }

    SelectScreenResponse SelectScreen::Click() {
        // Handling cancel for all pages/screens
        if (selection == 1) {
            if (currentPage == SELECT_PAGE_TYPE_PATCHES ) {
                currentPage = SELECT_PAGE_TYPES;
                return SELECT_SCREEN_RESPONSE_REFRESH;
            } else if (currentPage ==  SELECT_PAGE_BANK_PATCHES) {
                bankNumber = 0;
                patchNumber = 0;
                currentPage = SELECT_PAGE_BANKS;
                return SELECT_SCREEN_RESPONSE_REFRESH;
            } else if (fromPlay) {
                return SELECT_SCREEN_RESPONSE_PLAY;
            } else {
                return SELECT_SCREEN_RESPONSE_CANCEL;
            }

        // Page down from bank/type screen
        } else if (selection == 0 && currentPage >= 0) {
            currentPage = (SelectScreenPage)((currentPage + 1) % maxSelectScreenPage);

        // Page up from bank/type screen
        } else if (selection == 2 && currentPage >= 0) {
            currentPage = (SelectScreenPage)((currentPage - 1  + maxSelectScreenPage) % maxSelectScreenPage);

        // Patch group selected from Bank select page
        } else if (currentPage == SELECT_PAGE_BANKS) {
            bankNumber = (selection - 3) / 4;
            patchNumber = (selection - 3) % 4 * 32;
            selection = 3;
            currentPage = SELECT_PAGE_BANK_PATCHES;

        // Patch type selected from type select page
        } else if (currentPage == SELECT_PAGE_TYPES) {
            patchType = (PatchType)(selection - 3);
            patchTypePage = 0;
            patchTypeMax = kiwiSynth->patchTypes[patchType].size();
            selection = 3;
            currentPage = SELECT_PAGE_TYPE_PATCHES;

        } else if (currentPage == SELECT_PAGE_BANK_PATCHES) {
            if (selection == 0) {
                patchNumber = patchNumber - 8;
                if (patchNumber < 0) {
                    patchNumber = 120;
                    bankNumber = (bankNumber - 1 + NUM_PATCH_BANKS) % NUM_PATCH_BANKS;
                }

            } else if (selection == 2) {
                patchNumber = patchNumber + 8;
                if (patchNumber > 120) {
                    patchNumber = 0;
                    bankNumber = (bankNumber + 1) % NUM_PATCH_BANKS;
                }

            } else if (!saving) {
                kiwiSynth->LoadPatch(bankNumber, patchNumber + selection - 3);
                return SELECT_SCREEN_RESPONSE_PLAY;

            } else if (saving) {
                saveBank = bankNumber;
                savePatch = patchNumber + selection - 3;
                selection = 0;
                currentPage = SELECT_PAGE_SAVE_CONFIRM;
            }

        } else if (currentPage == SELECT_PAGE_TYPE_PATCHES) {
            if (selection == 0) {
                patchTypePage = patchTypePage - 1;
                if (patchTypePage < 0) {
                    patchTypePage = (patchTypeMax - 1) / 8;
                }

            } else if (selection == 2) {
                patchTypePage = patchTypePage + 1;
                if (patchTypePage > (patchTypeMax - 1) / 8) {
                    patchTypePage = 0;
                }

            } else if (!saving) {
                bankNumber = kiwiSynth->patchTypes[patchType][selection - 3 + patchTypePage * 8]->bankNumber;
                patchNumber = kiwiSynth->patchTypes[patchType][selection - 3 + patchTypePage * 8]->patchNumber;
                kiwiSynth->LoadPatch(bankNumber, patchNumber);
                return SELECT_SCREEN_RESPONSE_PLAY;

            } else if (saving) {
                saveBank = kiwiSynth->patchTypes[patchType][selection - 3 + patchTypePage * 8]->bankNumber;
                savePatch = kiwiSynth->patchTypes[patchType][selection - 3 + patchTypePage * 8]->patchNumber;
                selection = 0;
                currentPage = SELECT_PAGE_SAVE_CONFIRM;
            }
        } else if (currentPage == SELECT_PAGE_SAVE_CONFIRM) {
            if (selection == 0) {
                // If the patch we are saving was an initialized patch, set it to type other by default.
                if (kiwiSynth->patch.GetPatchType() == PATCH_INIT) {
                    kiwiSynth->patch.SetPatchType(PATCH_OTHER);
                }
                kiwiSynth->SavePatch(saveBank, savePatch);
                return SELECT_SCREEN_RESPONSE_PLAY;
            } else {
                return SELECT_SCREEN_RESPONSE_CANCEL;
            }
        }

        return SELECT_SCREEN_RESPONSE_REFRESH;
    }

} // namespace kiwisynth
