#include "SysexManager.h"

namespace kiwi_synth
{
    uint8_t DSY_SDRAM_BSS sysexBuffer[KIWI_SYSEX_BUFFER_SIZE];

    void SysexManager::Init(MidiUartHandler* midi) {
        this->midi = midi;
    }

    void SysexManager::Send(Patch* patch) {
        SavedPatch savedPatch;
        patch->Save(&savedPatch);
        int size = BuildBuffer(&savedPatch);

        uint8_t sysexChar = 0xF0;
        midi->SendMessage(&sysexChar, 1);
        System::Delay(80);

        uint8_t sysexHeader[2];
        sysexHeader[0] = 0x7D; // Educational use manufacturer ID
        sysexHeader[1] = 0x00; // Model number
        midi->SendMessage(sysexHeader, 2);
        System::Delay(80);

        int chunkSize = 32;
        int numChunks = size / chunkSize + 1;
        for (int i = 0; i < numChunks; i++) {
            int nextSize = std::min(size - i * chunkSize, chunkSize);
            midi->SendMessage(sysexBuffer + i * chunkSize, nextSize);
            System::Delay(80);
        }

        sysexChar = 0xF7;
        midi->SendMessage(&sysexChar, 1);
    }

    void SysexManager::Receive(Patch* patch, MidiUartHandler* midi, GpioExpansion* ge) {
        while(1) {
            midi->Listen();
            while(midi->HasEvents())
            {
                MidiEvent event = midi->PopEvent();
                if (event.type == SystemCommon && event.sc_type == SystemExclusive) {
                    SystemExclusiveEvent sysexEvent = event.AsSystemExclusive();
                    for (int i = 0; i < sysexEvent.length; i++) {
                        uint8_t nextVal = sysexEvent.data[i];
                    }
                    return;
                }
            }
            ge->Process();
            if (patch->voice1Settings->getBoolValue(GEN_SELECT_BUTTON)) {
                return;
            }
            System::Delay(5);
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

    int SysexManager::WriteInt(int position, int value) {
        char bytes[4];
        IntToBytes(bytes, value);

        int bytesWritten = 0;
        for (size_t i = 0; i < 4; i++) {
            bytesWritten += WriteByte(position + i * 2, bytes[i]);
        } 

        return bytesWritten;
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

    int SysexManager::WriteBool(int position, bool value) {
        char bytes[1];
        BoolToBytes(bytes, value);

        return WriteByte(position, bytes[0]);
    }

    void SysexManager::IntToBytes(char* bytes, int intValue)
    {
        bytes[0] = (intValue >> 24) & 0xFF;
        bytes[1] = (intValue >> 16) & 0xFF;
        bytes[2] = (intValue >> 8) & 0xFF;
        bytes[3] = (intValue >> 0) & 0xFF;
    }

    void SysexManager::BoolToBytes(char* bytes, bool boolValue)
    {
        bytes[0] = boolValue;
    }

    void SysexManager::FloatToBytes(char* bytes, float floatValue)
    {
        FloatConvert fc;
        fc.floatValue = floatValue;

        for (int i = 0; i < 4; i++) {
            bytes[i] = fc.byteValue[i];
        }
    }

}
