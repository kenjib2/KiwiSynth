#ifndef __KIWI_SYNTH_SELECT_SCREEN_H__
#define __KIWI_SYNTH_SELECT_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../KiwiSynth.h"
#include "../Patch/Patch.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{

    enum SelectScreenResponse
    {
        SELECT_SCREEN_RESPONSE_CANCEL,
        SELECT_SCREEN_RESPONSE_PLAY
    };

    class SelectScreen
    {
        public:
            SelectScreen(){}
            virtual ~SelectScreen(){};
            void Init(KiwiDisplay* display, KiwiSynth* kiwiSynth);
            bool saving;

            void Display();
            SelectScreenResponse Click();

        private:
            KiwiSynth* kiwiSynth;
            KiwiDisplay* display;
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_SELECT_SCREEN_H__
