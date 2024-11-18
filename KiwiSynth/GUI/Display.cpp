#include "Display.h"



using namespace kiwisynth;



void Display::Init(KiwiSynth* kiwiSynth, Performance* performance)
{
    DisplayConfig cfg;
    cfg.Defaults();
    Init(&cfg, kiwiSynth, performance);
}



void Display::Init(DisplayConfig *displayConfig, KiwiSynth* kiwiSynth, Performance* performance)
{
    this->kiwiSynth_ = kiwiSynth;
    patch_ = &(kiwiSynth->patch);
    this->performance_ = performance;
    guiButton_ = false;
    mode_ = MODE_PLAY;
    menuActive_ = false;
    prevSelectValue_ = patch_->voice1Settings_->getIntValue(GEN_SELECT);

    playScreen_.Init(&display_, patch_);
    bootloaderScreen_.Init(&display_);
    patchScreen_.Init(&display_, patch_);
    selectScreen_.Init(&display_, kiwiSynth);
    systemScreen_.Init(&display_, performance);
    intValueScreen_.Init(&display_, patch_);
    #ifdef __SETTINGS_SCREENS__
        settingsScreen1_.Init(&display_, patch_);
        settingsScreen2_.Init(&display_, patch_);
        settingsScreen3_.Init(&display_, patch_);
        settingsScreen4_.Init(&display_, patch_);
        settingsScreen5_.Init(&display_, patch_);
        settingsScreen6_.Init(&display_, patch_);
        settingsScreen7_.Init(&display_, patch_);
    #endif

    KiwiDisplay::Config cfg;
    cfg.driver_config.transport_config.i2c_config.periph         = displayConfig->periph;
    cfg.driver_config.transport_config.i2c_config.speed          = displayConfig->speed;
    cfg.driver_config.transport_config.i2c_config.mode           = displayConfig->mode;
    cfg.driver_config.transport_config.i2c_config.pin_config.scl = displayConfig->sclPin;
    cfg.driver_config.transport_config.i2c_config.pin_config.sda = displayConfig->sdaPin;

    display_.Init(cfg);
    display_.Fill(false);
}



void Display::HandleInput()
{
    bool updateNeeded = false;

    updateNeeded = HandleEncoder();
    updateNeeded = updateNeeded || HandleClick();

    if (__builtin_expect(updateNeeded, 0)) {
        Update();
    }
}



bool Display::HandleEncoder() {
    bool updateNeeded = false;

    // Handle encoder rotations: Figure out if it changed. -1 is counterclockwise, 1 is clockwise, and 0 is no movement.
    int newValue = patch_->voice1Settings_->getIntValue(GEN_SELECT);
    int direction = 0;
    if (__builtin_expect(newValue != prevSelectValue_, 0)) {
        if ((newValue > prevSelectValue_ || (newValue == -128 && prevSelectValue_ == 127))
            && !(newValue == 127 && prevSelectValue_ == -128)) {
            direction = 1;
        } else if (newValue < prevSelectValue_ || (newValue == 127 && prevSelectValue_ == -128)) {
            direction = -1;
        }
        prevSelectValue_ = newValue;

        if (__builtin_expect(!menuActive_, 1)) {
            // Cycle through display screens
            if (direction == 1) {
                if (mode_ + 1 < DISPLAY_MODE_OPTIONS) {
                    mode_ = (DisplayMode)(mode_ + 1);
                } else {
                    mode_ = MODE_PLAY;
                }
            } else if (direction == -1) {
                if (mode_ - 1 < 0) {
                    mode_ = (DisplayMode)(DISPLAY_MODE_OPTIONS - 1);
                } else {
                    mode_ = (DisplayMode)(mode_ - 1);
                }
            }
            updateNeeded = true;
        } else {
            // Handle menus within screens
            if (__builtin_expect(mode_ == MODE_PLAY, 1)) {
            } else if (mode_ == MODE_PATCH_SCREEN) {
                if (direction == 1) {
                    patchScreen_.Increment();
                } else if (direction == -1) {
                    patchScreen_.Decrement();
                }
                updateNeeded = true;
            /*} else if (mode_ == MODE_INT_VALUE_SCREEN) {
                if (direction == 1) {
                    intValueScreen_.Increment();
                } else if (direction == -1) {
                    intValueScreen_.Decrement();
                }
                updateNeeded = true;*/
            #ifdef __SETTINGS_SCREENS__
                } else if (mode_ == MODE_SETTINGS_SCREEN_6) {
                    if (direction == 1) {
                        settingsScreen6_.Increment();
                    } else if (direction == -1) {
                        settingsScreen6_.Decrement();
                    }
                    updateNeeded = true;
                } else if (mode_ == MODE_SETTINGS_SCREEN_7) {
                    if (direction == 1) {
                        settingsScreen7_.Increment();
                    } else if (direction == -1) {
                        settingsScreen7_.Decrement();
                    }
                    updateNeeded = true;
            #endif
            } else if (mode_ == MODE_SELECT_SCREEN) {
                if (direction == 1) {
                    selectScreen_.Increment();
                } else if (direction == -1) {
                    selectScreen_.Decrement();
                }
                updateNeeded = true;
            } else if (mode_ == MODE_SYSTEM_SCREEN) {
                if (direction == 1) {
                    systemScreen_.Increment();
                } else if (direction == -1) {
                    systemScreen_.Decrement();
                }
                updateNeeded = true;
            }
        }
    }

    return updateNeeded;
}



