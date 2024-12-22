#ifndef __KIWI_SYNTH_SYSEX_MANAGER_H__
#define __KIWI_SYNTH_SYSEX_MANAGER_H__



#include <vector>
#include "daisy_seed.h"
#include "../Patch/Patch.h"
#include "KiwiMidi.h"



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
        KiwiMidiUartHandler* midi_;

        int BuildBuffer(SavedPatch* savedPatch);
        void BuildSavedPatch(SavedPatch* savedPatch);
        int WriteByte(int position, char value);
        int ReadByte(int position, char* value);
        int WriteInt(int position, int value);
        int ReadInt(int position, int* value);
        int WriteFloat(int position, float value);
        int ReadFloat(int position, float* value);
        int WriteBool(int position, bool value);
        int ReadBool(int position, bool* value);
        void IntToBytes(char* bytes, int intValue);
        int BytesToInt(char* bytes);
        void BoolToBytes(char* bytes, bool boolValue);
        bool BytesToBool(char* bytes);
        void FloatToBytes(char* bytes, float floatValue);
        float BytesToFloat(char* bytes);

    public:
        SysexManager() {}
        ~SysexManager() {}
        void Init(KiwiMidiUartHandler* midi);

        void Send(SavedPatch* savedPatch);
        // True if a patch was received and loaded
        bool Receive(SavedPatch* savedPatch, Patch* patch, GpioExpansion* ge);
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_SYSEX_MANAGER_H__
