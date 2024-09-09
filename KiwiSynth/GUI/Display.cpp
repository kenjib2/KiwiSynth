#include "Display.h"

namespace kiwi_synth
{

    void Display::Init(PatchSettings* patchSettings)
    {
        DisplayConfig cfg;
        cfg.Defaults();
        Init(&cfg, patchSettings);
    }

    void Display::Init(DisplayConfig *displayConfig, PatchSettings* patchSettings)
    {
        KiwiDisplay::Config cfg;
        cfg.driver_config.transport_config.i2c_config.periph         = displayConfig->periph;
        cfg.driver_config.transport_config.i2c_config.speed          = displayConfig->speed;
        cfg.driver_config.transport_config.i2c_config.mode           = displayConfig->mode;
        cfg.driver_config.transport_config.i2c_config.pin_config.scl = displayConfig->sclPin;
        cfg.driver_config.transport_config.i2c_config.pin_config.sda = displayConfig->sdaPin;

        this->patchSettings = patchSettings;
        mode = PLAY;

        welcomeScreen.Init(&display, patchSettings);
        bootloaderScreen.Init(&display, patchSettings);
        intValueScreen.Init(&display, patchSettings);
        patchScreen.Init(&display, patchSettings);

        display.Init(cfg);
        display.Fill(false);
    }

    int Display::GetSelectValue(int numElements)
    {
        int8_t value = patchSettings->getIntValue(GEN_SELECT);
        if (value < 0) {
            value += numElements;
        } else {
            value %= numElements;
        }

        patchSettings->setValue(GEN_SELECT, value);

        return value + 1;
    }

    void Display::Update()
    {
        switch (mode) {
            case BOOTLOADER:
                bootloaderScreen.Display();
                break;
            case PLAY:
                welcomeScreen.Display();
                break;
            case INTS:
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
        }
    }
} // namespace kiwi_synth
