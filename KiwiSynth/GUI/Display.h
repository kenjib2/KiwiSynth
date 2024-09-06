#ifndef __KIWI_SYNTH_DISPLAY_H__
#define __KIWI_SYNTH_DISPLAY_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/PatchSettings.h"

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
            speed               = I2CHandle::Config::Speed::I2C_100KHZ;
            mode                = I2CHandle::Config::Mode::I2C_MASTER;
            sclPin              = seed::D12; // Pin 13 I2C1_SDA
            sdaPin              = seed::D11; // Pin 12 I2C1_SCL
        }
    };

    class Display
    {
        public:
            PatchSettings* patchSettings;

            Display(){}
            virtual ~Display(){};

            void Init();
            void Init(DisplayConfig *displayConfig);

            void OutputMessage(char* message);
            void Update();

        private:
            KiwiDisplay display;
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_DISPLAY_H__
