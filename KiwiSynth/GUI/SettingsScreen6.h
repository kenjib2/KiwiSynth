#ifndef __KIWI_SYNTH_SETTINGS_SCREEN_6_H__
#define __KIWI_SYNTH_SETTINGS_SCREEN_6_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/Patch.h"
#include "EnumToText.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
    // These are all possible selected menu items on this screen.
    enum SettingsScreen6Selection
    {
        SETTINGS_SCREEN_6_NONE = -1,
        SETTINGS_SCREEN_6_SRC1,
        SETTINGS_SCREEN_6_SRC2,
        SETTINGS_SCREEN_6_SRC3,
        SETTINGS_SCREEN_6_SRC4,
        SETTINGS_SCREEN_6_RETURN
    };
    const static int SETTINGS_SCREEN_6_OPTIONS = 5;

    // These are response messages sent back to Display depending on what the user selects. They
    // instruct Display to perform different actions after the patch screen is exited.
    enum SettingsScreen6Response
    {
        SETTINGS_SCREEN_6_RESPONSE_EDIT,
        SETTINGS_SCREEN_6_RESPONSE_NOEDIT
    };

    /*
     * Settings screen 6 displays all of the data for moodulations while also listening for any changes to the
     * values if in live mode. This allows you to see what is being changed.
     * 
     * Clicking alternates between settings 1 and 2 when in split or layered mode.
     */
    class SettingsScreen6
    {
        public:
            SettingsScreen6(){}
            virtual ~SettingsScreen6(){};
            void Init(KiwiDisplay* display, Patch* );

            void Display();
            void Increment();
            void Decrement();
            SettingsScreen6Response Click();

        private:
            char buffer[256];
            KiwiDisplay* display;
            Patch* patch;
            SettingsScreen6Selection selected;

    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_SETTINGS_SCREEN_6_H__
