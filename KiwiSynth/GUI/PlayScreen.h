#ifndef __KIWI_SYNTH_PLAY_SCREEN_H__
#define __KIWI_SYNTH_PLAY_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Controls/KiwiMcp23017.h"
#include "../Patch/Patch.h"
#include "EnumToText.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwisynth
{
    /*
     * The audio loop is only active when the display is showing the play screen. It will
     * display "Live Mode" if live mode is active. If a patch is loaded it will display the
     * bank number, patch number, and patch name.
     */
    class PlayScreen
    {
        public:
            PlayScreen(){}
            virtual ~PlayScreen(){};
            void Init(KiwiDisplay* display, Patch* patch);

            void Display();

        private:
            char buffer[32];
            Patch* patch;
            KiwiDisplay* display;
    };
} // namespace kiwisynth


#endif // __KIWI_SYNTH_PLAY_SCREEN_H__
