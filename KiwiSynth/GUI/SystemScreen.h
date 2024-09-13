#ifndef __KIWI_SYNTH_SYSTEM_SCREEN_H__
#define __KIWI_SYNTH_SYSTEM_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Controls/KiwiMcp23017.h"
#include "../../KUtils.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
    class SystemScreen
    {
        public:
            SystemScreen(){}
            virtual ~SystemScreen(){};
            void Init(KiwiDisplay* display, Performance* performance);

            void Display();

        private:
            char buffer[256];
            KiwiDisplay* display;
            Performance* performance;
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_SYSTEM_SCREEN_H__
