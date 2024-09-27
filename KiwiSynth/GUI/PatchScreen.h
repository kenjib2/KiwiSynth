#ifndef __KIWI_SYNTH_PATCH_SCREEN_H__
#define __KIWI_SYNTH_PATCH_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../KiwiSynth.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
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
        PATCH_SCREEN_LOAD,
        PATCH_SCREEN_SAVE,
        PATCH_SCREEN_RETURN
    };
    const static int PATCH_SCREEN_OPTIONS = 10;

    class PatchScreen
    {
        public:
            PatchScreenSelection selected;

            PatchScreen(){}
            virtual ~PatchScreen(){};
            void Init(KiwiDisplay* display, KiwiSynth* kiwiSynth, Patch* patch);

            void Display();
            void Increment();
            void Decrement();
            bool Click();

        private:
            char buffer[41];
            char value[32];
            char nameEditBuffer[MAX_PATCH_NAME_LENGTH + 1];
            KiwiDisplay* display;
            KiwiSynth* kiwiSynth;
            Patch* patch;
            int letterSelected;
            bool editingName;
            bool editingSplitNote;

            void GetPatchType(char* buffer);
            void GetVoiceMode(char* buffer);
            void GetMidiNote(char* buffer);
            void GetFxType(char* buffer);
            void GetReverbType(char* buffer);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_PATCH_SCREEN_H__
