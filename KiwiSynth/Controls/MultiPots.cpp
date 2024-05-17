#include <algorithm>
#include "MultiPots.h"

namespace kiwi_synth
{

    void ProcessTimer(void* multiPots)
    {
        ((MultiPots*)multiPots)->Process();
    }

    MultiPots::MultiPots(DaisySeed *hw, MultiPotsConfig *multiPotsConfig) : hw(hw)
    {
        numMps = multiPotsConfig->numMps;
        numChannels = multiPotsConfig->numChannels;
        numDirectPots = multiPotsConfig->numDirectPots;
        pinsSignal = new Pin[numMps];
        pinsDirect = new Pin[numDirectPots];
        InitMulti(multiPotsConfig);

        mpValueBuffer = new float*[numMps];
        for (int i = 0; i < numMps; i++) {
            mpValueBuffer[i] = new float[numChannels];
        }
        currentPot = 0;

        directValueBuffer = new float[numDirectPots];

        if (multiPotsConfig->useTimer) {
            InitTimer(multiPotsConfig->refreshRate);
        }
    }

    MultiPots::~MultiPots()
    {
        if (pinsSignal) {
            free(pinsSignal);
        }

        for (int i = 0; i < numMps; i++) {
            if (mpValueBuffer[i]) {
                free(mpValueBuffer[i]);
            }
        }
        if (mpValueBuffer) {
            free(mpValueBuffer);
        }

        if (directValueBuffer) {
            free(directValueBuffer);
        }
    }

    void MultiPots::StartTimer()
    {
        timer.Start();
    }

    void MultiPots::Process()
    {
        // We read before selecting because reading will not use the new pin if we only just selected it. We need to cycle in between.
        ReadPots();
        currentPot = (currentPot + 1) % std::max(numChannels, numDirectPots);
        // We are setting the pin for the *next* call of Process.
        SelectMpChannel(currentPot);
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
        if (numChannels > 8) {
        a3.Init(multiPotsConfig->pinA3, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
        }

        AdcChannelConfig *adcConfig = new AdcChannelConfig[numMps];
        for (int i = 0; i < numMps; i++)
        {
            pinsSignal[i] = multiPotsConfig->pinsSignal[i];
            //Configure the signal pins as ADC inputs. This is where we'll read the knob values from each multiplexer.
            adcConfig[i].InitSingle(pinsSignal[i]);
        }

        for (int i = 0; i < numDirectPots; i++)
        {
            pinsDirect[i] = multiPotsConfig->pinsDirect[i];
            //Configure the signal pins as ADC inputs. This is where we'll read the knob values from each direct ADC pot.
            adcConfig[i + numMps].InitSingle(pinsDirect[i]);
        }

        hw->adc.Init(adcConfig, numMps + numDirectPots);
        free(adcConfig);
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
    void MultiPots::InitTimer(int refreshRate)
    {
        TimerHandle::Config config;
        config.dir = TimerHandle::Config::CounterDir::UP;
        config.enable_irq = true;
        //config.period = 50000;
        config.period = refreshRate;
        config.periph = TimerHandle::Config::Peripheral::TIM_5;

        timer.Init(config);
        timer.SetPrescaler(2400);
        timer.SetCallback((daisy::TimerHandle::PeriodElapsedCallback)&ProcessTimer, (void *)(this));
    }

    void MultiPots::SelectMpChannel(int channelNumber)
    {
        currentPot = channelNumber;

        int a3Value = channelNumber / 8;
        int a2Value = channelNumber % 8 / 4;
        int a1Value = channelNumber % 4 / 2;
        int a0Value = channelNumber % 2;

        if (numChannels > 8) {
            a3.Write(a3Value);
        }
        a2.Write(a2Value);
        a1.Write(a1Value);
        a0.Write(a0Value);
    }

    void MultiPots::ReadPots()
    {
        if (currentPot < numChannels)
        {
            for (int i = 0; i < numMps; i++)
            {
                mpValueBuffer[i][currentPot] = hw->adc.GetFloat(i);
            }
        }

        if (currentPot < numDirectPots) {
            directValueBuffer[currentPot] = hw->adc.GetFloat(currentPot + numMps);
        }
    }

} // namespace kiwi_synth
