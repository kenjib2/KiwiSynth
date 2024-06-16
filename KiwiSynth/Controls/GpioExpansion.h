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
    bool readRequired = false;
    uint32_t interruptTime = 0;
    uint32_t lastInterruptTime = 0;

    /*
     * Internal function for interrupt callbacks.
     */
    void IntCallback(Pin pin);

    /*
     * Internal function of type daisy::TimerHandle::PeriodElapsedCallback used for timer callbacks.
     */
    void ProcessTimer(void* data);

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
        int numPins;
        Pin interruptPin;
        Pin sdaPin;
        Pin sclPin;
        bool useTimer;
        int refreshRate;
        uint16_t* pullupMap;
    };

    class GpioExpansion : public Control
    {
        private:
            int numGpioExpansions = 4;
            int numPins = 16;
            GPIO interrupt;
            TimerHandle timer;
            std::vector<ControlListener*> controlListeners;
            std::vector<int> controlIds;
            std::vector<KiwiMcp23017> mcps;
            uint16_t* pinValues;

            void InitTimer(int refreshRate);
        public:
            GpioExpansion() {}
            ~GpioExpansion();
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
    }; // class MultiPots

} // namespace kiwi_synth

#endif // __KIWI_SYNTH_GPIO_EXPANSION_H__
