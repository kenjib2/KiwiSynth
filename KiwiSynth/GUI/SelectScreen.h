#ifndef __KIWI_SYNTH_SELECT_SCREEN_H__
#define __KIWI_SYNTH_SELECT_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../KiwiSynth.h"
#include "../Patch/Patch.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{

    enum SelectScreenPage
    {
        SELECT_PAGE_BANKS,
        SELECT_PAGE_PATCH_TYPES
    };
    const static int maxSelectScreenPage = 2;

    enum SelectScreenResponse
    {
        SELECT_SCREEN_RESPONSE_CANCEL,
        SELECT_SCREEN_RESPONSE_REFRESH,
        SELECT_SCREEN_RESPONSE_PLAY
    };

    class SelectScreen
    {
        public:
            SelectScreen(){}
            virtual ~SelectScreen(){};
            void Init(KiwiDisplay* display, KiwiSynth* kiwiSynth);
            bool saving;
            bool fromPlay;
            int selection;
            SelectScreenPage currentPage;

            void Display();
            void Increment();
            void Decrement();
            SelectScreenResponse Click();

        private:
            char buffer[41];
            int numSelections;
            KiwiSynth* kiwiSynth;
            KiwiDisplay* display;

            void DisplayPatches(int bankNumber, int patchNumber);
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_SELECT_SCREEN_H__
