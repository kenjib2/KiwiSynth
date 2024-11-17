#ifndef __KIWI_SYNTH_SYSEX_MANAGER_H__
#define __KIWI_SYNTH_SYSEX_MANAGER_H__

#include <vector>
#include "daisy_seed.h"
#include "Patch.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwisynth
{

    static const int KIWI_SYSEX_BUFFER_SIZE = 8192;

    union FloatConvert
    {
        float   floatValue;
        uint8_t byteValue[4];
    };

    class SysexManager
    {
        private:
            MidiUartHandler* midi;

            int BuildBuffer(SavedPatch* savedPatch);
            int WriteByte(int position, char value);
            int WriteInt(int position, int value);
            int WriteFloat(int position, float value);
            int WriteBool(int position, bool value);
            void IntToBytes(char* bytes, int intValue);
            void BoolToBytes(char* bytes, bool boolValue);
            void FloatToBytes(char* bytes, float floatValue);

        public:
            SysexManager() {}
            ~SysexManager() {}
            void Init(MidiUartHandler* midi);

            void Send(Patch* patch);
            void Receive(Patch* patch, MidiUartHandler* midi, GpioExpansion* ge);
    };

} // namespace kiwisynth
#endif // __KIWI_SYNTH_SYSEX_MANAGER_H__
