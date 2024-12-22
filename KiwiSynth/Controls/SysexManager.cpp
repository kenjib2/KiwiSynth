#include "SysexManager.h"



using namespace kiwisynth;



uint8_t DSY_SDRAM_BSS sysexBuffer[KIWI_SYSEX_BUFFER_SIZE];
int bufferPtr_;



void SysexManager::Init(KiwiMidiUartHandler* midi) {
    midi_ = midi;
}



void SysexManager::Send(SavedPatch* savedPatch) {
    int size = BuildBuffer(savedPatch);

    uint8_t sysexChar = 0xF0;
    midi_->SendMessage(&sysexChar, 1);
    System::Delay(10);

    uint8_t sysexHeader[2];
    sysexHeader[0] = 0x7D; // Educational use manufacturer ID
    sysexHeader[1] = 0x00; // Model number
    midi_->SendMessage(sysexHeader, 2);
    System::Delay(10);

    int chunkSize = 32;
    int numChunks = size / chunkSize + 1;
    for (int i = 0; i < numChunks; i++) {
        int nextSize = std::min(size - i * chunkSize, chunkSize);
        midi_->SendMessage(sysexBuffer + i * chunkSize, nextSize);
        System::Delay(10);
    }

    sysexChar = 0xF7;
    midi_->SendMessage(&sysexChar, 1);
}



bool SysexManager::Receive(SavedPatch* savedPatch, Patch* patch, GpioExpansion* ge) {
    bufferPtr_ = 0;

    while(1) {
        midi_->Listen();
        while(midi_->HasEvents())
        {
            KiwiMidiEvent event = midi_->PopEvent();
            if (event.type == SystemCommon && event.sc_type == SystemExclusive) {
                KiwiSystemExclusiveEvent sysexEvent = event.AsKiwiSystemExclusive();

                if (bufferPtr_ + sysexEvent.length <= KIWI_SYSEX_BUFFER_SIZE) {
                    memcpy(sysexBuffer + bufferPtr_, sysexEvent.data, sysexEvent.length);
                }
                bufferPtr_ += sysexEvent.length;

                if (event.finalSysexMessage) {
                    // Checking manufacturer and model number
                    if (sysexBuffer[0] == 0x7D && sysexBuffer[1] == 0x00) {
                        SavedPatch savedPatch;
                        BuildSavedPatch(&savedPatch);
                        return true;
                    }
                }
            }
        }
        ge->Process();
        if (patch->voice1Settings_->getBoolValue(GEN_SELECT_BUTTON)) {
            return false;
        }
        System::Delay(5);
    }

    return false;
}



void SysexManager::BuildSavedPatch(SavedPatch* savedPatch) {
    int position = 2;
    int value = 0;

    position += ReadInt(position, &value);
    savedPatch->type = (PatchType)value;
    position += ReadInt(position, &value);
    savedPatch->voiceMode = (VoiceMode)value;
    position += ReadInt(position, &value);
    savedPatch->splitNote = value;
    position += ReadInt(position, &value);
    savedPatch->effectsMode = (EffectsMode)value;
    position += ReadInt(position, &value);
    savedPatch->reverbMode = (ReverbMode)value;

    for (int i = 0; i < MAX_PATCH_NAME_LENGTH + 1; i++) {
        char byte;
        position += ReadByte(position, &byte);
        savedPatch->name[i] = byte;
    }

    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += ReadFloat(position, &(savedPatch->voice1FloatValues[i]));
    }
    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        int nextInt;
        position += ReadInt(position, &nextInt);
        savedPatch->voice1IntValues[i] = nextInt;
    }
    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += ReadBool(position, &(savedPatch->voice1BoolValues[i]));
    }
    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += ReadFloat(position, &(savedPatch->voice2FloatValues[i]));
    }
    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        int nextInt;
        position += ReadInt(position, &nextInt);
        savedPatch->voice2IntValues[i] = nextInt;
    }
    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += ReadBool(position, &(savedPatch->voice2BoolValues[i]));
    }
}



