#ifndef __KIWI_SYNTH_INT_VALUE_SCREEN_H__
#define __KIWI_SYNTH_INT_VALUE_SCREEN_H__

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

#include "../Patch/Patch.h"
#include "EnumToText.h"

using KiwiDisplay = OledDisplay<SSD130xI2c128x64Driver>;

namespace kiwi_synth
{
    /*
     * The int value screen displays all of the encoder based data for the patch while also listening
     * for any changes to the values if in live mode. This allows you to see what is being changed and
     * what the potential values are when using the encoders.
     * 
     * This screen does not have a menu and does not respond to clicks or internal scrolling. All
     * updates are made using the standard physical patch controls.
     */
    class IntValueScreen
    {
        public:
            IntValueScreen(){}
            virtual ~IntValueScreen(){};
            void Init(KiwiDisplay* display, Patch* patch);

            void Display();

        private:
            char buffer[256];
            KiwiDisplay* display;
            Patch* patch;

            // Functions for translating code numbers into text representations.
            void GetWaveform(char* buffer, int vcoNumber);
            void GetNoiseType(char* buffer);
            void GetLfoWaveform(char* buffer, int lfoNumber);
            void GetVcfType(char* buffer);
            void GetModSource(char* buffer, int modNumber);
            void GetModDestination(char* buffer, int modNumber);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_INT_VALUE_SCREEN_H__
