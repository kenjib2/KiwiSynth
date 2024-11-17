#ifndef __KIWI_SYNTH_SELECT_SCREEN_H__
#define __KIWI_SYNTH_SELECT_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../KiwiSynth.h"
#include "../Patch/Patch.h"
#include "EnumToText.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwisynth
{

    enum SelectScreenPage
    {
        SELECT_PAGE_SAVE_CONFIRM = -3,
        SELECT_PAGE_BANK_PATCHES = -2,
        SELECT_PAGE_TYPE_PATCHES = -1,
        SELECT_PAGE_BANKS = 0,
        SELECT_PAGE_TYPES
    };
    const static int maxSelectScreenPage = 2;

    enum SelectScreenResponse
    {
        SELECT_SCREEN_RESPONSE_CANCEL,
        SELECT_SCREEN_RESPONSE_REFRESH,
        SELECT_SCREEN_RESPONSE_PLAY
    };

    /*
     * Select a patch by either bank and patch number or patch type. This can be used for either loading or saving depending on
     * how the saving member variable is set.
     */
    class SelectScreen
    {
        public:
            SelectScreen(){}
            virtual ~SelectScreen(){};
            void Init(KiwiDisplay* display, KiwiSynth* kiwiSynth);
            bool saving;
            bool fromPlay;
            int selection;
            int bankNumber;
            int patchNumber;
            int saveBank;
            int savePatch;
            PatchType patchType;
            int patchTypePage;
            int patchTypeMax;
            SelectScreenPage currentPage;

            /*
            * There are five different display modes:
            *
            * SELECT_PAGE_PATCH_TYPES:  Lists all possible patch types, which when selected will drill down into a list
            *     of all availabale patches of that type.
            * SELECT_PAGE_BANKS: Lists all patch banks along with shortcuts to jump to certain ranges within each bank.
            * SELECT_PAGE_BANK_PATCHES: Lists a set of eight patches within the bank, along with arrows for scrolling to
            *     previous or subsequent sets of patches, and a cancel button.
            * SELECT_PAGE_TYPE_PATCHES: Lists a set of eight patches of a given patch type, along with arrows for scrolling
            *     to previous or subsequent sets of patches, and a cancel button.
            * SELECT_PAGE_SAVE_CONFIRM: Prompts to user to confirm that they wish to overwrite an existing patch when
            *     saving.
            */
            void Display();
            void Increment();
            void Decrement();
            /*
            * Handles selecting menu items.
            */
            SelectScreenResponse Click();

        private:
            char buffer[32];
            int numSelections;
            KiwiSynth* kiwiSynth;
            KiwiDisplay* display;
    };

} // namespace kiwisynth
#endif // __KIWI_SYNTH_SELECT_SCREEN_H__
