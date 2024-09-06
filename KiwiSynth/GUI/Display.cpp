#include "Display.h"

namespace kiwi_synth
{

    void Display::Init()
    {
        DisplayConfig cfg;
        cfg.Defaults();
        Init(&cfg);
    }

    void Display::Init(DisplayConfig *displayConfig)
    {
        KiwiDisplay::Config cfg;
        cfg.driver_config.transport_config.i2c_config.periph         = displayConfig->periph;
        cfg.driver_config.transport_config.i2c_config.speed          = displayConfig->speed;
        cfg.driver_config.transport_config.i2c_config.mode           = displayConfig->mode;
        cfg.driver_config.transport_config.i2c_config.pin_config.scl = displayConfig->sclPin;
        cfg.driver_config.transport_config.i2c_config.pin_config.sda = displayConfig->sdaPin;

        display.Init(cfg);
        display.Fill(false);
    }

    void Display::OutputMessage(char* message)
    {
        display.SetCursor(0, 0);
        display.WriteString(message, Font_6x8, true);
        display.Update();
    }

    void Display::Update()
    { 
        static int counter = 0;
        char buffer[16];
        sprintf(buffer, "Count %d", counter++);
        //sprintf(buffer, "Test %d", patchSettings->getIntValue(patchSettings->lastChangedSetting));
        //display.SetCursor(xPos[patchSettings->lastChangedSetting], yPos[patchSettings->lastChangedSetting]);
        display.SetCursor(0, 0);
        display.WriteString(buffer, Font_6x8, true);
        display.Update();
    }
} // namespace kiwi_synth
