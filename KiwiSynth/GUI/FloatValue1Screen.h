#ifndef __KIWI_SYNTH_FLOAT_VALUE_1_SCREEN_H__
#define __KIWI_SYNTH_FLOAT_VALUE_1_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/Patch.h"
#include "EnumToText.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
    /*
     * The float value 1 screen displays all of the bool based data and some float based data for the
     * patch while also listening for any changes to the values if in live mode. This allows you to see
     * what is being changed.
     * 
     * Clicking will switch between voice 1 and 2. All updates are made using the standard physical patch
     * controls.
     */
    class FloatValue1Screen
    {
        public:
            FloatValue1Screen(){}
            virtual ~FloatValue1Screen(){};
            void Init(KiwiDisplay* display, Patch* patch);

            void Display();
            void Click();

        private:
            char buffer[256];
            KiwiDisplay* display;
            Patch* patch;
            uint8_t voiceNumber;
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_FLOAT_VALUE_1_SCREEN_H__