bool Display::HandleClick() {
    bool updateNeeded = false;

    // Handle encoder button clicks
    bool prevGuiButton = guiButton_;
    guiButton_ = patch_->voice1Settings_->getBoolValue(GEN_SELECT_BUTTON);
    if (__builtin_expect(prevGuiButton && !guiButton_, 0)) {
        PatchScreenResponse patchResponse;
        SelectScreenResponse selectResponse;
        //IntScreenResponse intResponse;
        SystemScreenResponse systemResponse;
        #ifdef __SETTINGS_SCREENS__
            SettingsScreen6Response settings6Response;
            SettingsScreen7Response settings7Response;
        #endif

        if (__builtin_expect(mode_ == MODE_PLAY, 1)) {
            // From play mode we want to load patches, starting positioned at the current patch for convenience.
            menuActive_ = true;
            selectScreen_.saving_ = false;
            selectScreen_.fromPlay_ = true;
            if (patch_->GetLiveMode()) {
                selectScreen_.bankNumber_ = 0;
                selectScreen_.patchNumber_ = 0;
                selectScreen_.selection_ = 3;
                selectScreen_.currentPage_ = SELECT_PAGE_BANKS;
            } else {
                selectScreen_.bankNumber_ = patch_->GetBankNumber();
                selectScreen_.patchNumber_ = patch_->GetPatchNumber() & ~7;
                selectScreen_.selection_ = patch_->GetPatchNumber() % 8 + 3;
                selectScreen_.currentPage_ = SELECT_PAGE_BANK_PATCHES;
            }
            mode_ = MODE_SELECT_SCREEN;
            updateNeeded = true;
        /*} else if (mode_ == MODE_INT_VALUE_SCREEN) {
            intResponse = intValueScreen_.Click();
            if (intResponse == INT_SCREEN_RESPONSE_EDIT) {
                menuActive_ = true;
            } else {
                menuActive_ = false;
            } // mode_ == INT_SCREEN_RESPONSE_NOEDIT
            updateNeeded = true;*/
        #ifdef __SETTINGS_SCREENS__
            } else if (mode_ == MODE_SETTINGS_SCREEN_1) {
                settingsScreen1_.Click();
                updateNeeded = true;
            } else if (mode_ == MODE_SETTINGS_SCREEN_2) {
                settingsScreen2_.Click();
                updateNeeded = true;
            } else if (mode_ == MODE_SETTINGS_SCREEN_3) {
                settingsScreen3_.Click();
                updateNeeded = true;
            } else if (mode_ == MODE_SETTINGS_SCREEN_4) {
                settingsScreen4_.Click();
                updateNeeded = true;
            } else if (mode_ == MODE_SETTINGS_SCREEN_5) {
                settingsScreen5_.Click();
                updateNeeded = true;
            } else if (mode_ == MODE_SETTINGS_SCREEN_6) {
                settings6Response = settingsScreen6_.Click();
                if (settings6Response == SETTINGS_SCREEN_6_RESPONSE_EDIT) {
                    menuActive_ = true;
                } else {
                    menuActive_ = false;
                }
                updateNeeded = true;
            } else if (mode_ == MODE_SETTINGS_SCREEN_7) {
                settings7Response = settingsScreen7_.Click();
                if (settings7Response == SETTINGS_SCREEN_7_RESPONSE_EDIT) {
                    menuActive_ = true;
                } else {
                    menuActive_ = false;
                }
                updateNeeded = true;
        #endif
        } else if (mode_ == MODE_PATCH_SCREEN) {
            // Send the click to the patch screen and then handle the response.
            patchResponse = patchScreen_.Click();
            if (patchResponse == PATCH_SCREEN_RESPONSE_EDIT) {
                menuActive_ = true;
            } else if (patchResponse == PATCH_SCREEN_RESPONSE_LOAD) {
                menuActive_ = true;
                selectScreen_.saving_ = false;
                selectScreen_.fromPlay_ = false;
                selectScreen_.currentPage_ = SELECT_PAGE_BANKS;
                selectScreen_.selection_ = 3;
                mode_ = MODE_SELECT_SCREEN;
            } else if (patchResponse == PATCH_SCREEN_RESPONSE_SAVE) {
                menuActive_ = true;
                selectScreen_.saving_ = true;
                selectScreen_.fromPlay_ = false;
                if (patch_->GetLiveMode()) {
                    selectScreen_.bankNumber_ = 0;
                    selectScreen_.patchNumber_ = 0;
                    selectScreen_.selection_ = 3;
                    selectScreen_.currentPage_ = SELECT_PAGE_BANKS;
                } else {
                    selectScreen_.bankNumber_ = patch_->GetBankNumber();
                    selectScreen_.patchNumber_ = patch_->GetPatchNumber() & ~7;
                    selectScreen_.selection_ = patch_->GetPatchNumber() % 8 + 3;
                    selectScreen_.currentPage_ = SELECT_PAGE_BANK_PATCHES;
                }
                mode_ = MODE_SELECT_SCREEN;
            } else if (patchResponse == PATCH_SCREEN_RESPONSE_PLAY) {
                menuActive_ = false;
                mode_ = MODE_PLAY;
            } else { // patchResponse == PATCH_SCREEN_RESPONSE_NOEDIT
                menuActive_ = false;
            }
            updateNeeded = true;
        } else if (mode_ == MODE_SELECT_SCREEN) {
            // Send the click to the patch select screen and then handle the response.
            selectResponse = selectScreen_.Click();
            if (selectResponse == SELECT_SCREEN_RESPONSE_PLAY) {
                prevSelectValue_ = patch_->voice1Settings_->getIntValue(GEN_SELECT); // To prevent buggy screen scrolling after load
                menuActive_ = false;
                mode_ = MODE_PLAY;
            } else if (selectResponse == SELECT_SCREEN_RESPONSE_CANCEL) {
                menuActive_ = false;
                mode_ = MODE_PATCH_SCREEN;
            }
            updateNeeded = true;
        } else if (mode_ == MODE_SYSTEM_SCREEN) {
            systemResponse = systemScreen_.Click();

            if (systemResponse == SYSTEM_SCREEN_RESPONSE_EDIT) {
                menuActive_ = true;
                updateNeeded = true;
            } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_NOEDIT) {
                menuActive_ = false;
                updateNeeded = true;
            } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_SYSEX_SEND) {
                systemScreen_.DisplaySending();
                kiwiSynth_->SendSysex();
                menuActive_ = false;
                updateNeeded = true;
            } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_SYSEX_READ) {
                systemScreen_.DisplayReceiving();
                kiwiSynth_->ReceiveSysex();
                menuActive_ = false;
                mode_ = MODE_PLAY;
                updateNeeded = true;
            } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_PANIC) {
                menuActive_ = false;
                kiwiSynth_->Panic();
                mode_ = MODE_PLAY;
                updateNeeded = true;
            } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_UPDATE) {
                // Enter bootloader mode to listen for a bios update
                mode_ = MODE_BOOTLOADER;
                Update(); // Update now instead of using updateNeeded because the next line will halt program execution
                System::ResetToBootloader(daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
            }
        }
    }

    return updateNeeded;
}



