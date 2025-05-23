#pragma once



#include "per/gpio.h"
#include "per/i2c.h"



using namespace daisy;

namespace kiwisynth
{



// Adapted from https://github.com/blemasle/arduino-mcp23017
/**
 * Registers addresses.
 * The library use addresses for IOCON.BANK = 0.
 * See "3.2.1 Byte mode and Sequential mode".
 */
enum class MCPRegister : uint8_t
{
    IODIR_A = 0x00, // Controls the direction of the data I/O for port A.
    IODIR_B = 0x01, // Controls the direction of the data I/O for port B.
    IPOL_A
    = 0x02, // Configures the polarity on the corresponding GPIO_ port bits for port A.
    IPOL_B
    = 0x03, // Configures the polarity on the corresponding GPIO_ port bits for port B.
    GPINTEN_A
    = 0x04, // Controls the interrupt-on-change for each pin of port A.
    GPINTEN_B
    = 0x05, // Controls the interrupt-on-change for each pin of port B.
    DEFVAL_A
    = 0x06, // Controls the default comparaison value for interrupt-on-change for port A.
    DEFVAL_B
    = 0x07, // Controls the default comparaison value for interrupt-on-change for port B.
    INTCON_A
    = 0x08, // Controls how the associated pin value is compared for the interrupt-on-change for port A.
    INTCON_B
    = 0x09, // Controls how the associated pin value is compared for the interrupt-on-change for port B.
    IOCON  = 0x0A, // Controls the device.
    GPPU_A = 0x0C, // Controls the pull-up resistors for the port A pins.
    GPPU_B = 0x0D, // Controls the pull-up resistors for the port B pins.
    INTF_A = 0x0E, // Reflects the interrupt condition on the port A pins.
    INTF_B = 0x0F, // Reflects the interrupt condition on the port B pins.
    INTCAP_A
    = 0x10, // Captures the port A value at the time the interrupt occured.
    INTCAP_B
    = 0x11, // Captures the port B value at the time the interrupt occured.
    GPIO_A = 0x12, // Reflects the value on the port A.
    GPIO_B = 0x13, // Reflects the value on the port B.
    OLAT_A = 0x14, // Provides access to the port A output latches.
    OLAT_B = 0x15, // Provides access to the port B output latches.
};



enum class MCPPort : uint8_t
{
    A = 0,
    B = 1
};



inline MCPRegister operator+(MCPRegister a, MCPPort b)
{
    return static_cast<MCPRegister>(static_cast<uint8_t>(a)
                                    + static_cast<uint8_t>(b));
}



enum class MCPMode : uint8_t
{
    INPUT,
    INPUT_PULLUP,
    OUTPUT,
};



/**
 * Controls if the two interrupt pins mirror each other.
 * See "3.6 Interrupt Logic".
 * 
 * Based on DaisyLib mcp23x17.h and interrupt code from:
 * https://github.com/blemasle/arduino-mcp23017/blob/master/src/MCP23017.cpp
 * 
 */
enum class MCP23017InterruptMode : uint8_t
{
    Separated = 0,	///< Interrupt pins are kept independent
    Or = 0b01000000	///< Interrupt pins are mirrored
};



enum MCP23017InterruptTrigger : uint8_t
{
    CHANGE,
    FALLING,
    RISING
};



/**
 * Barebones driver for MCP23017 I2C 16-Bit I/O Expander
 * For now it supports only polling approach.
 * 
 * Usage:
 *  Mcp23017 mcp;
 *  mcp.Init();
 *  mcp.PortMode(MCP23017Port::A, 0xFF); // Inputs
 *  mcp.PortMode(MCP23017Port::B, 0xFF);
 *  mcp.Read();
 *  mcp.GetPin(2);
 */
class KiwiMcp23017Transport
{
public:
    daisy::I2CHandle i2c_;
    uint8_t          i2c_address_;
    uint8_t          timeout_{10};



