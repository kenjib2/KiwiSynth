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
    const static uint32_t VALIDATION_BASE_ADDRESS = QSPI_BASE_ADDRESS + 1048576;
    const static char VALIDATION_CODE[32] = "BOMBASTIC_AUDIO_KIWI___00.00.03"; // 32 characters including terminating null at the end
    const static int VALIDATION_SIZE = sizeof(VALIDATION_CODE); // 32;
    const static int PATCH_SIZE = 1024; // Storage set aside for one patch
    const static int PATCH_PAGE = 4096; // Minimum eraseable size for flash

    /*
     * Storage addresses:
     * Validation Code: 0 to 31 -- will use 256 for block alignment
     * Patch Previews: (16 + 4) * 512 = 10240 = 256 to 10495 OR BUILD THIS DYNAMICALLY FROM SAVED PATCHES TO LESSEN RE-WRITES OR DON'T NEED AT ALL?
     * Saved Patch: 615 each -- will use 768 for 3 blocks. 256 + 768 * n to 256 to 768 * (n + 1). Could also store blocks of 4 or 5 each in 4096 byte sections to limit erasures.
     * End of data: 256 + 768 * 512 = 393472
     * 
     * Single SavedPatch data size: 21 for vars + 99 for bools + 99 for int8_ts + 396 for floats = 615
     *   
     * 
     * 
     * 
     * Saved Patch: 615 but will pad out to 1024 bytes total
     * Each 4 patches is 4096 -- the minimum required erase. This limits erase/write to only 4 patches at a time and extends flash memory lifespan. (100,000 erases)
     * 
     * Each bank is 128k, or 32 groups of 4 patches each
     *      0 - 131071: Patch bank 1
     * 131072 - 262143: Patch bank 2
     * 262144 - 393215: Patch bank 3
     * 393216 - 524287: Patch bank 4
     * 524288 - 655359: Patch bank 5
     * 655360 - 786431: Patch bank 6
     * 786432 - 917503: Patch bank 7
     * 917504 - 1048575: Patch bank 8
     * 
     * 1048576: 32 character validation/version code.
     */
    class Storage
    {
        private:
            QSPIHandle qspi;

            void InitializeIfNeeded();
            void ReadValidationCode(char* buffer);
            void WriteValidationCode();

        public:
            void Init(QSPIHandle qspi);
            void Test(char* buffer);

    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_STORAGE_H__
