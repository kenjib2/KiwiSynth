#include "Storage.h"

namespace kiwi_synth
{

    void Storage::Init(QSPIHandle qspi) {
        this->qspi = qspi;
        InitializeIfNeeded();
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
    
    void Storage::InitializeIfNeeded() {
        char checkCode[32];
        ReadValidationCode(checkCode);
        if (strcmp(checkCode, VALIDATION_CODE) != 0) {
            WriteValidationCode();

            // Write 8 banks of initialized patches, 4 at a time.
        }
    }

    void Storage::Test(char* buffer) {
        ReadValidationCode(buffer);
    }

} // namespace kiwi_synth
