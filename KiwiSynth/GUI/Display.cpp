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
        prevSelectValue = patch->activeSettings->getIntValue(GEN_SELECT);

        welcomeScreen.Init(&display, patch);
        bootloaderScreen.Init(&display);
        intValueScreen.Init(&display, patch);
        patchScreen.Init(&display, kiwiSynth, patch);
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

        // Handle encoder rotations
        int newValue = patch->activeSettings->getIntValue(GEN_SELECT);
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
                switch (mode) {
                    case MODE_PATCH_SCREEN:
                        if (direction == 1) {
                            patchScreen.Increment();
                        } else if (direction == -1) {
                            patchScreen.Decrement();
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

        // Handle encoder button clicks
        bool prevGuiButton = guiButton;
        guiButton = patch->activeSettings->getBoolValue(GEN_SELECT_BUTTON);
        if (prevGuiButton && !guiButton) {
            switch (mode) {
                default:
                    break;
                case MODE_PLAY:
                    // Maybe load a patch?
                    break;

                case MODE_PATCH_SCREEN:
                    menuActive = patchScreen.Click();
                    updateNeeded = true;
                    break;

                case MODE_SYSTEM_SCREEN:
                    mode = MODE_BOOTLOADER;
                    Update(); // Update now instead of using updateNeeded because the next line will halt program execution
                    System::ResetToBootloader(daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
                    break;
            }
        }

        if (updateNeeded) {
            Update();
        }
    }
    
    int Display::GetSelectValue(int numElements)
    {
        int8_t value = patch->activeSettings->getIntValue(GEN_SELECT);
        if (value < 0) {
            value += numElements;
        } else {
            value %= numElements;
        }

        patch->activeSettings->setValue(GEN_SELECT, value);

        return value;
    }

    void Display::Update()
    {
        switch (mode) {
            case MODE_PLAY:
            default:
                welcomeScreen.Display();
                break;
            case MODE_INT_SCREEN:
                intValueScreen.Display();
                break;
            case MODE_PATCH_SCREEN:
                patchScreen.Display();
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
