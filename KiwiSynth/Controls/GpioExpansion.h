#ifndef __KIWI_SYNTH_GPIO_EXPANSION_H__
#define __KIWI_SYNTH_GPIO_EXPANSION_H__

#include <vector>
#include <map>

#include "daisy_seed.h"
#include "IControl.h"
#include "IControlListener.h"
#include "KiwiMcp23017.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
   /*
    * Used to initialize a GpioExpansion object.
    *
    * numGpioExpansions: The number of expansions attached.
    * interruptPin: The pin used for GPIO Expansion interrupts.
    * sdaPin: The pin used for I2C SDA.
    * sdlPin: The pin used for I2C SCL.
    * pullupMap: An array of uint16_t values. Each of which has a bit for each pin position. For each position, a 0 indicates no pullup
    *     and a 1 indicates an internal pullup.
    * activeMap: An array of uint16_t values. Each of which has a bit for each pin position. For each position, a 0 indicates that the pin
    *     is being used and should be read. A 1 indicates that the pin is not in use and might possibly be floating.
    */
    struct GpioExpansionConfig
    {
        int numGpioExpansions;
        Pin interruptPin;
        Pin sdaPin;
        Pin sclPin;
        uint16_t pullupMap[4];
        uint16_t activeMap[4];

        void Defaults()
        {
            // All A7 and B7 must be pullup to avoid a crash due to chip bug
            pullupMap[0] = 0b1111111111111111;
            pullupMap[1] = 0b1111111111111111;
            pullupMap[2] = 0b1111111111111111;
            pullupMap[3] = 0b1001111110111111;

            activeMap[0] = 0b0111111101111111;
            activeMap[1] = 0b0011111101111111;
            activeMap[2] = 0b0111111101111111;
            activeMap[3] = 0b0011111100011111;

            numGpioExpansions   = 4;
            interruptPin        = seed::D18;
            sdaPin              = seed::D11; // Pin 12 I2C1_SCL
            sclPin              = seed::D12; // Pin 13 I2C1_SDA
        }
    };

    class GpioExpansion : public IControl
    {
        private:
            static const int NUM_PINS = 16;
            int numGpioExpansions = 4;
            IControlListener* controlListener = nullptr;

        public:
            bool bitValues[4][16];
            std::vector<KiwiMcp23017> mcps;
            GPIO interrupt;
            GpioExpansion() {}
            ~GpioExpansion() {}
            void Init();
            void Init(GpioExpansionConfig *gpioExpansionConfig);
            inline void RegisterControlListener(IControlListener* controlListener) { this->controlListener = controlListener; }

            /*
             * Checks to see if a read has been requested by the interrupt trigger. If so, read and update all GPIO pin values.
             * Returns true if there was a read and false if not.
             */
            bool Process(bool forceRead = false);
            /*
             * Clears all interrupt flags.
             */
            inline void ClearInterrupts() { for (int i = 0; i < numGpioExpansions; i++) { mcps.at(i).clearInterrupts(); } }
            /*
             * Retrieves a GPIO pin value from cached values stored in memory.
             */
            inline bool getPinValue(uint8_t address, uint8_t pin) { return bitValues[address - 0x20][pin]; }

    }; // class MultiPots

} // namespace kiwi_synth

#endif // __KIWI_SYNTH_GPIO_EXPANSION_H__
