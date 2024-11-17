#ifndef __KIWI_SYNTH_MULTIPOTS_H__
#define __KIWI_SYNTH_MULTIPOTS_H__

#include <vector>

#include "daisy_seed.h"
#include "IControl.h"
#include "IControlListener.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwisynth
{
    const static int NUM_MPS = 3;
    const static int NUM_CHANNELS = 16;
    const static int CHANNELS_MOD = 15;
    const static int NUM_DIRECT_POTS = 2;

    /*
     * Internal function of type daisy::TimerHandle::PeriodElapsedCallback used for timer callbacks.
     */
    //void ProcessMultiPotsTimer(void* data);

   /*
    * Used to initialize a MultiPots object.
    *
    * pinA0: The pin which is connected to A0 on the multiplexer.
    * pinA1: The pin which is connected to A0 on the multiplexer.
    * pinA2: The pin which is connected to A0 on the multiplexer.
    * pinA3: The pin which is connected to A0 on the multiplexer. This will be ignored for 8 channel multiplexers.
    * pinsSignal: An array of Pins to which the signal output on each multiplexer is connected. The size of this array should
    *     therefore match the number of attached multiplexers.
    * pinsDirect: An array of Pins to which potentiometers are directly connected to ADC inputs.
    * useTimer: If true, a timer interrupt will be used to automatically refresh the potentiometers. If set to false, Process()
    *     will need to be called manually to continue updating the potentiometers.
    * refreshRate: If useTimer = true, this will be how often the data is refreshed measured in hundredths of a millisecond.
    */
    struct MultiPotsConfig
    {
        Pin pinA0;
        Pin pinA1;
        Pin pinA2;
        Pin pinA3;
        std::vector<Pin> pinsSignal;
        std::vector<Pin> pinsDirect;
        bool useTimer;
        int refreshRate;
    };

    /*
    * Manages a collection of potentiometers. Both multiplexed potentiometers as well as directly connected potentiometers
    * are supported. The primary concern with this implementation is maintaining low processing overhead, so the number of
    * simultaneous reads during a given cycle is kept minimal. It is assumed that all multiplexers are attached sharing the
    * a0, a1, a2, and a3 (if applicable) pins and each multiplexer has its own corresponding signal pin.
    * 
    * An internal timer configured for TIM_5 can be selected by passing in useTimer = true; If so, the pot values will polled
    * and updated when the timer triggers. If useTimer=false, then you will have to call Process() regularly to step through
    * polling each channel of the multiplexers and directly connected potentiometer.
    * 
    * When the timer triggers or Process() is called, the current channel selected on each multiplexer will be polled for
    * a value and those values will be stored in a buffer. After this selected channel will increase by one. One of the
    * directly connected pots will also be polled. As a result it will take max(numChannels, numDirectPots) iterations of
    * Process() for all values to be read from all potentiometers and put into the buffer. When GetMpValue or GetDirectValue
    * are called, the value is read from the buffer, so the value therein could be as many as max(numChannels, numDirectPots)
    * iterations old. The refresh rate should be set with this in mind.
    */
    class MultiPots : public IControl
    {
        private:
            int currentPot = 0;
            GPIO a0;
            GPIO a1;
            GPIO a2;
            GPIO a3;
            std::vector<Pin> pinsSignal;
            std::vector<Pin> pinsDirect;
            DaisySeed *hw;
            //TimerHandle timer;
            IControlListener* controlListener = nullptr;

            void InitMulti(MultiPotsConfig *multiPotsConfig);
            //void InitTimer(int refreshRate);
            void SelectMpChannel(int channelNumber);
            void ReadPots();

        public:
            MultiPots() {};
            ~MultiPots() {};
            void Init(DaisySeed *hw, MultiPotsConfig *multiPotsConfig);

            /*
             * When the multipots are updated with new values, the registered control listener will be called using the IControlListener
             * interface callback function.
             */
            inline void RegisterControlListener(IControlListener* controlListener) { this->controlListener = controlListener; }

            /*
            * Loads a single channel of pot values for all attached multiplexers and/or a single directly connected potentiometer
            * into the buffer. This function only needs to be called if the timer is not being used.
            */
            inline void Process()
            {
                // We read before selecting because reading will not use the new pin if we only just selected it. We need to cycle in between.
                ReadPots();
                currentPot = (currentPot + 1) & CHANNELS_MOD; // Same as % NUM_CHANNELS
                // We are setting the pin for the *next* call of Process.
                SelectMpChannel(currentPot);
            }
            /*
            * Returns a stored multiplexed pot value from the buffer.
            */
            float GetMpValue(int boardNumber, int channelNumber);
            /*
            * Returns a stored direct ADC pot value from the buffer.
            */
            float GetDirectValue(int potNumber);
    }; // class MultiPots

} // namespace kiwisynth


#endif // __KIWI_SYNTH_MULTIPOTS_H__
