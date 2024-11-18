#include <algorithm>
#include "MultiPots.h"



using namespace kiwisynth;



void MultiPots::Init(DaisySeed *hw, MultiPotsConfig *multiPotsConfig)
{
    InitMulti(multiPotsConfig);
    currentPot_ = 0;
}



float MultiPots::GetMpValue(int boardNumber, int channelNumber)
{
    return mpValueBuffer_[boardNumber][channelNumber];
}



float MultiPots::GetDirectValue(int potNumber)
{
    return directValueBuffer_[potNumber];
}



void MultiPots::InitMulti(MultiPotsConfig *multiPotsConfig)
{
    a0_.Init(multiPotsConfig->pinA0, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
    a1_.Init(multiPotsConfig->pinA1, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
    a2_.Init(multiPotsConfig->pinA2, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
    if (NUM_CHANNELS > 8) {
    a3_.Init(multiPotsConfig->pinA3, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
    }

    AdcChannelConfig *adcConfig = new AdcChannelConfig[NUM_MPS + NUM_DIRECT_POTS];
    for (int i = 0; i < NUM_MPS; i++)
    {
        pinsSignal_.push_back(multiPotsConfig->pinsSignal[i]);
        //Configure the signal pins as ADC inputs. This is where we'll read the knob values from each multiplexer.
        adcConfig[i].InitSingle(pinsSignal_[i]);
    }

    for (int i = 0; i < NUM_DIRECT_POTS; i++)
    {
        pinsDirect_.push_back(multiPotsConfig->pinsDirect[i]);
        //Configure the signal pins as ADC inputs. This is where we'll read the knob values from each direct ADC pot.
        adcConfig[i + NUM_MPS].InitSingle(pinsDirect_[i]);
    }

    hw_->adc.Init(adcConfig, NUM_MPS + NUM_DIRECT_POTS);
    free(adcConfig);
}



void MultiPots::SelectMpChannel(int channelNumber)
{
    currentPot_ = channelNumber;

    int a3Value = channelNumber >> 3; // same as / 8
    int a2Value = (channelNumber & 7) >> 2; // same as % 8 / 4
    int a1Value = (channelNumber & 3) >> 1; // same as % 4 / 2
    int a0Value = channelNumber & 1; // same as / 2

    // Remove a3 if num_channels <= 8
    a3_.Write(a3Value); // channelNumber & 0b1000
    a2_.Write(a2Value); // channelNumber & 0b0100
    a1_.Write(a1Value); // channelNumber & 0b0010
    a0_.Write(a0Value); // channelNumber & 0b0001
}



void MultiPots::ReadPots()
{
    mpValueBuffer_[0][currentPot_] = hw_->adc.GetFloat(0);
    mpValueBuffer_[1][currentPot_] = hw_->adc.GetFloat(1);
    mpValueBuffer_[2][currentPot_] = hw_->adc.GetFloat(2);

    if (currentPot_ < NUM_DIRECT_POTS) {
        directValueBuffer_[currentPot_] = hw_->adc.GetFloat(currentPot_ + NUM_MPS);
    }

    // Check for NULL if there ever might not be a controlListener
    controlListener_->controlMpUpdate(currentPot_);
}
