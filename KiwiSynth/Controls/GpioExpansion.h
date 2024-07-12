#ifndef __KIWI_SYNTH_GPIO_EXPANSION_H__
#define __KIWI_SYNTH_GPIO_EXPANSION_H__

#include <vector>

#include "daisy_seed.h"
#include "Control.h"
#include "ControlListener.h"
#include "KiwiMcp23017.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    //extern KiwiMcp23017 mcp;
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
     * Internal function of type daisy::TimerHandle::PeriodElapsedCallback used for timer callbacks.
     */
    void ProcessGpioExpansionTimer(void* data);

   /*
    * Used to initialize a MultiPots object.
    *
    * numGpioExpansions: The number of expansions attached.
    * numPins: The number of pins each expansion has. This should be either 8 or 16. Other values are not supported
    *     and all multiplexers must have the same number of channels.
    * interruptPin: The pin used for GPIO Expansion interrupts.
    * sdaPin: The pin used for I2C SDA.
    * sdlPin: The pin used for I2C SCL.
    * useTimer: If true, a timer interrupt will be used to automatically pins if there has been a read request. If set to false, Process()
    *     will need to be called manually to read pins with a read request.
    * refreshRate: If useTimer = true, this will be how often the data is refreshed measured in hundredths of a millisecond.
    * pullupMap: An array of uint16_t values. Each of which has a bit for each pin position. For each position, a 0 indicates no pullup
    *     and a 1 indicates an internal pullup.
    */
    struct GpioExpansionConfig
    {
        int numGpioExpansions;
        Pin interruptPin;
        Pin sdaPin;
        Pin sclPin;
        bool useTimer;
        int refreshRate;
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
            useTimer            = true;
            refreshRate         = 50;
            pullupMap           = pullups;
            activeMap           = actives;
        }
    };

    class GpioExpansion : public Control
    {
        private:
            int numPins = 16;
            TimerHandle timer;
            ControlListener* controlListener = nullptr;
            int controlId;

            void InitTimer(int refreshRate);
        public:
            int numGpioExpansions = 4;
            uint16_t* pinValues;
            std::vector<KiwiMcp23017> mcps;
            GPIO interrupt;
            GpioExpansion() {}
            ~GpioExpansion();
            void Init();
            void Init(GpioExpansionConfig *gpioExpansionConfig);

            void RegisterControlListener(ControlListener* controlListener, int controlId);
            /*
            * Starts the input read timer running.
            */
            void StartTimer();
            /*
             * Checks to see if a read has been requested by the interrupt trigger. If so, read and update all GPIO pin values.
             */
            void Process();
            /*
             * Clears all interrupt flags.
             */
            void ClearInterrupts();

            uint16_t getPinValues(int expansionNumber);
    }; // class MultiPots

} // namespace kiwi_synth

#endif // __KIWI_SYNTH_GPIO_EXPANSION_H__
