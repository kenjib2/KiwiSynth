#include "SelectScreen.h"

namespace kiwisynth
{
    void SelectScreen::Init(KiwiDisplay* display, KiwiSynth* kiwiSynth) {
        kiwiSynth_ = kiwiSynth;
        display_ = display;
        saving_ = false;
        selection_ = 3;
        numSelections_ = 35;
        currentPage_ = SELECT_PAGE_BANKS;
    }

    void SelectScreen::Display() {
        char value[22];

        display_->Fill(false);
        switch (currentPage_) {
            default:
            case SELECT_PAGE_TYPES:
                numSelections_ = 16;

                display_->SetCursor(0, 0);
                display_->WriteString("Lead", Font_6x8, selection_ != 3);
                display_->SetCursor(0, 8);
                display_->WriteString("Pad", Font_6x8, selection_ != 4);
                display_->SetCursor(0, 16);
                display_->WriteString("Synth", Font_6x8, selection_ != 5);
                display_->SetCursor(0, 24);
                display_->WriteString("Drone", Font_6x8, selection_ != 6);
                display_->SetCursor(0, 32);
                display_->WriteString("Bass", Font_6x8, selection_ != 7);
                display_->SetCursor(0, 40);
                display_->WriteString("Pluck", Font_6x8, selection_ != 8);
                display_->SetCursor(0, 48);
                display_->WriteString("Key", Font_6x8, selection_ != 9);
                display_->SetCursor(0, 56);
                display_->WriteString("Brass", Font_6x8, selection_ != 10);
                display_->SetCursor(66, 0);
                display_->WriteString("String", Font_6x8, selection_ != 11);
                display_->SetCursor(66, 8);
                display_->WriteString("Arp", Font_6x8, selection_ != 12);
                display_->SetCursor(66, 16);
                display_->WriteString("Percussion", Font_6x8, selection_ != 13);
                display_->SetCursor(66, 24);
                display_->WriteString("Effect", Font_6x8, selection_ != 14);
                display_->SetCursor(66, 32);
                display_->WriteString("Other", Font_6x8, selection_ != 15);

                display_->SetCursor(120, 0);
                display_->WriteString("^", Font_6x8, selection_ != 0);
                display_->SetCursor(120, 28);
                display_->WriteString("X", Font_6x8, selection_ != 1);
                display_->SetCursor(120, 56);
                display_->WriteString("v", Font_6x8, selection_ != 2);
                break;

            case SELECT_PAGE_BANKS:
                numSelections_ = 35;

                for (int i = 0; i < 8; i++) {
                    display_->SetCursor(0, i * 8);
                    sprintf(buffer_, "%c: ", 'A' + i);
                    display_->WriteString(buffer_, Font_6x8, true);

                    for (int j = 0; j < 4; j++) {
                        display_->SetCursor(18 + j * 4 * 6, i * 8);
                        sprintf(buffer_, "%03d", (j) * 32 + 1);
                        display_->WriteString(buffer_, Font_6x8, selection_ != 4 * i + j + 3);
                    }
                }

                if (!saving_) {
                    display_->SetCursor(120, 0);
                    display_->WriteString("^", Font_6x8, selection_ != 0);
                }
                display_->SetCursor(120, 28);
                display_->WriteString("X", Font_6x8, selection_ != 1);
                if (!saving_) {
                    display_->SetCursor(120, 56);
                    display_->WriteString("v", Font_6x8, selection_ != 2);
                }
                break;

            case SELECT_PAGE_BANK_PATCHES:
                numSelections_ = 11;

                for (int i = 0; i < 8; i++) {
                    display_->SetCursor(0, i * 8);
                    sprintf(buffer_, "%c.%03d %s", 'A' + bankNumber_, patchNumber_ + i + 1, kiwiSynth_->patchBanks[bankNumber_][patchNumber_ + i].name);
                    display_->WriteString(buffer_, Font_6x8, i + 3 != selection_);
                }

                display_->SetCursor(120, 0);
                display_->WriteString("^", Font_6x8, selection_ != 0);
                display_->SetCursor(120, 28);
                display_->WriteString("X", Font_6x8, selection_ != 1);
                display_->SetCursor(120, 56);
                display_->WriteString("v", Font_6x8, selection_ != 2);
                break;

            case SELECT_PAGE_TYPE_PATCHES:
                numSelections_ = 3 + std::min(patchTypeMax_ - (patchTypePage_ * 8), 8);

                for (int i = 0; i < numSelections_ - 3; i++) {
                    display_->SetCursor(0, i * 8);
                    sprintf(buffer_, "%c.%03d %s",
                        'A' + kiwiSynth_->patchTypes[patchType_][i + patchTypePage_ * 8]->bankNumber,
                        kiwiSynth_->patchTypes[patchType_][i + patchTypePage_ * 8]->patchNumber + 1,
                        kiwiSynth_->patchTypes[patchType_][i + patchTypePage_ * 8]->name);
                    display_->WriteString(buffer_, Font_6x8, i + 3 != selection_);
                }

                display_->SetCursor(120, 0);
                display_->WriteString("^", Font_6x8, selection_ != 0);
                display_->SetCursor(120, 28);
                display_->WriteString("X", Font_6x8, selection_ != 1);
                display_->SetCursor(120, 56);
                display_->WriteString("v", Font_6x8, selection_ != 2);
                break;

            case SELECT_PAGE_SAVE_CONFIRM:
                numSelections_ = 2;

                display_->SetCursor(0, 0);
                sprintf(buffer_, "Overwrite patch?");
                display_->WriteString(buffer_, Font_6x8, true);

                display_->SetCursor(0, 8);
                sprintf(buffer_, "Data will be lost.");
                display_->WriteString(buffer_, Font_6x8, true);

                display_->SetCursor(0, 24);
                sprintf(buffer_, "%c.%03d %s", 'A' + saveBank_, savePatch_ + 1, kiwiSynth_->patchBanks[saveBank_][savePatch_].name);
                display_->WriteString(buffer_, Font_6x8, true);

                display_->SetCursor(0, 32);
                EnumToText::GetPatchType(value, kiwiSynth_->patchBanks[saveBank_][savePatch_].type);
                sprintf(buffer_, "Type: %s", value);
                display_->WriteString(buffer_, Font_6x8, true);

                display_->SetCursor(0, 48);
                sprintf(buffer_, "Yes");
                display_->WriteString(buffer_, Font_6x8, selection_ != 0);

                display_->SetCursor(66, 48);
                sprintf(buffer_, "No");
                display_->WriteString(buffer_, Font_6x8, selection_ != 1);
                break;
        }
        display_->Update();
    }

