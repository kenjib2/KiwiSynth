#ifndef __KIWI_SYNTH_GPIO_EXPANSION_H__
#define __KIWI_SYNTH_GPIO_EXPANSION_H__

#include <vector>
#include <map>

#include "daisy_seed.h"
#include "Control.h"
#include "ControlListener.h"
#include "KiwiMcp23017.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
	extern uint16_t pin0Value;
	extern uint16_t pinRead;

    extern bool gpioReadRequired;
    extern uint32_t gpioLastInterruptTime;
    extern char tempBuffer[256];

    /*
     * Internal function for interrupt callbacks.
     */
    void GpioExpansionInterruptCallback(Pin pin);

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
        uint16_t* pullupMap;
        uint16_t* activeMap;

        void Defaults()
        {
            uint16_t pullups[4];
            pullups[0] = 0xFFFF;
            pullups[1] = 0xFFFF;
            pullups[2] = 0xFFFF;
            pullups[3] = 0xFFFF;

            uint16_t actives[4];
            actives[0] = 0b1111111011111110;
            actives[1] = 0b1111110001111111;
            actives[2] = 0b1111111011111110;
            actives[3] = 0b1111110011111000;

            numGpioExpansions   = 4;
            interruptPin        = seed::D18;
            sdaPin              = seed::D11; // Pin 12 I2C1_SCL
            sclPin              = seed::D12; // Pin 13 I2C1_SDA
            pullupMap           = pullups;
            activeMap           = actives;
        }
    };

    class GpioExpansion : public Control
    {
        private:
            int numPins = 16;
            ControlListener* controlListener = nullptr;
            int controlId;

        public:
            int numGpioExpansions = 4;
            std::map<uint8_t, uint16_t> pinValues;
            std::vector<KiwiMcp23017> mcps;
            GPIO interrupt;
            GpioExpansion() {}
            ~GpioExpansion();
            void Init();
            void Init(GpioExpansionConfig *gpioExpansionConfig);

            void RegisterControlListener(ControlListener* controlListener, int controlId);
            /*
             * Checks to see if a read has been requested by the interrupt trigger. If so, read and update all GPIO pin values.
             */
            void Process();
            /*
             * Clears all interrupt flags.
             */
            void ClearInterrupts();

            uint16_t getPinValues(uint8_t address);
            bool getPinValue(uint8_t address, uint8_t pin);
    }; // class MultiPots

} // namespace kiwi_synth

#endif // __KIWI_SYNTH_GPIO_EXPANSION_H__