    struct Config
    {
        daisy::I2CHandle::Config i2c_config;
        uint8_t           i2c_address;
        void              Defaults()
        {
            i2c_config.periph         = daisy::I2CHandle::Config::Peripheral::I2C_1;
            i2c_config.speed          = daisy::I2CHandle::Config::Speed::I2C_400KHZ;
            i2c_config.mode           = daisy::I2CHandle::Config::Mode::I2C_MASTER;
            i2c_config.pin_config.scl = {DSY_GPIOB, 8}; // Pin 12 I2C1_SCL
            i2c_config.pin_config.sda = {DSY_GPIOB, 9}; // Pin 13 I2C1_SDA
            i2c_address               = 0x20;
            i2c_config.address        = 0x20;
        }
    };



    void Init()
    {
        Config config;
        config.Defaults();
        Init(config);
    };



    void Init(const Config& config)
    {
        i2c_address_ = config.i2c_address << 1;
        i2c_.Init(config.i2c_config);
    };



    daisy::I2CHandle::Result WriteReg(MCPRegister reg, uint8_t val)
    {
        uint8_t data[1] = {val};
        return i2c_.WriteDataAtAddress(
            i2c_address_, static_cast<uint8_t>(reg), 1, data, 1, timeout_);
    }



    daisy::I2CHandle::Result WriteReg(MCPRegister reg, uint8_t portA, uint8_t portB)
    {
        uint8_t data[2] = {portA, portB};
        return i2c_.WriteDataAtAddress(
            i2c_address_, static_cast<uint8_t>(reg), 1, data, 2, timeout_);
    }



    uint8_t ReadReg(MCPRegister reg)
    {
        uint8_t data[1] = {0x00};
        i2c_.ReadDataAtAddress(
            i2c_address_, static_cast<uint8_t>(reg), 1, data, 1, timeout_);
        return data[0];
    }



    void ReadReg(MCPRegister reg, uint8_t& portA, uint8_t& portB)
    {
        uint8_t data[2];
        i2c_.ReadDataAtAddress(
            i2c_address_, static_cast<uint8_t>(reg), 1, data, 2, timeout_);

        portA = data[0];
        portB = data[1];
    }
};



template <typename Transport>
class KiwiMcp23X17
{
    public:
        uint8_t i2c_address;



        struct Config
        {
            typename Transport::Config transport_config;
        };



        void Init()
        {
            Config config;
            config.transport_config.Defaults();
            Init(config);
        };



        void Init(const Config& config)
        {
            i2c_address = config.transport_config.i2c_config.address;
            transport_.Init(config.transport_config);

            //BANK = 	0 : sequential register addresses
            //MIRROR = 	0 : use configureInterrupt
            //SEQOP = 	1 : sequential operation disabled, address pointer does not increment
            //DISSLW = 	0 : slew rate enabled
            //HAEN = 	0 : hardware address pin is always enabled on 23017
            //ODR = 	0 : open drain output
            //INTPOL = 	0 : interrupt active low
            transport_.WriteReg(MCPRegister::IOCON, 0b01100010);
            //transport.WriteReg(MCPRegister::IOCON, 0b00100000);

            //enable all pull up resistors (will be effective for input pins only)
            transport_.WriteReg(MCPRegister::GPPU_A, 0xFF, 0xFF);
        };



        /**
         * Controls the pins direction on a whole port at once.
         * 
         * directions: 0 - output, 1 - input
         * pullups: 0 - disabled, 1 - enabled
         * inverted: 0 - false/normal, 1 - true/inverted
         * 
         * See "3.5.1 I/O Direction register".
         */
        void PortMode(MCPPort port,
                    uint8_t directions,
                    uint8_t pullups  = 0xFF,
                    uint8_t inverted = 0x00)
        {
            transport_.WriteReg(MCPRegister::IODIR_A + port, directions);
            transport_.WriteReg(MCPRegister::GPPU_A + port, pullups);
            transport_.WriteReg(MCPRegister::IPOL_A + port, inverted);
        }



