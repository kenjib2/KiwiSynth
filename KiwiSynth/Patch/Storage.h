#ifndef __KIWI_SYNTH_STORAGE_H__
#define __KIWI_SYNTH_STORAGE_H__


#include "daisy_seed.h"

#include "Patch.h"
#include "SavedPatch.h"

using namespace daisy;

namespace kiwi_synth
{
    const static char validationCode[32] = "BOMBASTIC_AUDIO_KIWI_DATA_0.0.1";
    const static int VALIDATION_SIZE = sizeof(validationCode); // 32;
    const static int PATCHES_ADDRESS = 256;
    const static int PATCHES_SIZE = 768;

    /*
     * Storage addresses:
     * Validation Code: 0 to 31 -- will use 256 for block alignment
     * Patch Previews: (16 + 4) * 512 = 10240 = 256 to 10495 OR BUILD THIS DYNAMICALLY FROM SAVED PATCHES TO LESSEN RE-WRITES OR DON'T NEED AT ALL?
     * Saved Patch: 615 each -- will use 768 for 3 blocks. 256 + 768 * n to 256 to 768 * (n + 1)
     * End of data: 256 + 768 * 512 = 393472
     * 
     * Single SavedPatch data size: 21 for vars + 99 for bools + 99 for int8_ts + 396 for floats = 615
     *   
     */
    class Storage
    {
        private:
            QSPIHandle qspi;

            void InitializeIfNeeded();

        public:
            void Init(QSPIHandle qspi);
            void Test(char* buffer);
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_STORAGE_H__
