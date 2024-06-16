#include "GpioExpansion.h"

namespace kiwi_synth
{

    void IntCallback(Pin pin)
    {
        uint32_t now = System::GetUs();
        if (now - interruptTime > 500 || interruptTime > now) {
            lastInterruptTime = interruptTime;
            interruptTime = now;
            readRequired = true;
        }
    }

    void ProcessTimer(void* gpioExpansion)
    {
        if (readRequired) {
            readRequired = false;
            ((GpioExpansion*)gpioExpansion)->Process();
        }
    }

    GpioExpansion::~GpioExpansion()
    {
        free(pinValues);
    }


    void GpioExpansion::Init(GpioExpansionConfig *gpioExpansionConfig)
    {
        numGpioExpansions = gpioExpansionConfig->numGpioExpansions;
        numPins = gpioExpansionConfig->numPins;
        pinValues = (uint16_t*)malloc(numPins * sizeof(uint16_t));
        readRequired = true;

        for (int i = 0; i < numGpioExpansions; i++) {
            KiwiMcp23017 mcp;
            KiwiMcp23017::Config cfg;
            cfg.transport_config.Defaults();
            cfg.transport_config.i2c_config.address = i;
            cfg.transport_config.i2c_config.pin_config.sda = gpioExpansionConfig->sdaPin;
            cfg.transport_config.i2c_config.pin_config.scl = gpioExpansionConfig->sclPin;

            mcp.Init(cfg);

            uint8_t pullupsPortA = 0xFF;
            uint8_t pullupsPortB = 0xFF;
            if (gpioExpansionConfig->pullupMap) {
                pullupsPortA = (gpioExpansionConfig->pullupMap[i] & 0b1111111100000000) >> 8;
                pullupsPortB = gpioExpansionConfig->pullupMap[i] & 0b0000000011111111;
            }
            mcp.PortMode(kiwi_synth::MCPPort::A, 0xFF, pullupsPortA); // All pins are inputs
            mcp.PortMode(kiwi_synth::MCPPort::B, 0xFF, pullupsPortB); // All pins are inputs

            mcp.interruptMode(kiwi_synth::MCP23017InterruptMode::Or);
            mcp.interrupt(kiwi_synth::MCPPort::A, kiwi_synth::CHANGE);
            mcp.interrupt(kiwi_synth::MCPPort::B, kiwi_synth::CHANGE);

            mcps.push_back(mcp);
        }

        if (gpioExpansionConfig->useTimer) {
            InitTimer(gpioExpansionConfig->refreshRate);
        }
        interrupt.Init(gpioExpansionConfig->interruptPin, GPIO::Mode::INTERRUPT_RISING, GPIO::Pull::NOPULL, GPIO::Speed::LOW, IntCallback);

    }

    void GpioExpansion::RegisterControlListener(ControlListener* controlListener, int controlId)
    {
        controlListeners.push_back(controlListener);
        controlIds.push_back(controlId);
    }

    /*
     * (timer speed (Hz)) = (Arduino clock speed (16MHz)) / prescaler
     * interrupt frequency (Hz) = (Arduino clock speed 480,000,000Hz) / (prescaler * (compare match register + 1))
     * compare match register = [ 480,000,000/ (prescaler * desired interrupt frequency) ] - 1
     * 
     * The clock runs at 480MHz when boosted. TM_5 is a 32-bit timer with a max value of 4,294,967,295. For some
     * reason the prescalar works against 240MHz instead of 480MHz.
     * 
     * A prescalar of 2,400 means:
     * timer speed = 240,000,000 / 2,400 = 100,000 = 100KHz timer speed.
     * 
     * For a 1ms interrupt frequency
     * Compare match register = (240,000,000Hz / (2,400 * 1000Hz)) = 100
     * 
     * This means that with a prescalar of 2,400, the refreshRate will be measured in hundreths of a millisecond.
     * 
     * 
     * 10000 = @.25 seconds
     */
    void GpioExpansion::InitTimer(int refreshRate)
    {
        TimerHandle::Config config;
        config.dir = TimerHandle::Config::CounterDir::UP;
        config.enable_irq = true;
        config.period = refreshRate;
        config.periph = TimerHandle::Config::Peripheral::TIM_4;

        timer.Init(config);
        timer.SetPrescaler(2400);
        timer.SetCallback((daisy::TimerHandle::PeriodElapsedCallback)&ProcessTimer, (void *)(this));
    }

    void GpioExpansion::StartTimer()
    {
        timer.Start();
    }

    void GpioExpansion::Process()
    {
        // Read all mcps and write to pinValues
        for (int i = 0; i < numGpioExpansions; i++) {
            pinValues[i] = mcps.at(i).Read();
        }

        for (size_t i = 0; i < controlListeners.size(); i++) {
            controlListeners.at(i)->controlUpdate(0, controlIds.at(i));
        }
    }

} // namespace kiwi_synth
