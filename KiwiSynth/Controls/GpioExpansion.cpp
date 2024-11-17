#include "GpioExpansion.h"

namespace kiwisynth
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

        memset(bitValues, 0, 4 * 16 * sizeof(bool));
        for (int i = 0; i < numGpioExpansions; i++) {
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
            mcp.PortMode(kiwisynth::MCPPort::A, 0xFF, pullupsPortA, 0x00); // All pins are inputs
            mcp.PortMode(kiwisynth::MCPPort::B, 0xFF, pullupsPortB, 0x00); // All pins are inputs

            uint8_t interruptsPortA = 0xFF;
            uint8_t interruptsPortB = 0xFF;
            if (gpioExpansionConfig->activeMap) {
                interruptsPortA = (gpioExpansionConfig->activeMap[i] & 0b1111111100000000) >> 8;
                interruptsPortB = gpioExpansionConfig->activeMap[i] & 0b0000000011111111;
            }
            mcp.interruptMode(kiwisynth::MCP23017InterruptMode::Or);
            mcp.interrupt(kiwisynth::MCPPort::A, kiwisynth::CHANGE, interruptsPortA);
            mcp.interrupt(kiwisynth::MCPPort::B, kiwisynth::CHANGE, interruptsPortB);

            mcps.push_back(mcp);

        	interrupt.Init(seed::D18, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN, GPIO::Speed::LOW);
        }
    }

    bool GpioExpansion::Process(bool forceRead)
    {
        bool interrupted = interrupt.Read();
        if (interrupted || forceRead) {
            uint16_t nextValues;

            // Read all mcps and write to pinValues
            nextValues = mcps.at(0).Read();
            for (int i = 0; i < 16; i++) {
                bitValues[mcps.at(0).i2c_address - 0x20][i] = nextValues & (1 << (i - 1));
            }
            controlListener->controlGpioUpdate(mcps.at(0).i2c_address); // Check for NULL if we ever might not be using a controlListener

            nextValues = mcps.at(1).Read();
            for (int i = 0; i < 16; i++) {
                bitValues[mcps.at(1).i2c_address - 0x20][i] = nextValues & (1 << (i - 1));
            }
            controlListener->controlGpioUpdate(mcps.at(1).i2c_address); // Check for NULL if we ever might not be using a controlListener

            nextValues = mcps.at(2).Read();
            for (int i = 0; i < 16; i++) {
                bitValues[mcps.at(2).i2c_address - 0x20][i] = nextValues & (1 << (i - 1));
            }
            controlListener->controlGpioUpdate(mcps.at(2).i2c_address); // Check for NULL if we ever might not be using a controlListener

            nextValues = mcps.at(3).Read();
            for (int i = 0; i < 16; i++) {
                bitValues[mcps.at(3).i2c_address - 0x20][i] = nextValues & (1 << (i - 1));
            }
            controlListener->controlGpioUpdate(mcps.at(3).i2c_address); // Check for NULL if we ever might not be using a controlListener
        }

        return interrupted;
    }

} // namespace kiwisynth
