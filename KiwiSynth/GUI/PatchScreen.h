#ifndef __KIWI_SYNTH_PATCH_SCREEN_H__
#define __KIWI_SYNTH_PATCH_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../KiwiSynth.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
    class PatchScreen
    {
        public:
            PatchScreen(){}
            virtual ~PatchScreen(){};
            void Init(KiwiDisplay* display, KiwiSynth* kiwiSynth, Patch* patch);

            void Display();

        private:
            char buffer[41];
            char value[18];
            KiwiDisplay* display;
            KiwiSynth* kiwiSynth;
            Patch* patch;

            void GetVoiceMode(char* buffer);
            void GetMidiNote(char* buffer);
            void GetFxType(char* buffer);
            void GetReverbType(char* buffer);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_PATCH_SCREEN_H__
