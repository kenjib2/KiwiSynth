#ifndef __KIWI_SYNTH_PLAY_SCREEN_H__
#define __KIWI_SYNTH_PLAY_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Controls/KiwiMcp23017.h"
#include "../Patch/Patch.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
    class PlayScreen
    {
        public:
            PlayScreen(){}
            virtual ~PlayScreen(){};
            void Init(KiwiDisplay* display, Patch* patch);

            void Display();

        private:
            char buffer[256];
            Patch* patch;
            KiwiDisplay* display;
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_PLAY_SCREEN_H__