        /**
         * Controls a single pin direction. 
         * Pin 0-7 for port A, 8-15 fo port B.
         * 
         * 1 = Pin is configured as an input.
         * 0 = Pin is configured as an output.
         *
         * See "3.5.1 I/O Direction register".
         */
        void PinMode(uint8_t pin, MCPMode mode, bool inverted)
        {
            MCPRegister iodirreg  = MCPRegister::IODIR_A;
            MCPRegister pullupreg = MCPRegister::GPPU_A;
            MCPRegister polreg    = MCPRegister::IPOL_A;
            uint8_t     iodir, pol, pull;
            if(pin > 7)
            {
                iodirreg  = MCPRegister::IODIR_B;
                pullupreg = MCPRegister::GPPU_B;
                polreg    = MCPRegister::IPOL_B;
                pin -= 8;
            }
            iodir = transport_.ReadReg(iodirreg);
            if(mode == MCPMode::INPUT || mode == MCPMode::INPUT_PULLUP)
                SetBit(iodir, pin);
            else
                ClearBit(iodir, pin);
            pull = transport_.ReadReg(pullupreg);
            if(mode == MCPMode::INPUT_PULLUP)
                SetBit(pull, pin);
            else
                ClearBit(pull, pin);
            pol = transport_.ReadReg(polreg);
            if(inverted)
                SetBit(pol, pin);
            else
                ClearBit(pol, pin);
            transport_.WriteReg(iodirreg, iodir);
            transport_.WriteReg(pullupreg, pull);
            transport_.WriteReg(polreg, pol);
        }



        /**
         * Writes a single pin state.
         * Pin 0-7 for port A, 8-15 for port B.
         * 
         * 1 = Logic-high
         * 0 = Logic-low
         * 
         * See "3.5.10 Port register".
         */
        void WritePin(uint8_t pin, uint8_t state)
        {
            MCPRegister gpioreg = MCPRegister::GPIO_A;
            uint8_t     gpio;
            if(pin > 7)
            {
                gpioreg = MCPRegister::GPIO_B;
                pin -= 8;
            }

            gpio = transport_.ReadReg(gpioreg);
            if(state > 0)
            {
                gpio = SetBit(gpio, pin);
            }
            else
            {
                gpio = ClearBit(gpio, pin);
            }

            transport_.WriteReg(gpioreg, gpio);
        }



        /**
         * Reads a single pin state.
         * Pin 0-7 for port A, 8-15 for port B.
         * 
         * 1 = Logic-high
         * 0 = Logic-low
         * 
         * See "3.5.10 Port register".
         */
        uint8_t ReadPin(uint8_t pin)
        {
            MCPRegister gpioreg = MCPRegister::GPIO_A;
            uint8_t     gpio;
            if(pin > 7)
            {
                gpioreg = MCPRegister::GPIO_B;
                pin -= 8;
            }

            gpio = transport_.ReadReg(gpioreg);
            if(ReadBit(gpio, pin))
            {
                return 1;
            }

            return 0;
        }



        /**
         * Writes pins state to a whole port.
         * 
         * 1 = Logic-high
         * 0 = Logic-low
         * 
         * See "3.5.10 Port register".
         */
        void WritePort(MCPPort port, uint8_t value)
        {
            transport_.WriteReg(MCPRegister::GPIO_A + port, value);
        }



        /**
         * Reads pins state for a whole port.
         * 
         * 1 = Logic-high
         * 0 = Logic-low
         * 
         * See "3.5.10 Port register".
         */
        uint8_t ReadPort(MCPPort port)
        {
            return transport_.ReadReg(MCPRegister::GPIO_A + port);
        }