int Display::GetSelectValue(int numElements)
{
    int8_t value = patch_->voice1Settings_->getIntValue(GEN_SELECT);
    if (value < 0) {
        value += numElements;
    } else {
        value %= numElements;
    }

    patch_->voice1Settings_->setValue(GEN_SELECT, value);

    return value;
}



void Display::Update()
{
    // Pass on updating message to the relevant screen
    switch (mode_) {
        case MODE_PLAY:
            playScreen_.Display();
            break;
        case MODE_INT_VALUE_SCREEN:
            intValueScreen_.Display();
            break;
        #ifdef __SETTINGS_SCREENS__
            case MODE_SETTINGS_SCREEN_1:
                settingsScreen1_.Display();
                break;
            case MODE_SETTINGS_SCREEN_2:
                settingsScreen2_.Display();
                break;
            case MODE_SETTINGS_SCREEN_3:
                settingsScreen3_.Display();
                break;
            case MODE_SETTINGS_SCREEN_4:
                settingsScreen4_.Display();
                break;
            case MODE_SETTINGS_SCREEN_5:
                settingsScreen5_.Display();
                break;
            case MODE_SETTINGS_SCREEN_6:
                settingsScreen6_.Display();
                break;
            case MODE_SETTINGS_SCREEN_7:
                settingsScreen7_.Display();
                break;
        #endif
        case MODE_PATCH_SCREEN:
            patchScreen_.Display();
            break;
        case MODE_SELECT_SCREEN:
            selectScreen_.Display();
            break;
        case MODE_SYSTEM_SCREEN:
            systemScreen_.Display();
            break;
        case MODE_BOOTLOADER:
            bootloaderScreen_.Display();
            break;
    }
}
