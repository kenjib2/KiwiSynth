#include <algorithm>
#include "MultiPots.h"

namespace kiwi_synth
{
    float DSY_SDRAM_BSS mpValueBuffer[NUM_MPS][NUM_CHANNELS];
    float DSY_SDRAM_BSS directValueBuffer[NUM_DIRECT_POTS];

    void MultiPots::Init(DaisySeed *hw, MultiPotsConfig *multiPotsConfig)
    {
        InitMulti(multiPotsConfig);
        currentPot = 0;
    }

    float MultiPots::GetMpValue(int boardNumber, int channelNumber)
    {
        return mpValueBuffer[boardNumber][channelNumber];
    }

    float MultiPots::GetDirectValue(int potNumber)
    {
        return directValueBuffer[potNumber];
    }

    void MultiPots::InitMulti(MultiPotsConfig *multiPotsConfig)
    {
        a0.Init(multiPotsConfig->pinA0, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
        a1.Init(multiPotsConfig->pinA1, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
        a2.Init(multiPotsConfig->pinA2, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
        if (NUM_CHANNELS > 8) {
        a3.Init(multiPotsConfig->pinA3, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
        }

        AdcChannelConfig *adcConfig = new AdcChannelConfig[NUM_MPS + NUM_DIRECT_POTS];
        for (int i = 0; i < NUM_MPS; i++)
        {
            pinsSignal.push_back(multiPotsConfig->pinsSignal[i]);
            //Configure the signal pins as ADC inputs. This is where we'll read the knob values from each multiplexer.
            adcConfig[i].InitSingle(pinsSignal[i]);
        }

        for (int i = 0; i < NUM_DIRECT_POTS; i++)
        {
            pinsDirect.push_back(multiPotsConfig->pinsDirect[i]);
            //Configure the signal pins as ADC inputs. This is where we'll read the knob values from each direct ADC pot.
            adcConfig[i + NUM_MPS].InitSingle(pinsDirect[i]);
        }

        hw->adc.Init(adcConfig, NUM_MPS + NUM_DIRECT_POTS);
        free(adcConfig);
    }

    void MultiPots::SelectMpChannel(int channelNumber)
    {
        currentPot = channelNumber;

        int a3Value = channelNumber >> 3; // same as / 8
        int a2Value = (channelNumber & 7) >> 2; // same as % 8 / 4
        int a1Value = (channelNumber & 3) >> 1; // same as % 4 / 2
        int a0Value = channelNumber & 1; // same as / 2

        // Remove a3 if num_channels <= 8
        a3.Write(a3Value); // channelNumber & 0b1000
        a2.Write(a2Value); // channelNumber & 0b0100
        a1.Write(a1Value); // channelNumber & 0b0010
        a0.Write(a0Value); // channelNumber & 0b0001
    }

    void MultiPots::ReadPots()
    {
        mpValueBuffer[0][currentPot] = hw->adc.GetFloat(0);
        mpValueBuffer[1][currentPot] = hw->adc.GetFloat(1);
        mpValueBuffer[2][currentPot] = hw->adc.GetFloat(2);

        if (currentPot < NUM_DIRECT_POTS) {
            directValueBuffer[currentPot] = hw->adc.GetFloat(currentPot + NUM_MPS);
        }

        // Check for NULL if there ever might not be a controlListener
        controlListener->controlMpUpdate(currentPot);
    }

} // namespace kiwi_synth
