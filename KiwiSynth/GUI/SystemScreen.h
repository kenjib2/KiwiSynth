#ifndef __KIWI_SYNTH_SYSTEM_SCREEN_H__
#define __KIWI_SYNTH_SYSTEM_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Controls/KiwiMcp23017.h"
#include "../Util/KUtils.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwisynth
{
    // These are all possible selected menu items on this screen.
    enum SystemScreenSelection
    {
        SYSTEM_SCREEN_NONE = -1,
        SYSTEM_SCREEN_PANIC,
        SYSTEM_SCREEN_SYSEX_SEND,
        SYSTEM_SCREEN_SYSEX_READ,
        SYSTEM_SCREEN_UPDATE,
        SYSTEM_SCREEN_RETURN
    };
    const static int SYSTEM_SCREEN_OPTIONS = 5;

    enum SystemScreenResponse
    {
        SYSTEM_SCREEN_RESPONSE_EDIT,
        SYSTEM_SCREEN_RESPONSE_NOEDIT,
        SYSTEM_SCREEN_RESPONSE_PANIC,
        SYSTEM_SCREEN_RESPONSE_SYSEX_SEND,
        SYSTEM_SCREEN_RESPONSE_SYSEX_READ,
        SYSTEM_SCREEN_RESPONSE_UPDATE
    };

    /*
     * Displays performance info and allows for BIOS update.
     */
    class SystemScreen
    {
        public:
            SystemScreenSelection selected;

            SystemScreen(){}
            virtual ~SystemScreen(){};
            void Init(KiwiDisplay* display, Performance* performance);

            /*
             * Displays performance info and allows for BIOS update.
             */
            void Display();
            void DisplaySending();
            void DisplayReceiving();
            void Increment();
            void Decrement();
            SystemScreenResponse Click();

        private:
            char buffer[32];
            KiwiDisplay* display;
            Performance* performance;
    };
} // namespace kiwisynth


#endif // __KIWI_SYNTH_SYSTEM_SCREEN_H__
