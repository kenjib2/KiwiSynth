#ifndef __KIWI_SYNTH_SETTINGS_SCREEN_3_H__
#define __KIWI_SYNTH_SETTINGS_SCREEN_3_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/Patch.h"
#include "EnumToText.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwisynth
{

    /*
     * Settings screen 3 displays all of the data for the amplifier and general settings while also listening for
     * any changes to the values if in live mode. This allows you to see what is being changed.
     * 
     * Clicking alternates between settings 1 and 2 when in split or layered mode.
     */
    class SettingsScreen3
    {
        public:
            SettingsScreen3(){}
            virtual ~SettingsScreen3(){};
            void Init(KiwiDisplay* display, Patch* patch);

            void Display();
            void Click();

        private:
            char buffer[32];
            KiwiDisplay* display;
            Patch* patch;
            int voiceNumber;

    };
} // namespace kiwisynth


#endif // __KIWI_SYNTH_SETTINGS_SCREEN_3_H__
