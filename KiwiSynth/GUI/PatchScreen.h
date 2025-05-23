#ifndef __KIWI_SYNTH_PATCH_SCREEN_H__
#define __KIWI_SYNTH_PATCH_SCREEN_H__



#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/Patch.h"
#include "EnumToText.h"



using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;



namespace kiwisynth
{



// These are all possible selected menu items on this screen.
enum PatchScreenSelection
{
    PATCH_SCREEN_NONE = -1,
    PATCH_SCREEN_NAME,
    PATCH_SCREEN_TYPE,
    PATCH_SCREEN_VOICES,
    PATCH_SCREEN_SPLIT_NOTE,
    PATCH_SCREEN_FX,
    PATCH_SCREEN_REVERB,
    PATCH_SCREEN_LIVE,
    PATCH_SCREEN_SWAP,
    PATCH_SCREEN_LOAD,
    PATCH_SCREEN_SAVE,
    PATCH_SCREEN_RETURN
};
const static int PATCH_SCREEN_OPTIONS = 11;

// These are response messages sent back to Display depending on what the user selects. They
// instruct Display to perform different actions after the patch screen is exited.
enum PatchScreenResponse
{
    PATCH_SCREEN_RESPONSE_EDIT,
    PATCH_SCREEN_RESPONSE_NOEDIT,
    PATCH_SCREEN_RESPONSE_PLAY,
    PATCH_SCREEN_RESPONSE_LOAD,
    PATCH_SCREEN_RESPONSE_SAVE
};



/*
 * Change patch level settings as well as saving and loading patches. This is also where
 * the option to return to Live Mode is.
 */
class PatchScreen
{
    private:
        char buffer_[32];
        char value_[24];
        char nameEditBuffer_[MAX_PATCH_NAME_LENGTH + 1];
        KiwiDisplay* display_;
        Patch* patch_;
        int letterSelected_; // Used for editing patch name
        bool editingName_; // Used for editing patch name
        bool editingSplitNote_; // Used for editing the split note value

    public:
        PatchScreenSelection selected_;

        PatchScreen(){}
        virtual ~PatchScreen(){}
        void Init(KiwiDisplay* display, Patch* patch);

        void Display();
        void Increment();
        void Decrement();
        PatchScreenResponse Click();
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_PATCH_SCREEN_H__
