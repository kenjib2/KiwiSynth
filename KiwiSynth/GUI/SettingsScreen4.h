#ifndef __KIWI_SYNTH_SETTINGS_SCREEN_4_H__
#define __KIWI_SYNTH_SETTINGS_SCREEN_4_H__



#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/Patch.h"
#include "EnumToText.h"



using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;



namespace kiwisynth
{



/*
 * Settings screen 4 displays all of the data for the LFOs while also listening for any changes to the
 * values if in live mode. This allows you to see what is being changed.
 * 
 * Clicking alternates between settings 1 and 2 when in split or layered mode.
 */
class SettingsScreen4
{
    private:
        char buffer_[32];
        KiwiDisplay* display_;
        Patch* patch_;
        int voiceNumber_;

    public:
        SettingsScreen4(){}
        virtual ~SettingsScreen4(){}
        void Init(KiwiDisplay* display, Patch* patch);

        void Display();
        void Click();
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_SETTINGS_SCREEN_4_H__
