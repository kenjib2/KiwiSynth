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
        char val1[8], val2[8], val3[8];

        display->Fill(false);

        display->SetCursor(0, 0);
        GetVoiceMode(val1);
        GetWaveform(val2, 0);
        GetVcfType(val3);
        sprintf(buffer, "VM %s W1 %s VCF %s", val1, val2, val3);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        GetWaveform(val1, 1);
        sprintf(buffer, "W2 %s OCT %d INT %d", val1, patch->getActiveSettings()->getIntValue(VCO_2_OCTAVE) - 2, patch->getActiveSettings()->getIntValue(VCO_2_INTERVAL) - 11);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        GetWaveform(val1, 2);
        sprintf(buffer, "W3 %s OCT %d INT %d", val1, patch->getActiveSettings()->getIntValue(VCO_3_OCTAVE) - 2, patch->getActiveSettings()->getIntValue(VCO_3_INTERVAL) - 11);
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
    void IntValueScreen::GetVoiceMode(char* buffer)
    {
        switch (patch->getActiveSettings()->getIntValue(VCO_VOICES)) {
            case 0:
                strcpy(buffer, "2v");
                break;
            case 1:
                strcpy(buffer, "1v");
                break;
            case 2:
                strcpy(buffer, "Mt");
                break;
            #ifdef __FUNCTIONALITY_OPTION__
            case 3:
                strcpy(buffer, "3v");
                break;
            #endif // __FUNCTIONALITY_OPTION__
        }
    }

    // 2 character value
    void IntValueScreen::GetWaveform(char* buffer, int vcoNumber) {
        switch (patch->getActiveSettings()->getIntValue((PatchSetting)(VCO_1_WAVEFORM + vcoNumber))) {
            case 0:
                strcpy(buffer, "Sq");
                break;
            case 1:
                strcpy(buffer, "Sa");
                break;
            case 2:
                strcpy(buffer, "Tr");
                break;
        }
    }

    // 2 character value
    void IntValueScreen::GetNoiseType(char* buffer) {
        switch (patch->getActiveSettings()->getIntValue((PatchSetting)(VCO_NOISE_TYPE))) {
            case 0:
                strcpy(buffer, "Wh");
                break;
            case 1:
                strcpy(buffer, "Ds");
                break;
        }
    }

    // 2 character value
    void IntValueScreen::GetLfoWaveform(char* buffer, int lfoNumber) {
        switch (patch->getActiveSettings()->getIntValue((PatchSetting)(LFO_1_WAVEFORM + lfoNumber))) {
            case 0:
                strcpy(buffer, "Tr");
                break;
            case 1:
                strcpy(buffer, "Sq");
                break;
            case 2:
                strcpy(buffer, "Sa");
                break;
            case 3:
                strcpy(buffer, "Ra");
                break;
        }
    }

    // 2 character value
    void IntValueScreen::GetVcfType(char* buffer) {
        switch (patch->getActiveSettings()->getIntValue((PatchSetting)(VCF_FILTER_TYPE))) {
            case 0:
                strcpy(buffer, "LdLP");
                break;
            case 1:
                strcpy(buffer, "LoPa");
                break;
            case 2:
                strcpy(buffer, "HiPa");
                break;
            case 3:
                strcpy(buffer, "BndP");
                break;
            case 4:
                strcpy(buffer, "Notc");
                break;
            case 5:
                strcpy(buffer, "Peak");
                break;
            case 6:
                strcpy(buffer, "1PHP");
                break;
            case 7:
                strcpy(buffer, "1PLP");
                break;
        }
    }

    // 5 character value
    void IntValueScreen::GetModSource(char* buffer, int modNumber) {
        switch (patch->getActiveSettings()->getIntValue((PatchSetting)(MOD_1_SOURCE + modNumber))) {
            case 0:
                strcpy(buffer, "None ");
                break;
            case 1:
                strcpy(buffer, "Fixed");
                break;
            case 2:
                strcpy(buffer, "LFO 1");
                break;
            case 3:
                strcpy(buffer, "LFO 2");
                break;
            case 4:
                strcpy(buffer, "Env 1");
                break;
            case 5:
                strcpy(buffer, "Env 2");
                break;
            case 6:
                strcpy(buffer, "SmHld");
                break;
            case 7:
                strcpy(buffer, "Note ");
                break;
            case 8:
                strcpy(buffer, "Veloc");
                break;
            case 9:
                strcpy(buffer, "AftTo");
                break;
            case 10:
                strcpy(buffer, "ModWh");
                break;
            case 11:
                strcpy(buffer, "PBend");
                break;
            case 12:
                strcpy(buffer, "Expre");
                break;
            case 13:
                strcpy(buffer, "Susta");
                break;
            case 14:
                strcpy(buffer, "VcNum");
                break;
        }
    }

    // 2 character value
    void IntValueScreen::GetModDestination(char* buffer, int modNumber) {
        switch (patch->getActiveSettings()->getIntValue((PatchSetting)(MOD_1_DESTINATION + modNumber))) {
            case -1:
                strcpy(buffer, "None ");
                break;
            case 0:
                strcpy(buffer, "Frequ");
                break;
            case 1:
                strcpy(buffer, "PWdth");
                break;
            case 2:
                strcpy(buffer, "V1Frq");
                break;
            case 3:
                strcpy(buffer, "V1Pwd");
                break;
            case 4:
                strcpy(buffer, "V2Frq");
                break;
            case 5:
                strcpy(buffer, "V2Pwd");
                break;
            case 6:
                strcpy(buffer, "V3Frq");
                break;
            case 7:
                strcpy(buffer, "V3Pwd");
                break;
            case 8:
                strcpy(buffer, "NsLvl");
                break;
            case 9:
                strcpy(buffer, "SHRat");
                break;
            case 10:
                strcpy(buffer, "VCFCt");
                break;
            case 11:
                strcpy(buffer, "VCFRs");
                break;
            case 12:
                strcpy(buffer, "VCALv");
                break;
            case 13:
                strcpy(buffer, "VCAE1");
                break;
            case 14:
                strcpy(buffer, "L1Frq");
                break;
            case 15:
                strcpy(buffer, "L1Pwd");
                break;
            case 16:
                strcpy(buffer, "L1Phs");
                break;
            case 17:
                strcpy(buffer, "L2Frq");
                break;
            case 18:
                strcpy(buffer, "L2Pwd");
                break;
            case 19:
                strcpy(buffer, "L2Phs");
                break;
            case 20:
                strcpy(buffer, "E1Atk");
                break;
            case 21:
                strcpy(buffer, "E1Dec");
                break;
            case 22:
                strcpy(buffer, "E1Sus");
                break;
            case 23:
                strcpy(buffer, "E1Rel");
                break;
            case 24:
                strcpy(buffer, "E2Atk");
                break;
            case 25:
                strcpy(buffer, "E2Dec");
                break;
            case 26:
                strcpy(buffer, "E2Sus");
                break;
            case 27:
                strcpy(buffer, "E2Rel");
                break;
            case 28:
                strcpy(buffer, "Balnc");
                break;
        }
    }

} // namespace kiwi_synth
