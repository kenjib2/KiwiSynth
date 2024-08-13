#include <algorithm>
#include "MultiPots.h"

namespace kiwi_synth
{
    float DSY_SDRAM_BSS mpValueBuffer[NUM_MPS][NUM_CHANNELS];
    float DSY_SDRAM_BSS directValueBuffer[NUM_DIRECT_POTS];

    /*void ProcessMultiPotsTimer(void* multiPots)
    {
        ((MultiPots*)multiPots)->Process();
    }*/

    void MultiPots::Init(DaisySeed *hw, MultiPotsConfig *multiPotsConfig)
    {
        InitMulti(multiPotsConfig);
        currentPot = 0;

        /*if (multiPotsConfig->useTimer) {
            InitTimer(multiPotsConfig->refreshRate);
        }*/
    }

    void MultiPots::RegisterControlListener(ControlListener* controlListener, int controlId)
    {
        this->controlListener = controlListener;
        this->controlId = controlId;
    }

    /*void MultiPots::StartTimer()
    {
        timer.Start();
    }*/

    void MultiPots::Process()
    {
        // We read before selecting because reading will not use the new pin if we only just selected it. We need to cycle in between.
        ReadPots();
        currentPot = (currentPot + 1) % std::max(NUM_CHANNELS, NUM_DIRECT_POTS);
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
        if (NUM_CHANNELS > 8) {
        a3.Init(multiPotsConfig->pinA3, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
        }

        AdcChannelConfig *adcConfig = new AdcChannelConfig[NUM_MPS];
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
    /*void MultiPots::InitTimer(int refreshRate)
    {
        TimerHandle::Config config;
        config.dir = TimerHandle::Config::CounterDir::UP;
        config.enable_irq = true;
        config.period = refreshRate;
        config.periph = TimerHandle::Config::Peripheral::TIM_5;

        timer.Init(config);
        timer.SetPrescaler(2400);
        timer.SetCallback((daisy::TimerHandle::PeriodElapsedCallback)&ProcessMultiPotsTimer, (void *)(this));
    }*/

    void MultiPots::SelectMpChannel(int channelNumber)
    {
        currentPot = channelNumber;

        int a3Value = channelNumber / 8;
        int a2Value = channelNumber % 8 / 4;
        int a1Value = channelNumber % 4 / 2;
        int a0Value = channelNumber % 2;

        if (NUM_CHANNELS > 8) {
            a3.Write(a3Value); // channelNumber & 0b1000
        }
        a2.Write(a2Value); // channelNumber & 0b0100
        a1.Write(a1Value); // channelNumber & 0b0010
        a0.Write(a0Value); // channelNumber & 0b0001
    }

    void MultiPots::ReadPots()
    {
        if (currentPot < NUM_CHANNELS)
        {
            for (int i = 0; i < NUM_MPS; i++)
            {
                mpValueBuffer[i][currentPot] = hw->adc.GetFloat(i);
            }
        }

        if (currentPot < NUM_DIRECT_POTS) {
            directValueBuffer[currentPot] = hw->adc.GetFloat(currentPot + NUM_MPS);
        }

        if (controlListener) {
            controlListener->controlUpdate(currentPot, controlId);
        }
    }

} // namespace kiwi_synth
