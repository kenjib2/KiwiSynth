#include "Display.h"

namespace kiwi_synth
{

    void Display::Init(Patch* patch)
    {
        DisplayConfig cfg;
        cfg.Defaults();
        Init(&cfg, patch);
    }

    void Display::Init(DisplayConfig *displayConfig, Patch* patch)
    {
        KiwiDisplay::Config cfg;
        cfg.driver_config.transport_config.i2c_config.periph         = displayConfig->periph;
        cfg.driver_config.transport_config.i2c_config.speed          = displayConfig->speed;
        cfg.driver_config.transport_config.i2c_config.mode           = displayConfig->mode;
        cfg.driver_config.transport_config.i2c_config.pin_config.scl = displayConfig->sclPin;
        cfg.driver_config.transport_config.i2c_config.pin_config.sda = displayConfig->sdaPin;

        this->patch = patch;
        guiButton = false;
        mode = PLAY;

        welcomeScreen.Init(&display);
        bootloaderScreen.Init(&display);
        intValueScreen.Init(&display, patch);
        patchScreen.Init(&display, patch);

        display.Init(cfg);
        display.Fill(false);
    }

    void Display::HandleInput()
    {
        bool prevGuiButton = guiButton;
        guiButton = patch->activeSettings->getBoolValue(GEN_SELECT_BUTTON);
        if (prevGuiButton && !guiButton) {
            switch (mode) {
                case BOOTLOADER:
                    break;
                case PLAY:
                    mode = GUI;
                    patch->activeSettings->setValue(GEN_SELECT, (int8_t)0);
                    break;
                case GUI:
                    mode = PLAY;
                    Update();
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
                welcomeScreen.Display();
                break;
            case GUI:
                switch(GetSelectValue(2)) {
                    case 0:
                    default:
                        intValueScreen.Display();
                        break;
                    case 1:
                        patchScreen.Display();
                        // Add a int getSelectValue(int max) that reads in GEN_SELECT and then sets it back in patchSettings within a fixed range then returns that modulus value.
                        break;
                }
                break;
           case BOOTLOADER:
                bootloaderScreen.Display();
                break;
        }
    }
} // namespace kiwi_synth
