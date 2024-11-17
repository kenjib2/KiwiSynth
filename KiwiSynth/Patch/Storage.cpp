#include "Storage.h"

namespace kiwisynth
{

    void Storage::Init(QSPIHandle qspi) {
        this->qspi = qspi;
        InitializeIfNeeded();
    }

    void Storage::InitializeIfNeeded() {
        char checkCode[32];
        ReadValidationCode(checkCode);
        if (strcmp(checkCode, VALIDATION_CODE) != 0) {
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
            WriteValidationCode();
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

    SavedPatch Storage::LoadPatch(int bankNumber, int patchNumber) {
        SavedPatch savedPatch;
        void* readPtr = qspi.GetData(PATCHES_BASE_ADDRESS + bankNumber * BANK_SIZE + patchNumber * PATCH_SIZE);
        memcpy(&savedPatch, readPtr, sizeof(SavedPatch));
        return savedPatch;
    }

    void Storage::LoadPatch(SavedPatch* savedPatch, int bankNumber, int patchNumber) {
        void* readPtr = qspi.GetData(PATCHES_BASE_ADDRESS + bankNumber * BANK_SIZE + patchNumber * PATCH_SIZE);
        memcpy(&savedPatch, readPtr, sizeof(SavedPatch));
    }

    // Patches are always saved in pairs, so we have to load the corresponding partner and then save both patches together.
    void Storage::SavePatch(Patch* patch, int bankNumber, int patchNumber) {
        int patchGroup = patchNumber / 2;
        SavedPatch patchGroupData[2];

        if (patchNumber % 2 == 0) {
            // Save the first patch
            patch->Save(patchGroupData);

            // Copy the existing second patch
            void* readPtr = qspi.GetData(PATCHES_BASE_ADDRESS + bankNumber * BANK_SIZE + (patchNumber + 1) * PATCH_SIZE);
            memcpy(patchGroupData + 1, readPtr, sizeof(SavedPatch));

        } else {
            // Copy the existing first patch
            void* readPtr = qspi.GetData(PATCHES_BASE_ADDRESS + bankNumber * BANK_SIZE + (patchNumber - 1) * PATCH_SIZE);
            memcpy(patchGroupData, readPtr, sizeof(SavedPatch));

            // Save the second patch
            patch->Save(patchGroupData + 1);
        }

        WritePatchGroup(bankNumber, patchGroup, patchGroupData);
    }

} // namespace kiwisynth
