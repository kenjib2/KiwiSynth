#include "Display.h"

namespace kiwi_synth
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
        prevSelectValue = patch->voice1Settings->getIntValue(GEN_SELECT);

        playScreen.Init(&display, patch);
        bootloaderScreen.Init(&display);
        intValueScreen.Init(&display, patch);
        floatValue1Screen.Init(&display, patch);
        floatValue2Screen.Init(&display, patch);
        floatValue3Screen.Init(&display, patch);
        patchScreen.Init(&display, patch);
        selectScreen.Init(&display, kiwiSynth);
        systemScreen.Init(&display, performance);

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

        if (updateNeeded) {
            Update();
        }
    }

    bool Display::HandleEncoder() {
        bool updateNeeded = false;

        // Handle encoder rotations: Figure out if it changed. -1 is counterclockwise, 1 is clockwise, and 0 is no movement.
        int newValue = patch->voice1Settings->getIntValue(GEN_SELECT);
        int direction = 0;
        if (newValue == prevSelectValue) {
            direction = 0;
        } else {
            if ((newValue > prevSelectValue || (newValue == -128 && prevSelectValue == 127))
                && !(newValue == 127 && prevSelectValue == -128)) {
                direction = 1;
            } else if (newValue < prevSelectValue || (newValue == 127 && prevSelectValue == -128)) {
                direction = -1;
            }
            prevSelectValue = newValue;
        }

        if (direction) {
            if (menuActive) {
                // Handle menus within screens
                switch (mode) {
                    case MODE_INT_SCREEN:
                        if (direction == 1) {
                            intValueScreen.Increment();
                        } else if (direction == -1) {
                            intValueScreen.Decrement();
                        }
                        updateNeeded = true;
                        break;

                    case MODE_PATCH_SCREEN:
                        if (direction == 1) {
                            patchScreen.Increment();
                        } else if (direction == -1) {
                            patchScreen.Decrement();
                        }
                        updateNeeded = true;
                        break;

                    case MODE_SELECT_SCREEN:
                        if (direction == 1) {
                            selectScreen.Increment();
                        } else if (direction == -1) {
                            selectScreen.Decrement();
                        }
                        updateNeeded = true;
                        break;

                    case MODE_SYSTEM_SCREEN:
                        if (direction == 1) {
                            systemScreen.Increment();
                        } else if (direction == -1) {
                            systemScreen.Decrement();
                        }
                        updateNeeded = true;
                        break;

                    default:
                        break;
                }
            } else {
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
            }
        }

        return updateNeeded;
    }

    bool Display::HandleClick() {
        bool updateNeeded = false;

        // Handle encoder button clicks
        bool prevGuiButton = guiButton;
        guiButton = patch->voice1Settings->getBoolValue(GEN_SELECT_BUTTON);
        if (prevGuiButton && !guiButton) {
            PatchScreenResponse patchResponse;
            SelectScreenResponse selectResponse;
            SystemScreenResponse systemResponse;
            IntScreenResponse intResponse;

            switch (mode) {
                default:
                    break;
                case MODE_PLAY:
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
                    break;
                
                case MODE_INT_SCREEN:
                    intResponse = intValueScreen.Click();
                    switch (intResponse) {
                        case INT_SCREEN_RESPONSE_EDIT:
                            menuActive = true;
                            break;
                        case INT_SCREEN_RESPONSE_NOEDIT:
                            menuActive = false;
                            break;
                    }
                    updateNeeded = true;
                    break;

                case MODE_FLOAT_1_SCREEN:
                    floatValue1Screen.Click();
                    updateNeeded = true;
                    break;

                case MODE_FLOAT_2_SCREEN:
                    floatValue2Screen.Click();
                    updateNeeded = true;
                    break;

                case MODE_FLOAT_3_SCREEN:
                    floatValue3Screen.Click();
                    updateNeeded = true;
                    break;

                case MODE_PATCH_SCREEN:
                    // Send the click to the patch screen and then handle the response.
                    patchResponse = patchScreen.Click();
                    switch (patchResponse) {
                        case PATCH_SCREEN_RESPONSE_EDIT:
                            menuActive = true;
                            break;
                        default:
                        case PATCH_SCREEN_RESPONSE_NOEDIT:
                            menuActive = false;
                            break;
                        case PATCH_SCREEN_RESPONSE_LOAD:
                            menuActive = true;
                            selectScreen.saving = false;
                            selectScreen.fromPlay = false;
                            selectScreen.currentPage = SELECT_PAGE_BANKS;
                            selectScreen.selection = 3;
                            mode = MODE_SELECT_SCREEN;
                            break;
                        case PATCH_SCREEN_RESPONSE_SAVE:
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
                            break;
                        case PATCH_SCREEN_RESPONSE_PLAY:
                            menuActive = false;
                            mode = MODE_PLAY;
                            break;
                    }
                    updateNeeded = true;
                    break;

                case MODE_SELECT_SCREEN:
                    // Send the click to the patch select screen and then handle the response.
                    selectResponse = selectScreen.Click();
                    switch (selectResponse) {
                        default:
                        case SELECT_SCREEN_RESPONSE_CANCEL:
                            menuActive = false;
                            mode = MODE_PATCH_SCREEN;
                            break;
                        case SELECT_SCREEN_RESPONSE_REFRESH:
                            break;
                        case SELECT_SCREEN_RESPONSE_PLAY:
                            prevSelectValue = patch->voice1Settings->getIntValue(GEN_SELECT); // To prevent buggy screen scrolling after load
                            menuActive = false;
                            mode = MODE_PLAY;
                            break;
                    }
                    updateNeeded = true;
                    break;

                case MODE_SYSTEM_SCREEN:
                    systemResponse = systemScreen.Click();

                    switch (systemResponse)  {
                        case SYSTEM_SCREEN_RESPONSE_EDIT:
                            menuActive = true;
                            break;
                        case SYSTEM_SCREEN_RESPONSE_NOEDIT:
                            menuActive = false;
                            break;
                        case SYSTEM_SCREEN_RESPONSE_PANIC:
                            menuActive = false;
                            kiwiSynth->Panic();
                            mode = MODE_PLAY;
                            updateNeeded = true;
                            break;
                        case SYSTEM_SCREEN_RESPONSE_UPDATE:
                            // Enter bootloader mode to listen for a bios update
                            mode = MODE_BOOTLOADER;
                            Update(); // Update now instead of using updateNeeded because the next line will halt program execution
                            System::ResetToBootloader(daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
                            break;
                    }
                    break;
            }
        }

        return updateNeeded;
    }

    int Display::GetSelectValue(int numElements)
    {
        int8_t value = patch->voice1Settings->getIntValue(GEN_SELECT);
        if (value < 0) {
            value += numElements;
        } else {
            value %= numElements;
        }

        patch->voice1Settings->setValue(GEN_SELECT, value);

        return value;
    }

    void Display::Update()
    {
        // Pass on updating message to the relevant screen
        switch (mode) {
            case MODE_PLAY:
            default:
                playScreen.Display();
                break;
            case MODE_INT_SCREEN:
                intValueScreen.Display();
                break;
            case MODE_FLOAT_1_SCREEN:
                floatValue1Screen.Display();
                break;
            case MODE_FLOAT_2_SCREEN:
                floatValue2Screen.Display();
                break;
            case MODE_FLOAT_3_SCREEN:
                floatValue3Screen.Display();
                break;
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

} // namespace kiwi_synth
