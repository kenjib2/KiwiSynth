#ifndef __KIWI_SYNTH_INT_VALUE_SCREEN_H__
#define __KIWI_SYNTH_INT_VALUE_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../../KUtils.h"
#include "../Patch/PatchSettings.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
    class IntValueScreen
    {
        public:
            IntValueScreen(){}
            virtual ~IntValueScreen(){};
            void Init(KiwiDisplay* display, PatchSettings* patchSettings);

            void Display();

        private:
            char buffer[256];
            KiwiDisplay* display;
            PatchSettings* patchSettings;

            void GetVoiceMode(char* buffer);
            void GetWaveform(char* buffer, int vcoNumber);
            void GetNoiseType(char* buffer);
            void GetLfoWaveform(char* buffer, int lfoNumber);
            void GetVcfType(char* buffer);
            void GetModSource(char* buffer, int modNumber);
            void GetModDestination(char* buffer, int modNumber);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_INT_VALUE_SCREEN_H__
