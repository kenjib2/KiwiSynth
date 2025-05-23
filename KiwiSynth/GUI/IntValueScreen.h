#ifndef __KIWI_SYNTH_INT_VALUE_SCREEN_H__
#define __KIWI_SYNTH_INT_VALUE_SCREEN_H__



#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/Patch.h"
#include "EnumToText.h"



using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwisynth
{



// These are all possible selected menu items on this screen.
enum IntScreenSelection
{
    INT_SCREEN_NONE = -1,
    INT_SCREEN_SRC1,
    INT_SCREEN_SRC2,
    INT_SCREEN_SRC3,
    INT_SCREEN_SRC4,
    INT_SCREEN_VOICE,
    INT_SCREEN_RETURN
};
const static int INT_SCREEN_OPTIONS = 6;

// These are response messages sent back to Display depending on what the user selects. They
// instruct Display to perform different actions after the patch screen is exited.
enum IntScreenResponse
{
    INT_SCREEN_RESPONSE_EDIT,
    INT_SCREEN_RESPONSE_NOEDIT
};



/*
 * The int value screen displays all of the encoder based data for the patch while also listening
 * for any changes to the values if in live mode. This allows you to see what is being changed and
 * what the potential values are when using the encoders.
 * 
 * This screen has an additional function to invert modulation polarity, allowing mod settings to
 * work as negative instead of positive modifiers.
 */
class IntValueScreen
{
    private:
        char buffer_[256];
        KiwiDisplay* display_;
        Patch* patch_;
        IntScreenSelection selected_;
        int voiceNumber_;

    public:
        IntValueScreen(){}
        virtual ~IntValueScreen(){}
        void Init(KiwiDisplay* display, Patch* patch);

        void Display();
        void Increment();
        void Decrement();
        IntScreenResponse Click();
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_INT_VALUE_SCREEN_H__
