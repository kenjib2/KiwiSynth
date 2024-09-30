#include "IntValueScreen.h"

namespace kiwi_synth
{

    void IntValueScreen::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
    }

    void IntValueScreen::Display()
    {
        int8_t intVal1, intVal2;
        char val1[12], val2[12], val3[12];

        display->Fill(false);

        display->SetCursor(0, 0);
        GetWaveform(val1, 0);
        GetVcfType(val2);
        sprintf(buffer, "W1 %s   VCF %s", val1, val2);
        display->WriteString(buffer, Font_6x8, true);

        if (patch->GetLiveMode()) {
            intVal1 = patch->activeSettings->getIntValue(VCO_2_OCTAVE) - 2;
            intVal2 = patch->activeSettings->getIntValue(VCO_2_INTERVAL) - 11;
        } else {
            intVal1 = patch->loadedPatchSettings.getIntValue(VCO_2_OCTAVE) - 2;
            intVal2= patch->loadedPatchSettings.getIntValue(VCO_2_INTERVAL) - 11;
        }
        display->SetCursor(0, 8);
        GetWaveform(val1, 1);
        sprintf(buffer, "W2 %s OCT %d INT %d", val1, intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        if (patch->GetLiveMode()) {
            intVal1 = patch->activeSettings->getIntValue(VCO_3_OCTAVE) - 2;
            intVal2 = patch->activeSettings->getIntValue(VCO_3_INTERVAL) - 11;
        } else {
            intVal1 = patch->loadedPatchSettings.getIntValue(VCO_3_OCTAVE) - 2;
            intVal2= patch->loadedPatchSettings.getIntValue(VCO_3_INTERVAL) - 11;
        }
        display->SetCursor(0, 16);
        GetWaveform(val1, 2);
        sprintf(buffer, "W3 %s OCT %d INT %d", val1, intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        GetNoiseType(val1);
        GetLfoWaveform(val2, 0);
        GetLfoWaveform(val3, 1);
        sprintf(buffer, "N %s L1 %s L2 %s", val1, val2, val3);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        GetModSource(val1, 0);
        GetModDestination(val2, 0);
        sprintf(buffer, "SRC1 %s DST1 %s", val1, val2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        GetModSource(val1, 1);
        GetModDestination(val2, 1);
        sprintf(buffer, "SRC2 %s DST2 %s", val1, val2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        GetModSource(val1, 2);
        GetModDestination(val2, 2);
        sprintf(buffer, "SRC3 %s DST3 %s", val1, val2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        GetModSource(val1, 3);
        GetModDestination(val2, 3);
        sprintf(buffer, "SRC4 %s DST4 %s", val1, val2);
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

    // 2 character value
    void IntValueScreen::GetWaveform(char* buffer, int vcoNumber) {
        Waveform waveform;
        if (patch->GetLiveMode()) {
            waveform = (Waveform)patch->activeSettings->getIntValue((PatchSetting)(VCO_1_WAVEFORM + vcoNumber));
        } else {
            waveform = (Waveform)patch->loadedPatchSettings.getIntValue((PatchSetting)(VCO_1_WAVEFORM + vcoNumber));
        }
        EnumToText::GetWaveform(buffer, waveform);
    }

    // 2 character value
    void IntValueScreen::GetNoiseType(char* buffer) {
        int8_t noiseType;
        if (patch->GetLiveMode()) {
            noiseType = patch->activeSettings->getIntValue((PatchSetting)(VCO_NOISE_TYPE));
        } else {
            noiseType = patch->loadedPatchSettings.getIntValue((PatchSetting)(VCO_NOISE_TYPE));
        }
        EnumToText::GetNoiseType(buffer, noiseType);
    }

    // 2 character value
    void IntValueScreen::GetLfoWaveform(char* buffer, int lfoNumber) {
        int8_t lfoWaveform;
        if (patch->GetLiveMode()) {
            lfoWaveform = patch->activeSettings->getIntValue((PatchSetting)(LFO_1_WAVEFORM + lfoNumber));
        } else {
            lfoWaveform = patch->loadedPatchSettings.getIntValue((PatchSetting)(LFO_1_WAVEFORM + lfoNumber));
        }
        EnumToText::GetLfoWaveform(buffer, lfoWaveform);
    }

    // 8 character value
    void IntValueScreen::GetVcfType(char* buffer) {
        int8_t vcfType;
        if (patch->GetLiveMode()) {
            vcfType = patch->activeSettings->getIntValue((PatchSetting)(VCF_FILTER_TYPE));
        } else {
            vcfType = patch->loadedPatchSettings.getIntValue((PatchSetting)(VCF_FILTER_TYPE));
        }
        EnumToText::GetVcfType(buffer, vcfType);
    }

    // 5 character value
    void IntValueScreen::GetModSource(char* buffer, int modNumber) {
        int8_t modSource;
        if (patch->GetLiveMode()) {
            modSource = patch->activeSettings->getIntValue((PatchSetting)(MOD_1_SOURCE + modNumber));
        } else {
            modSource = patch->loadedPatchSettings.getIntValue((PatchSetting)(MOD_1_SOURCE + modNumber));
        }
        EnumToText::GetModSource(buffer, modSource);
    }

    // 2 character value
    void IntValueScreen::GetModDestination(char* buffer, int modNumber) {
        int8_t modDestination;
        if (patch->GetLiveMode()) {
            modDestination = patch->activeSettings->getIntValue((PatchSetting)(MOD_1_DESTINATION + modNumber));
        } else {
            modDestination = patch->loadedPatchSettings.getIntValue((PatchSetting)(MOD_1_DESTINATION + modNumber));
        }
        EnumToText::GetModDestination(buffer, modDestination);
    }

} // namespace kiwi_synth
