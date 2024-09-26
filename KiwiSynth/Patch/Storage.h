#ifndef __KIWI_SYNTH_STORAGE_H__
#define __KIWI_SYNTH_STORAGE_H__


#include "daisy_seed.h"

#include "Patch.h"
#include "SavedPatch.h"

using namespace daisy;

namespace kiwi_synth
{
    const static uint32_t QSPI_BASE_ADDRESS = 0x90100000;
    const static uint32_t PATCHES_BASE_ADDRESS = QSPI_BASE_ADDRESS;
    const static uint32_t VALIDATION_BASE_ADDRESS = QSPI_BASE_ADDRESS + 2097152;
    const static char VALIDATION_CODE[32] = "BOMBASTIC_AUDIO_KIWI___00.00.01"; // 32 characters including terminating null at the end
    const static int VALIDATION_SIZE = sizeof(VALIDATION_CODE); // 32;
    const static int PATCH_SIZE = 2048; // Storage set aside for one patch
    const static int PATCH_WRITE_SIZE = 1228; // Actual write size for one patch
    const static int PATCH_GROUP_SIZE = 4096; // Storage set aside for a group of four patches, and minimum eraseable size for flash
    const static int BANK_SIZE = 262144; // Storage set aside for 32 groups, or 128 patches.

    /*
     * Storage addresses:
     * Single SavedPatch data size: 1228 but will pad out to 2048 bytes total
     * Each 2 patches is 4096 -- the minimum required erase. This limits erase/write to only 2 patches at a time and extends flash memory lifespan. (100,000 erases)
     * Each bank is 256k, or 64 groups of 2 patches each
     *       0 -  262143: Patch bank 1
     *  262144 -  524287: Patch bank 2
     *  524288 -  786431: Patch bank 3
     *  786432 - 1048575: Patch bank 4
     * 1048576 - 1310719: Patch bank 5
     * 1310720 - 1572863: Patch bank 6
     * 1572864 - 1835007: Patch bank 7
     * 1835008 - 2097151: Patch bank 8
     * 2097152: 32 character validation/version code.
     */
    class Storage
    {
        private:
            QSPIHandle qspi;

            void InitializeIfNeeded();
            void ReadValidationCode(char* buffer);
            void WriteValidationCode();
            void WritePatchGroup(int bankNumber, int groupNumber, SavedPatch patches[4]);
            void ReadPatch(int bankNumber, int patchNumber, SavedPatch* patch);

        public:
            void Init(QSPIHandle qspi);
            void Test(char* buffer);

    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_STORAGE_H__
