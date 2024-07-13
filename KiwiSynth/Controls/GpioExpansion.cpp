#include "GpioExpansion.h"

namespace kiwi_synth
{
	uint16_t pinRead = 0;

    bool gpioReadRequired = false;
    uint32_t gpioLastInterruptTime = 0;

    void GpioExpansionInterruptCallback(Pin pin)
    {
        uint32_t now = System::GetUs();
        if (now - gpioLastInterruptTime > 1000 || gpioLastInterruptTime > now) {
            gpioLastInterruptTime = now;
            gpioReadRequired = true;
        }
    }

    GpioExpansion::~GpioExpansion()
    {
    }

    void GpioExpansion::Init()
    {
        GpioExpansionConfig cfg;
        cfg.Defaults();
        Init(&cfg);
    }

    void GpioExpansion::Init(GpioExpansionConfig *gpioExpansionConfig)
    {
        numGpioExpansions = gpioExpansionConfig->numGpioExpansions;
        gpioReadRequired = true;
        //gpioReadRequired = false;

        for (int i = 0; i < numGpioExpansions; i++) {
            pinValues[0x20 + i] = 0;
            KiwiMcp23017 mcp;
            KiwiMcp23017::Config cfg;
            cfg.transport_config.Defaults();
            cfg.transport_config.i2c_address = 0x20 + i;
            cfg.transport_config.i2c_config.address = 0x20 + i;
            cfg.transport_config.i2c_config.pin_config.sda = gpioExpansionConfig->sdaPin;
            cfg.transport_config.i2c_config.pin_config.scl = gpioExpansionConfig->sclPin;

            mcp.Init(cfg);

            uint8_t pullupsPortA = 0xFF;
            uint8_t pullupsPortB = 0xFF;
            if (gpioExpansionConfig->pullupMap) {
                pullupsPortA = (gpioExpansionConfig->pullupMap[i] & 0b1111111100000000) >> 8;
                pullupsPortB = gpioExpansionConfig->pullupMap[i] & 0b0000000011111111;
            }
            mcp.PortMode(kiwi_synth::MCPPort::A, 0xFF, pullupsPortA, 0x00); // All pins are inputs
            mcp.PortMode(kiwi_synth::MCPPort::B, 0xFF, pullupsPortB, 0x00); // All pins are inputs

            mcp.interruptMode(MCP23017InterruptMode::Or);
            uint8_t activesPortA = 0xFF;
            uint8_t activesPortB = 0xFF;
            if (gpioExpansionConfig->activeMap) {
                activesPortA = (gpioExpansionConfig->activeMap[i] & 0b1111111100000000) >> 8;
                activesPortB = gpioExpansionConfig->activeMap[i] & 0b0000000011111111;
            }
            mcp.interrupt(kiwi_synth::MCPPort::A, CHANGE, activesPortA);
            mcp.interrupt(kiwi_synth::MCPPort::B, CHANGE, activesPortB);

            mcps.push_back(mcp);
        }

        //interrupt.Init(gpioExpansionConfig->interruptPin, GPIO::Mode::INTERRUPT_RISING, GPIO::Pull::NOPULL, GPIO::Speed::LOW, GpioExpansionInterruptCallback);


        /*numGpioExpansions = gpioExpansionConfig->numGpioExpansions;
        pinValues = (uint16_t*)malloc(sizeof(uint16_t) * 16);
        gpioReadRequired = true;
        sprintf(tempBuffer, "Initialized");

        for (int i = 0; i < numGpioExpansions; i++) {
            KiwiMcp23017 mcp;
            KiwiMcp23017::Config cfg;
            cfg.transport_config.Defaults();
            cfg.transport_config.i2c_config.address = 0x20 + i;
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
        interrupt.Init(gpioExpansionConfig->interruptPin, GPIO::Mode::INTERRUPT_RISING, GPIO::Pull::NOPULL, GPIO::Speed::LOW, GpioExpansionInterruptCallback);*/

    }

    void GpioExpansion::RegisterControlListener(ControlListener* controlListener, int controlId)
    {
        this->controlListener = controlListener;
        this->controlId = controlId;
    }

    void GpioExpansion::Process()
    {
        // Read all mcps and write to pinValues
        for (int i = 0; i < numGpioExpansions; i++) {
            pinValues[mcps.at(i).i2c_address] = mcps.at(i).Read();
            mcps.at(i).clearInterrupts();

            if (controlListener) {
                controlListener->controlUpdate(mcps.at(i).i2c_address, controlId);
            }
        }

    }

    void GpioExpansion::ClearInterrupts()
    {
        for (int i = 0; i < numGpioExpansions; i++) {
            mcps.at(i).clearInterrupts();
        }
    }

    uint16_t GpioExpansion::getPinValues(uint8_t address)
    {
        return pinValues[address];
    }

    bool GpioExpansion::getPinValue(uint8_t address, uint8_t pin)
    {
        return (getPinValues(address) & (1 << (pin - 1))) > 0;
    }

} // namespace kiwi_synth
