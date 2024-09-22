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
        mode = PLAY;

        welcomeScreen.Init(&display);
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
        DisplayMode newMode = (DisplayMode)GetSelectValue(DISPLAY_MODE_MAX);
        if (newMode != mode) {
            mode = newMode;
            Update();
        }

        bool prevGuiButton = guiButton;
        guiButton = patch->activeSettings->getBoolValue(GEN_SELECT_BUTTON);
        if (prevGuiButton && !guiButton) {
            switch (mode) {
                default:
                    break;
                case PLAY:
                    patch->setReverbMode((ReverbMode)(patch->getReverbMode() + 1));
                    break;
                case PATCH_SCREEN:
                    patch->setEffectsMode((EffectsMode)(patch->getEffectsMode() + 1));
                    Update();
                    break;
                case SYSTEM_SCREEN:
                    mode = BOOTLOADER;
                    Update();
                    System::ResetToBootloader(daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
                    break;
            }
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
            case PLAY:
            default:
                welcomeScreen.Display();
                break;
            case INT_SCREEN:
                intValueScreen.Display();
                break;
            case PATCH_SCREEN:
                patchScreen.Display();
                break;
            case SYSTEM_SCREEN:
                systemScreen.Display();
                break;
            case BOOTLOADER:
                bootloaderScreen.Display();
                break;
        }
    }
} // namespace kiwi_synth
