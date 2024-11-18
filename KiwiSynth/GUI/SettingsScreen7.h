#ifndef __KIWI_SYNTH_SETTINGS_SCREEN_7_H__
#define __KIWI_SYNTH_SETTINGS_SCREEN_7_H__



#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/Patch.h"
#include "EnumToText.h"



using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;



namespace kiwisynth
{



// These are all possible selected menu items on this screen.
enum SettingsScreen7Selection
{
    SETTINGS_SCREEN_7_NONE = -1,
    SETTINGS_SCREEN_7_EFFECTS,
    SETTINGS_SCREEN_7_REVERB,
    SETTINGS_SCREEN_7_RETURN
};
const static int SETTINGS_SCREEN_7_OPTIONS = 3;

// These are response messages sent back to Display depending on what the user selects. They
// instruct Display to perform different actions after the patch screen is exited.
enum SettingsScreen7Response
{
    SETTINGS_SCREEN_7_RESPONSE_EDIT,
    SETTINGS_SCREEN_7_RESPONSE_NOEDIT
};



/*
 * Settings screen 7 displays all of the data for the effects while also listening for any changes to the
 * values if in live mode. This allows you to see what is being changed.
 * 
 * Clicking alternates between settings 1 and 2 when in split or layered mode.
 */
class SettingsScreen7
{
    private:
        char buffer_[64];
        KiwiDisplay* display_;
        Patch* patch_;
        SettingsScreen7Selection selected_;

    public:
        SettingsScreen7(){}
        virtual ~SettingsScreen7(){}
        void Init(KiwiDisplay* display, Patch* );

        void Display();
        void Increment();
        void Decrement();
        SettingsScreen7Response Click();
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_SETTINGS_SCREEN_7_H__
