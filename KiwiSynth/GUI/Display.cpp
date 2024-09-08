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

        mode = PLAY;

        display.Init(cfg);
        display.Fill(false);
    }

    void Display::Clear()
    {
        display.Fill(false);
    }

    void Display::Update()
    {
        Clear();
        switch (mode) {
            case BOOTLOADER:
                DisplayBootloader();
                break;
            case PLAY:
                DisplayWelcome();
                break;
            case INTS:
                DisplayInts();
                break;
        }
    }

    void Display::DisplayBootloader()
    {
        Clear();

		sprintf(buffer, "Ready for Update...");
        display.SetCursor(0, 0);
        display.WriteString(buffer, Font_6x8, true);

        display.Update();
    }

    void Display::DisplayWelcome()
    {
        Clear();

	    sprintf(buffer, "KiwiSynth");
        display.SetCursor(0, 0);
        display.WriteString(buffer, Font_6x8, true);

	    /*sprintf(buffer, "Kiwi Synth");
        display.SetCursor(0, 0);
        display.WriteString(buffer, Font_11x18, true);

        display.SetCursor(0, 22);
	    sprintf(buffer, "Version 1.0");
        display.WriteString(buffer, Font_6x8, true);

        display.SetCursor(36, 56);
	    sprintf(buffer, "Bombastic Audio");
        display.WriteString(buffer, Font_6x8, true);*/

        display.Update();
    }

    // 2 character value
    void Display::GetVoiceMode(char* buffer)
    {
        switch (patchSettings->getIntValue(VCO_VOICES)) {
            case 0:
                strcpy(buffer, "2v");
                break;
            case 1:
                strcpy(buffer, "3v");
                break;
            case 2:
                strcpy(buffer, "1v");
                break;
        }
    }

    // 2 character value
    void Display::GetWaveform(char* buffer, int vcoNumber) {
        switch (patchSettings->getIntValue((PatchSetting)(VCO_1_WAVEFORM + vcoNumber))) {
            case 0:
                strcpy(buffer, "Sq");
                break;
            case 1:
                strcpy(buffer, "Sa");
                break;
            case 2:
                strcpy(buffer, "Tr");
                break;
        }
    }

    void Display::DisplayInts()
    {
        char val1[4], val2[4];

        display.SetCursor(0, 0);
        GetVoiceMode(val1);
        GetWaveform(val2, 0);
        sprintf(buffer, "vm %s w1 %s", val1, val2);
        display.WriteString(buffer, Font_6x8, true);

        display.SetCursor(0, 8);
        GetWaveform(val1, 1);
        sprintf(buffer, "w2 %s oct %d int %d", val1, patchSettings->getIntValue(VCO_2_OCTAVE) - 2, patchSettings->getIntValue(VCO_2_INTERVAL) - 11);
        display.WriteString(buffer, Font_6x8, true);

        display.SetCursor(0, 16);
        GetWaveform(val1, 2);
        sprintf(buffer, "w3 %s oct %d int %d", val1, patchSettings->getIntValue(VCO_3_OCTAVE) - 2, patchSettings->getIntValue(VCO_3_INTERVAL) - 11);
        display.WriteString(buffer, Font_6x8, true);

        display.Update();
    }

} // namespace kiwi_synth
