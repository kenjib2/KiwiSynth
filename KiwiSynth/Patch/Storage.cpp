#include "Storage.h"

namespace kiwi_synth
{

    void Storage::Init(QSPIHandle qspi) {
        this->qspi = qspi;
        InitializeIfNeeded();
    }

    void Storage::InitializeIfNeeded() {
        char checkCode[32];
        ReadValidationCode(checkCode);
        if (strcmp(checkCode, VALIDATION_CODE) != 0) {
            WriteValidationCode();
            SavedPatch patchGroup[2];
            Patch patch;
            patch.DefaultSettings();
            patch.Save(&patchGroup[0]);
            patch.Save(&patchGroup[1]);
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 64; j++) {
                    WritePatchGroup(i, j, patchGroup);
                }
            }
        }
    }

    // buffer must be 32 character length minimum
    void Storage::ReadValidationCode(char* buffer) {
        void* readPtr = qspi.GetData(VALIDATION_BASE_ADDRESS);
        memcpy(buffer, readPtr, VALIDATION_SIZE);
    }

    void Storage::WriteValidationCode() {
        qspi.Erase(VALIDATION_BASE_ADDRESS, VALIDATION_BASE_ADDRESS + 4096);
        qspi.Write(VALIDATION_BASE_ADDRESS, VALIDATION_SIZE, (uint8_t*)VALIDATION_CODE);
    }
    
    void Storage::WritePatchGroup(int bankNumber, int groupNumber, SavedPatch patches[2]) {
        uint32_t writeAddr = PATCHES_BASE_ADDRESS + bankNumber * BANK_SIZE + groupNumber * PATCH_GROUP_SIZE;
        qspi.Erase(writeAddr, writeAddr + PATCH_GROUP_SIZE - 1);
        for (int i = 0; i < 2; i++) {
            uint8_t bytes[sizeof(SavedPatch)];
            memcpy(bytes, &(patches[i]), sizeof(SavedPatch));
            qspi.Write(writeAddr + i * PATCH_SIZE, PATCH_WRITE_SIZE, bytes);
        }
    }

    void Storage::ReadPatch(int bankNumber, int patchNumber, SavedPatch* patch) {
        void* readPtr = qspi.GetData(PATCHES_BASE_ADDRESS + bankNumber * BANK_SIZE + patchNumber * PATCH_SIZE);
        memcpy(patch, readPtr, sizeof(SavedPatch));
    }

    void Storage::Test(char* buffer) {
        SavedPatch patch;
        ReadPatch(3, 120, &patch);
        strcpy(buffer, patch.name);
    }

} // namespace kiwi_synth
