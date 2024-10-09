#ifndef __KIWI_SYNTH_DISPLAY_H__
#define __KIWI_SYNTH_DISPLAY_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../KiwiSynth.h"
#include "../../KUtils.h"
#include "PlayScreen.h"
#include "BootloaderScreen.h"
#include "IntValueScreen.h"
#include "FloatValue1Screen.h"
#include "FloatValue2Screen.h"
#include "FloatValue3Screen.h"
#include "PatchScreen.h"
#include "SelectScreen.h"
#include "SystemScreen.h"

using namespace daisy;
using namespace daisy::seed;
using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
   /*
    * Used to initialize a MultiPots object.
    *
    * numGpioExpansions: The number of expansions attached.
    * interruptPin: The pin used for GPIO Expansion interrupts.
    * sdaPin: The pin used for I2C SDA.
    * sdlPin: The pin used for I2C SCL.
    * useTimer: If true, a timer interrupt will be used to automatically pins if there has been a read request. If set to false, Process()
    *     will need to be called manually to read pins with a read request.
    * refreshRate: If useTimer = true, this will be how often the data is refreshed measured in hundredths of a millisecond.
    * pullupMap: An array of uint16_t values. Each of which has a bit for each pin position. For each position, a 0 indicates no pullup
    *     and a 1 indicates an internal pullup.
    * activeMap: An array of uint16_t values. Each of which has a bit for each pin position. For each position, a 0 indicates that the pin
    *     is being used and should be read. A 1 indicates that the pin is not in use and might possibly be floating.
    */
    struct DisplayConfig
    {
        I2CHandle::Config::Peripheral periph;
        I2CHandle::Config::Speed speed;
        I2CHandle::Config::Mode mode;
        Pin sclPin;
        Pin sdaPin;

        void Defaults()
        {
            periph              = I2CHandle::Config::Peripheral::I2C_1;
            speed               = I2CHandle::Config::Speed::I2C_400KHZ;
            mode                = I2CHandle::Config::Mode::I2C_MASTER;
            sclPin              = seed::D12; // Pin 13 I2C1_SDA
            sdaPin              = seed::D11; // Pin 12 I2C1_SCL
        }
    };

    /*
     * These are all of the possible screens of data that can be displayed on the Kiwi Synth. 
     * values 0 and greater can be scrolled through with the select knob. The other two screens
     * can only be entered when selected from other screens.
     */
    typedef enum {
        MODE_BOOTLOADER = -2,
        MODE_SELECT_SCREEN = -1,
        MODE_PLAY = 0,
        MODE_INT_SCREEN,
        MODE_FLOAT_1_SCREEN,
        MODE_FLOAT_2_SCREEN,
        MODE_FLOAT_3_SCREEN,
        MODE_PATCH_SCREEN,
        MODE_SYSTEM_SCREEN,
    } DisplayMode;
    const int DISPLAY_MODE_OPTIONS = 7; // Not including MODE_SELECT_SCREEN or MODE_BOOTLOADER

    /*
     * The display class is the main controller for the synth GUI. It handles input, manages which
     * pages are visible, and passes control messages on to whichever screen is active.
     */
    class Display
    {
        public:
            DisplayMode mode;

            Display(){}
            virtual ~Display(){};
            void Init(KiwiSynth* kiwiSynth, Performance* performance);
            void Init(DisplayConfig *displayConfig, KiwiSynth* kiwiSynth, Performance* performance);

            /*
             * Poll for input signals and then pass them on to whichever screen is active.
             */
            void HandleInput();
            void Update();

        private:
            bool guiButton;
            bool menuActive;
            int prevSelectValue;
            KiwiSynth* kiwiSynth;
            Patch* patch;
            Performance* performance;
            KiwiDisplay display;

            PlayScreen playScreen;
            BootloaderScreen bootloaderScreen;
            IntValueScreen intValueScreen;
            FloatValue1Screen floatValue1Screen;
            FloatValue2Screen floatValue2Screen;
            FloatValue3Screen floatValue3Screen;
            PatchScreen patchScreen;
            SelectScreen selectScreen;
            SystemScreen systemScreen;

            bool HandleEncoder();
            bool HandleClick();
            /*
             * Manages the selection values for various screens.
             */    
            int GetSelectValue(int numElements);
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_DISPLAY_H__