int SysexManager::BuildBuffer(SavedPatch* savedPatch) {
    int position = 0;

    position += WriteInt(position, savedPatch->type);
    position += WriteInt(position, savedPatch->voiceMode);
    position += WriteInt(position, savedPatch->splitNote);
    position += WriteInt(position, savedPatch->effectsMode);
    position += WriteInt(position, savedPatch->reverbMode);
    for (int i = 0; i < MAX_PATCH_NAME_LENGTH + 1; i++) {
        position += WriteByte(position, savedPatch->name[i]);
    }

    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += WriteFloat(position, savedPatch->voice1FloatValues[i]);
    }
    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += WriteInt(position, savedPatch->voice1IntValues[i]);
    }
    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += WriteBool(position, savedPatch->voice1BoolValues[i]);
    }

    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += WriteFloat(position, savedPatch->voice2FloatValues[i]);
    }
    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += WriteInt(position, savedPatch->voice2IntValues[i]);
    }
    for (int i = 0; i < NUM_PATCH_SETTINGS_SAVED; i++) {
        position += WriteBool(position, savedPatch->voice2BoolValues[i]);
    }

    return position;
}



int SysexManager::WriteByte(int position, char value) {
    uint8_t valHigh = (value & 0b11110000) >> 4;
    uint8_t valLow = value & 0b00001111;
    sysexBuffer[position] = valHigh;
    sysexBuffer[position + 1] = valLow;

    return 2;
}



int SysexManager::ReadByte(int position, char* value) {
    *value = (sysexBuffer[position] << 4) + sysexBuffer[position + 1];

    return 2;
}



int SysexManager::WriteInt(int position, int value) {
    char bytes[4];
    IntToBytes(bytes, value);

    int bytesWritten = 0;
    for (size_t i = 0; i < 4; i++) {
        bytesWritten += WriteByte(position + i * 2, bytes[i]);
    } 

    return bytesWritten;
}



int SysexManager::ReadInt(int position, int* value) {
    char bytes[4];
    for (int i = 0; i < 4; i++) {
        ReadByte(position + i * 2, &(bytes[i]));
    }
    *value = BytesToInt(bytes);

    return 8;
}



int SysexManager::WriteFloat(int position, float value) {
    char bytes[4];
    FloatToBytes(bytes, value);

    int bytesWritten = 0;
    for (size_t i = 0; i < 4; i++) {
        bytesWritten += WriteByte(position + i * 2, bytes[i]);
    } 

    return bytesWritten;
}



int SysexManager::ReadFloat(int position, float* value) {
    char bytes[4];
    int bytesRead = 0;
    for (size_t i = 0; i < 4; i++) {
        bytesRead += ReadByte(position + i * 2, &(bytes[i]));
    } 

    *value = BytesToFloat(bytes);

    return bytesRead;
}



int SysexManager::WriteBool(int position, bool value) {
    char bytes[1];
    BoolToBytes(bytes, value);

    return WriteByte(position, bytes[0]);
}



// NEEDS  ReadByte()
int SysexManager::ReadBool(int position, bool* value) {

    char byte[1];
    int bytesRead = ReadByte(position, byte);
    
    *value = BytesToBool(byte);

    return bytesRead;
}



void SysexManager::IntToBytes(char* bytes, int intValue)
{
    bytes[0] = (intValue >> 24) & 0xFF;
    bytes[1] = (intValue >> 16) & 0xFF;
    bytes[2] = (intValue >> 8) & 0xFF;
    bytes[3] = (intValue >> 0) & 0xFF;
}



int SysexManager::BytesToInt(char* bytes)
{
    return (bytes[0] << 24)
         + (bytes[1] << 16)
         + (bytes[2] << 8)
         + bytes[3];
}



void SysexManager::BoolToBytes(char* bytes, bool boolValue)
{
    bytes[0] = boolValue;
}



bool SysexManager::BytesToBool(char* bytes)
{
    return *bytes;
}



void SysexManager::FloatToBytes(char* bytes, float floatValue)
{
    FloatConvert fc;
    fc.floatValue = floatValue;

    for (int i = 0; i < 4; i++) {
        bytes[i] = fc.byteValue[i];
    }
}



float SysexManager::BytesToFloat(char* bytes)
{
    FloatConvert fc;
    for (int i = 0; i < 4; i++) {
        fc.byteValue[i] = bytes[i];
    }

    return fc.floatValue;
}
