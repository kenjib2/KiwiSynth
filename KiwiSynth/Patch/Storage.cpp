#include "Storage.h"

namespace kiwi_synth
{

    void Storage::Init(QSPIHandle qspi) {
        this->qspi = qspi;
        qspi.Write(0, VALIDATION_SIZE, (uint8_t*)validationCode);
        //InitializeIfNeeded();
    }
    
    void Storage::InitializeIfNeeded() {
        char checkCode[32];
        void* dataRead = qspi.GetData(0);

        memcpy(checkCode, dataRead, VALIDATION_SIZE);
        if (strcmp(checkCode, validationCode) != 0) {
            //qspi.Erase(0, 256);
            //qspi.Write(0, VALIDATION_SIZE, (uint8_t*)validationCode);
            
        /*    PatchPreview patchPreview;
            strcpy(patchPreview.name, "New");
            patchPreview.type = PATCH_SYNTH;
            for (int i = 0; i < TOTAL_PATCHES; i++) {
                qspi.Write(PREVIEWS_ADDRESS + i * PREVIEWS_SIZE, PREVIEWS_SIZE, reinterpret_cast<uint8_t*>(&patchPreview));
            }

            Patch patch;
            patch.DefaultSettings();
            SavedPatch savedPatch;
            patch.Save(&savedPatch);
            for (int i = 0; i < TOTAL_PATCHES; i++) {
                qspi.Write(PATCHES_ADDRESS + i * PATCHES_SIZE, PATCHES_SIZE, reinterpret_cast<uint8_t*>(&savedPatch));
            }*/
        }
    }

    void Storage::Test(char* buffer) {
        void* dataRead = qspi.GetData(0);

        memcpy(buffer, dataRead, VALIDATION_SIZE);
        //memcpy(buffer, "BOMBASTIC_AUDIO_KIWI_DATA_0.0.X", VALIDATION_SIZE);
    }

} // namespace kiwi_synth