    void SelectScreen::Increment() {
        selection_ = (selection_ + 1) % numSelections_;
        if (saving_ && currentPage_ == SELECT_PAGE_BANKS && (selection_ == 0 || selection_ == 2)) {
            // We can't page up and down when saving on bank selection because we don't want the type screen
            Increment();
        }
   }

    void SelectScreen::Decrement() {
        selection_ = (selection_ - 1 + numSelections_) % numSelections_;
        if (saving_ && currentPage_ == SELECT_PAGE_BANKS && (selection_ == 0 || selection_ == 2)) {
            // We can't page up and down when saving on bank selection because we don't want the type screen
            Decrement();
        }
    }

    SelectScreenResponse SelectScreen::Click() {
        // Handling cancel for all pages/screens
        if (selection_ == 1) {
            if (currentPage_ == SELECT_PAGE_TYPE_PATCHES ) {
                currentPage_ = SELECT_PAGE_TYPES;
                return SELECT_SCREEN_RESPONSE_REFRESH;
            } else if (currentPage_ ==  SELECT_PAGE_BANK_PATCHES) {
                bankNumber_ = 0;
                patchNumber_ = 0;
                currentPage_ = SELECT_PAGE_BANKS;
                return SELECT_SCREEN_RESPONSE_REFRESH;
            } else if (fromPlay_) {
                return SELECT_SCREEN_RESPONSE_PLAY;
            } else {
                return SELECT_SCREEN_RESPONSE_CANCEL;
            }

        // Page down from bank/type screen
        } else if (selection_ == 0 && currentPage_ >= 0) {
            currentPage_ = (SelectScreenPage)((currentPage_ + 1) % maxSelectScreenPage);

        // Page up from bank/type screen
        } else if (selection_ == 2 && currentPage_ >= 0) {
            currentPage_ = (SelectScreenPage)((currentPage_ - 1  + maxSelectScreenPage) % maxSelectScreenPage);

        // Patch group selected from Bank select page
        } else if (currentPage_ == SELECT_PAGE_BANKS) {
            bankNumber_ = (selection_ - 3) / 4;
            patchNumber_ = (selection_ - 3) % 4 * 32;
            selection_ = 3;
            currentPage_ = SELECT_PAGE_BANK_PATCHES;

        // Patch type selected from type select page
        } else if (currentPage_ == SELECT_PAGE_TYPES) {
            patchType_ = (PatchType)(selection_ - 3);
            patchTypePage_ = 0;
            patchTypeMax_ = kiwiSynth_->patchTypes[patchType_].size();
            selection_ = 3;
            currentPage_ = SELECT_PAGE_TYPE_PATCHES;

        } else if (currentPage_ == SELECT_PAGE_BANK_PATCHES) {
            if (selection_ == 0) {
                patchNumber_ = patchNumber_ - 8;
                if (patchNumber_ < 0) {
                    patchNumber_ = 120;
                    bankNumber_ = (bankNumber_ - 1 + NUM_PATCH_BANKS) % NUM_PATCH_BANKS;
                }

            } else if (selection_ == 2) {
                patchNumber_ = patchNumber_ + 8;
                if (patchNumber_ > 120) {
                    patchNumber_ = 0;
                    bankNumber_ = (bankNumber_ + 1) % NUM_PATCH_BANKS;
                }

            } else if (!saving_) {
                kiwiSynth_->LoadPatch(bankNumber_, patchNumber_ + selection_ - 3);
                return SELECT_SCREEN_RESPONSE_PLAY;

            } else if (saving_) {
                saveBank_ = bankNumber_;
                savePatch_ = patchNumber_ + selection_ - 3;
                selection_ = 0;
                currentPage_ = SELECT_PAGE_SAVE_CONFIRM;
            }

        } else if (currentPage_ == SELECT_PAGE_TYPE_PATCHES) {
            if (selection_ == 0) {
                patchTypePage_ = patchTypePage_ - 1;
                if (patchTypePage_ < 0) {
                    patchTypePage_ = (patchTypeMax_ - 1) / 8;
                }

            } else if (selection_ == 2) {
                patchTypePage_ = patchTypePage_ + 1;
                if (patchTypePage_ > (patchTypeMax_ - 1) / 8) {
                    patchTypePage_ = 0;
                }

            } else if (!saving_) {
                bankNumber_ = kiwiSynth_->patchTypes[patchType_][selection_ - 3 + patchTypePage_ * 8]->bankNumber;
                patchNumber_ = kiwiSynth_->patchTypes[patchType_][selection_ - 3 + patchTypePage_ * 8]->patchNumber;
                kiwiSynth_->LoadPatch(bankNumber_, patchNumber_);
                return SELECT_SCREEN_RESPONSE_PLAY;

            } else if (saving_) {
                saveBank_ = kiwiSynth_->patchTypes[patchType_][selection_ - 3 + patchTypePage_ * 8]->bankNumber;
                savePatch_ = kiwiSynth_->patchTypes[patchType_][selection_ - 3 + patchTypePage_ * 8]->patchNumber;
                selection_ = 0;
                currentPage_ = SELECT_PAGE_SAVE_CONFIRM;
            }
        } else if (currentPage_ == SELECT_PAGE_SAVE_CONFIRM) {
            if (selection_ == 0) {
                // If the patch we are saving was an initialized patch, set it to type other by default.
                if (kiwiSynth_->patch.GetPatchType() == PATCH_INIT) {
                    kiwiSynth_->patch.SetPatchType(PATCH_OTHER);
                }
                kiwiSynth_->SavePatch(saveBank_, savePatch_);
                return SELECT_SCREEN_RESPONSE_PLAY;
            } else {
                return SELECT_SCREEN_RESPONSE_CANCEL;
            }
        }

        return SELECT_SCREEN_RESPONSE_REFRESH;
    }

} // namespace kiwisynth
