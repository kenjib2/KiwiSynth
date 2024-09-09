#ifndef __KIWI_SYNTH_BOOTLOADER_SCREEN_H__
#define __KIWI_SYNTH_BOOTLOADER_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/PatchSettings.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
    class BootloaderScreen
    {
        public:
            BootloaderScreen(){}
            virtual ~BootloaderScreen(){};
            void Init(KiwiDisplay* display, PatchSettings* patchSettings);

            void Display();

        private:
            char buffer[256];
            KiwiDisplay* display;
            PatchSettings* patchSettings;
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_BOOTLOADER_SCREEN_H__
