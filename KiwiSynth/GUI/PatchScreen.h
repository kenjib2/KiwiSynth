#ifndef __KIWI_SYNTH_PATCH_SCREEN_H__
#define __KIWI_SYNTH_PATCH_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/Patch.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
    class PatchScreen
    {
        public:
            PatchScreen(){}
            virtual ~PatchScreen(){};
            void Init(KiwiDisplay* display, Patch* patch);

            void Display();

        private:
            char buffer[256];
            KiwiDisplay* display;
            Patch* patch;
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_PATCH_SCREEN_H__
