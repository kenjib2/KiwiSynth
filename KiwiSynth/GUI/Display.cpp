#include "Display.h"

namespace kiwisynth
{

    void Display::Init(KiwiSynth* kiwiSynth, Performance* performance)
    {
        DisplayConfig cfg;
        cfg.Defaults();
        Init(&cfg, kiwiSynth, performance);
    }

    void Display::Init(DisplayConfig *displayConfig, KiwiSynth* kiwiSynth, Performance* performance)
    {
        this->kiwiSynth = kiwiSynth;
        patch = &(kiwiSynth->patch);
        this->performance = performance;
        guiButton = false;
        mode = MODE_PLAY;
        menuActive = false;
        prevSelectValue = patch->voice1Settings_->getIntValue(GEN_SELECT);

        playScreen.Init(&display, patch);
        bootloaderScreen.Init(&display);
        patchScreen.Init(&display, patch);
        selectScreen.Init(&display, kiwiSynth);
        systemScreen.Init(&display, performance);
        intValueScreen.Init(&display, patch);
        #ifdef __SETTINGS_SCREENS__
            settingsScreen1.Init(&display, patch);
            settingsScreen2.Init(&display, patch);
            settingsScreen3.Init(&display, patch);
            settingsScreen4.Init(&display, patch);
            settingsScreen5.Init(&display, patch);
            settingsScreen6.Init(&display, patch);
            settingsScreen7.Init(&display, patch);
        #endif

        KiwiDisplay::Config cfg;
        cfg.driver_config.transport_config.i2c_config.periph         = displayConfig->periph;
        cfg.driver_config.transport_config.i2c_config.speed          = displayConfig->speed;
        cfg.driver_config.transport_config.i2c_config.mode           = displayConfig->mode;
        cfg.driver_config.transport_config.i2c_config.pin_config.scl = displayConfig->sclPin;
        cfg.driver_config.transport_config.i2c_config.pin_config.sda = displayConfig->sdaPin;

        display.Init(cfg);
        display.Fill(false);
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
        int newValue = patch->voice1Settings_->getIntValue(GEN_SELECT);
        int direction = 0;
        if (__builtin_expect(newValue != prevSelectValue, 0)) {
            if ((newValue > prevSelectValue || (newValue == -128 && prevSelectValue == 127))
                && !(newValue == 127 && prevSelectValue == -128)) {
                direction = 1;
            } else if (newValue < prevSelectValue || (newValue == 127 && prevSelectValue == -128)) {
                direction = -1;
            }
            prevSelectValue = newValue;

            if (__builtin_expect(!menuActive, 1)) {
                // Cycle through display screens
                if (direction == 1) {
                    if (mode + 1 < DISPLAY_MODE_OPTIONS) {
                        mode = (DisplayMode)(mode + 1);
                    } else {
                        mode = MODE_PLAY;
                    }
                } else if (direction == -1) {
                    if (mode - 1 < 0) {
                        mode = (DisplayMode)(DISPLAY_MODE_OPTIONS - 1);
                    } else {
                        mode = (DisplayMode)(mode - 1);
                    }
                }
                updateNeeded = true;
            } else {
                // Handle menus within screens
                if (__builtin_expect(mode == MODE_PLAY, 1)) {
                } else if (mode == MODE_PATCH_SCREEN) {
                    if (direction == 1) {
                        patchScreen.Increment();
                    } else if (direction == -1) {
                        patchScreen.Decrement();
                    }
                    updateNeeded = true;
                /*} else if (mode == MODE_INT_VALUE_SCREEN) {
                    if (direction == 1) {
                        intValueScreen.Increment();
                    } else if (direction == -1) {
                        intValueScreen.Decrement();
                    }
                    updateNeeded = true;*/
                #ifdef __SETTINGS_SCREENS__
                    } else if (mode == MODE_SETTINGS_SCREEN_6) {
                        if (direction == 1) {
                            settingsScreen6.Increment();
                        } else if (direction == -1) {
                            settingsScreen6.Decrement();
                        }
                        updateNeeded = true;
                    } else if (mode == MODE_SETTINGS_SCREEN_7) {
                        if (direction == 1) {
                            settingsScreen7.Increment();
                        } else if (direction == -1) {
                            settingsScreen7.Decrement();
                        }
                        updateNeeded = true;
                #endif
                } else if (mode == MODE_SELECT_SCREEN) {
                    if (direction == 1) {
                        selectScreen.Increment();
                    } else if (direction == -1) {
                        selectScreen.Decrement();
                    }
                    updateNeeded = true;
                } else if (mode == MODE_SYSTEM_SCREEN) {
                    if (direction == 1) {
                        systemScreen.Increment();
                    } else if (direction == -1) {
                        systemScreen.Decrement();
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
        bool prevGuiButton = guiButton;
        guiButton = patch->voice1Settings_->getBoolValue(GEN_SELECT_BUTTON);
        if (__builtin_expect(prevGuiButton && !guiButton, 0)) {
            PatchScreenResponse patchResponse;
            SelectScreenResponse selectResponse;
            //IntScreenResponse intResponse;
            SystemScreenResponse systemResponse;
            #ifdef __SETTINGS_SCREENS__
                SettingsScreen6Response settings6Response;
                SettingsScreen7Response settings7Response;
            #endif

            if (__builtin_expect(mode == MODE_PLAY, 1)) {
                // From play mode we want to load patches, starting positioned at the current patch for convenience.
                menuActive = true;
                selectScreen.saving = false;
                selectScreen.fromPlay = true;
                if (patch->GetLiveMode()) {
                    selectScreen.bankNumber = 0;
                    selectScreen.patchNumber = 0;
                    selectScreen.selection = 3;
                    selectScreen.currentPage = SELECT_PAGE_BANKS;
                } else {
                    selectScreen.bankNumber = patch->GetBankNumber();
                    selectScreen.patchNumber = patch->GetPatchNumber() & ~7;
                    selectScreen.selection = patch->GetPatchNumber() % 8 + 3;
                    selectScreen.currentPage = SELECT_PAGE_BANK_PATCHES;
                }
                mode = MODE_SELECT_SCREEN;
                updateNeeded = true;
            /*} else if (mode == MODE_INT_VALUE_SCREEN) {
                intResponse = intValueScreen.Click();
                if (intResponse == INT_SCREEN_RESPONSE_EDIT) {
                    menuActive = true;
                } else {
                    menuActive = false;
                } // mode == INT_SCREEN_RESPONSE_NOEDIT
                updateNeeded = true;*/
            #ifdef __SETTINGS_SCREENS__
                } else if (mode == MODE_SETTINGS_SCREEN_1) {
                    settingsScreen1.Click();
                    updateNeeded = true;
                } else if (mode == MODE_SETTINGS_SCREEN_2) {
                    settingsScreen2.Click();
                    updateNeeded = true;
                } else if (mode == MODE_SETTINGS_SCREEN_3) {
                    settingsScreen3.Click();
                    updateNeeded = true;
                } else if (mode == MODE_SETTINGS_SCREEN_4) {
                    settingsScreen4.Click();
                    updateNeeded = true;
                } else if (mode == MODE_SETTINGS_SCREEN_5) {
                    settingsScreen5.Click();
                    updateNeeded = true;
                } else if (mode == MODE_SETTINGS_SCREEN_6) {
                    settings6Response = settingsScreen6.Click();
                    if (settings6Response == SETTINGS_SCREEN_6_RESPONSE_EDIT) {
                        menuActive = true;
                    } else {
                        menuActive = false;
                    }
                    updateNeeded = true;
                } else if (mode == MODE_SETTINGS_SCREEN_7) {
                    settings7Response = settingsScreen7.Click();
                    if (settings7Response == SETTINGS_SCREEN_7_RESPONSE_EDIT) {
                        menuActive = true;
                    } else {
                        menuActive = false;
                    }
                    updateNeeded = true;
            #endif
            } else if (mode == MODE_PATCH_SCREEN) {
                // Send the click to the patch screen and then handle the response.
                patchResponse = patchScreen.Click();
                if (patchResponse == PATCH_SCREEN_RESPONSE_EDIT) {
                    menuActive = true;
                } else if (patchResponse == PATCH_SCREEN_RESPONSE_LOAD) {
                    menuActive = true;
                    selectScreen.saving = false;
                    selectScreen.fromPlay = false;
                    selectScreen.currentPage = SELECT_PAGE_BANKS;
                    selectScreen.selection = 3;
                    mode = MODE_SELECT_SCREEN;
                } else if (patchResponse == PATCH_SCREEN_RESPONSE_SAVE) {
                    menuActive = true;
                    selectScreen.saving = true;
                    selectScreen.fromPlay = false;
                    if (patch->GetLiveMode()) {
                        selectScreen.bankNumber = 0;
                        selectScreen.patchNumber = 0;
                        selectScreen.selection = 3;
                        selectScreen.currentPage = SELECT_PAGE_BANKS;
                    } else {
                        selectScreen.bankNumber = patch->GetBankNumber();
                        selectScreen.patchNumber = patch->GetPatchNumber() & ~7;
                        selectScreen.selection = patch->GetPatchNumber() % 8 + 3;
                        selectScreen.currentPage = SELECT_PAGE_BANK_PATCHES;
                    }
                    mode = MODE_SELECT_SCREEN;
                } else if (patchResponse == PATCH_SCREEN_RESPONSE_PLAY) {
                    menuActive = false;
                    mode = MODE_PLAY;
                } else { // patchResponse == PATCH_SCREEN_RESPONSE_NOEDIT
                    menuActive = false;
                }
                updateNeeded = true;
            } else if (mode == MODE_SELECT_SCREEN) {
                // Send the click to the patch select screen and then handle the response.
                selectResponse = selectScreen.Click();
                if (selectResponse == SELECT_SCREEN_RESPONSE_PLAY) {
                    prevSelectValue = patch->voice1Settings_->getIntValue(GEN_SELECT); // To prevent buggy screen scrolling after load
                    menuActive = false;
                    mode = MODE_PLAY;
                } else if (selectResponse == SELECT_SCREEN_RESPONSE_CANCEL) {
                    menuActive = false;
                    mode = MODE_PATCH_SCREEN;
                }
                updateNeeded = true;
            } else if (mode == MODE_SYSTEM_SCREEN) {
                systemResponse = systemScreen.Click();

                if (systemResponse == SYSTEM_SCREEN_RESPONSE_EDIT) {
                    menuActive = true;
                    updateNeeded = true;
                } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_NOEDIT) {
                    menuActive = false;
                    updateNeeded = true;
                } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_SYSEX_SEND) {
                    systemScreen.DisplaySending();
                    kiwiSynth->SendSysex();
                    menuActive = false;
                    updateNeeded = true;
                } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_SYSEX_READ) {
                    systemScreen.DisplayReceiving();
                    kiwiSynth->ReceiveSysex();
                    menuActive = false;
                    mode = MODE_PLAY;
                    updateNeeded = true;
                } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_PANIC) {
                    menuActive = false;
                    kiwiSynth->Panic();
                    mode = MODE_PLAY;
                    updateNeeded = true;
                } else if (systemResponse == SYSTEM_SCREEN_RESPONSE_UPDATE) {
                    // Enter bootloader mode to listen for a bios update
                    mode = MODE_BOOTLOADER;
                    Update(); // Update now instead of using updateNeeded because the next line will halt program execution
                    System::ResetToBootloader(daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
                }
            }
        }

        return updateNeeded;
    }

    int Display::GetSelectValue(int numElements)
    {
        int8_t value = patch->voice1Settings_->getIntValue(GEN_SELECT);
        if (value < 0) {
            value += numElements;
        } else {
            value %= numElements;
        }

        patch->voice1Settings_->setValue(GEN_SELECT, value);

        return value;
    }

    void Display::Update()
    {
        // Pass on updating message to the relevant screen
        switch (mode) {
            case MODE_PLAY:
                playScreen.Display();
                break;
            case MODE_INT_VALUE_SCREEN:
                intValueScreen.Display();
                break;
            #ifdef __SETTINGS_SCREENS__
                case MODE_SETTINGS_SCREEN_1:
                    settingsScreen1.Display();
                    break;
                case MODE_SETTINGS_SCREEN_2:
                    settingsScreen2.Display();
                    break;
                case MODE_SETTINGS_SCREEN_3:
                    settingsScreen3.Display();
                    break;
                case MODE_SETTINGS_SCREEN_4:
                    settingsScreen4.Display();
                    break;
                case MODE_SETTINGS_SCREEN_5:
                    settingsScreen5.Display();
                    break;
                case MODE_SETTINGS_SCREEN_6:
                    settingsScreen6.Display();
                    break;
                case MODE_SETTINGS_SCREEN_7:
                    settingsScreen7.Display();
                    break;
            #endif
            case MODE_PATCH_SCREEN:
                patchScreen.Display();
                break;
            case MODE_SELECT_SCREEN:
                selectScreen.Display();
                break;
            case MODE_SYSTEM_SCREEN:
                systemScreen.Display();
                break;
            case MODE_BOOTLOADER:
                bootloaderScreen.Display();
                break;
        }
    }

} // namespace kiwisynth
