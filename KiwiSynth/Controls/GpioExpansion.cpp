#include "GpioExpansion.h"

namespace kiwi_synth
{
    void GpioExpansion::Init()
    {
        GpioExpansionConfig cfg;
        cfg.Defaults();
        Init(&cfg);
    }

    void GpioExpansion::Init(GpioExpansionConfig *gpioExpansionConfig)
    {
        numGpioExpansions = gpioExpansionConfig->numGpioExpansions;

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

            mcps.push_back(mcp);
        }
    }

    void GpioExpansion::Process()
    {
        // Read all mcps and write to pinValues
        pinValues[mcps.at(0).i2c_address] = mcps.at(0).Read();
        mcps.at(0).clearInterrupts();
        controlListener->controlGpioUpdate(mcps.at(0).i2c_address); // Check for NULL if we ever might not be using a controlListener

        pinValues[mcps.at(1).i2c_address] = mcps.at(1).Read();
        mcps.at(1).clearInterrupts();
        controlListener->controlGpioUpdate(mcps.at(1).i2c_address); // Check for NULL if we ever might not be using a controlListener

        pinValues[mcps.at(2).i2c_address] = mcps.at(2).Read();
        mcps.at(2).clearInterrupts();
        controlListener->controlGpioUpdate(mcps.at(2).i2c_address); // Check for NULL if we ever might not be using a controlListener

        pinValues[mcps.at(3).i2c_address] = mcps.at(3).Read();
        mcps.at(3).clearInterrupts();
        controlListener->controlGpioUpdate(mcps.at(3).i2c_address); // Check for NULL if we ever might not be using a controlListener
    }

} // namespace kiwi_synth