        /**
         * Writes pins state to both ports.
         * 
         * 1 = Logic-high
         * 0 = Logic-low
         * 
         * See "3.5.10 Port register".
         */
        void Write(uint16_t value)
        {
            transport_.WriteReg(
                MCPRegister::GPIO_A, LowByte(value), HighByte(value));
        }



        /**
         * Reads pins state for both ports. 
         * 
         * 1 = Logic-high
         * 0 = Logic-low
         * 
         * See "3.5.10 Port register".
         */
        uint16_t Read()
        {
            uint8_t a = ReadPort(MCPPort::A);
            uint8_t b = ReadPort(MCPPort::B);

            pin_data_ = a | b << 8;
            return pin_data_;
        }



        /**
         * @brief Fetches pin state from the result of recent Read() call. Useful to preserve unneeded reads
         * 
         * @param id pin ID
         * @return uint8_t pin state: 0x00 or 0xFF
         */
        uint8_t GetPin(uint8_t id) { return ReadBit(pin_data_, id); }



        void interruptMode(MCP23017InterruptMode intMode)
        {
            uint8_t iocon = transport_.ReadReg(MCPRegister::IOCON);
            if(intMode == MCP23017InterruptMode::Or) iocon |= static_cast<uint8_t>(MCP23017InterruptMode::Or);
            else iocon &= ~(static_cast<uint8_t>(MCP23017InterruptMode::Or));

            transport_.WriteReg(MCPRegister::IOCON, iocon);
        }



        void interrupt(MCPPort port, uint8_t mode, uint8_t pins = 0xFF)
        {
            MCPRegister defvalreg = MCPRegister::DEFVAL_A + port;
            MCPRegister intconreg = MCPRegister::INTCON_A + port;

            //enable interrupt for port
            transport_.WriteReg(MCPRegister::GPINTEN_A + port, pins);
            switch(mode)
            {
            case CHANGE:
                //interrupt on change
                transport_.WriteReg(intconreg, 0);
                break;
            case FALLING:
                //interrupt falling : compared against defval, 0xff
                transport_.WriteReg(intconreg, 0xFF);
                transport_.WriteReg(defvalreg, 0xFF);
                break;
            case RISING:
                //interrupt rising : compared against defval, 0x00
                transport_.WriteReg(intconreg, 0xFF);
                transport_.WriteReg(defvalreg, 0x00);
                break;
            }
        }



        void interruptedBy(uint8_t& portA, uint8_t& portB)
        {
            transport_.ReadReg(MCPRegister::INTF_A, portA, portB);
        }



        void disableInterrupt(MCPPort port)
        {
            transport_.WriteReg(MCPRegister::GPINTEN_A + port, 0x00);
        }



        void clearInterrupts()
        {
            uint8_t a, b;
            clearInterrupts(a, b);
        }



        void clearInterrupts(uint8_t& portA, uint8_t& portB)
        {
            transport_.ReadReg(MCPRegister::INTCAP_A, portA, portB);
        }



    private:
        uint16_t  pin_data_;
        Transport transport_;



        uint8_t GetBit(uint8_t data, uint8_t id)
        {
            uint8_t mask     = 1 << id;
            uint8_t masked_n = data & mask;
            return masked_n >> id;
        }



        uint8_t SetBit(uint8_t data, uint8_t pos)
        {
            data = (data | (1 << pos));
            return data;
        }



        uint8_t ClearBit(uint8_t data, uint8_t pos)
        {
            data = (data & (~(1 << pos)));
            return data;
        }



        uint8_t ReadBit(uint16_t data, uint8_t pos)
        {
            return data & (1 << pos) ? 0xff : 0x00;
        }



        uint8_t LowByte(uint16_t val) { return val & 0xFF; }



        uint8_t HighByte(uint16_t val) { return (val >> 8) & 0xff; }
};



using KiwiMcp23017 = KiwiMcp23X17<KiwiMcp23017Transport>;



} // namespace kiwisynth