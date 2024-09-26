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
        char val1[12], val2[12], val3[12];

        display->Fill(false);

        display->SetCursor(0, 0);
        GetWaveform(val1, 0);
        GetVcfType(val2);
        sprintf(buffer, "W1 %s   VCF %s", val1, val2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        GetWaveform(val1, 1);
        sprintf(buffer, "W2 %s OCT %d INT %d", val1, patch->activeSettings->getIntValue(VCO_2_OCTAVE) - 2, patch->activeSettings->getIntValue(VCO_2_INTERVAL) - 11);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        GetWaveform(val1, 2);
        sprintf(buffer, "W3 %s OCT %d INT %d", val1, patch->activeSettings->getIntValue(VCO_3_OCTAVE) - 2, patch->activeSettings->getIntValue(VCO_3_INTERVAL) - 11);
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
        switch (patch->activeSettings->getIntValue(VCO_VOICES)) {
            case VOICE_MODE_POLY:
                strcpy(buffer, "Pl");
                break;
            case VOICE_MODE_MONO:
                strcpy(buffer, "Mo");
                break;
            case VOICE_MODE_MULTI:
                strcpy(buffer, "La");
                break;
            case VOICE_MODE_SPLIT:
                strcpy(buffer, "Sp");
                break;
        }
    }

    // 2 character value
    void IntValueScreen::GetWaveform(char* buffer, int vcoNumber) {
        switch (patch->activeSettings->getIntValue((PatchSetting)(VCO_1_WAVEFORM + vcoNumber))) {
            case WAVEFORM_SQUARE:
                strcpy(buffer, "Sq");
                break;
            case WAVEFORM_SAWTOOTH:
                strcpy(buffer, "Sa");
                break;
            case WAVEFORM_TRIANGLE:
                strcpy(buffer, "Tr");
                break;
            case WAVEFORM_VARISHAPE:
                strcpy(buffer, "Sh");
                break;
            case WAVEFORM_VARISAW:
                strcpy(buffer, "VS");
                break;
        }
    }

    // 2 character value
    void IntValueScreen::GetNoiseType(char* buffer) {
        switch (patch->activeSettings->getIntValue((PatchSetting)(VCO_NOISE_TYPE))) {
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
        switch (patch->activeSettings->getIntValue((PatchSetting)(LFO_1_WAVEFORM + lfoNumber))) {
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

    // 8 character value
    void IntValueScreen::GetVcfType(char* buffer) {
        switch (patch->activeSettings->getIntValue((PatchSetting)(VCF_FILTER_TYPE))) {
            case 0:
                strcpy(buffer, "LddrLoPa");
                break;
            case 1:
                strcpy(buffer, "LowPass");
                break;
            case 2:
                strcpy(buffer, "HighPass");
                break;
            case 3:
                strcpy(buffer, "BandPass");
                break;
            case 4:
                strcpy(buffer, "Notch");
                break;
            case 5:
                strcpy(buffer, "Peak");
                break;
            case 6:
                strcpy(buffer, "1PoleHiP");
                break;
            case 7:
                strcpy(buffer, "1PoleLoP");
                break;
        }
    }

    // 5 character value
    void IntValueScreen::GetModSource(char* buffer, int modNumber) {
        switch (patch->activeSettings->getIntValue((PatchSetting)(MOD_1_SOURCE + modNumber))) {
            case SRC_NONE:
                strcpy(buffer, "None ");
                break;
            case SRC_FIXED:
                strcpy(buffer, "Fixed");
                break;
            case SRC_LFO_1:
                strcpy(buffer, "LFO 1");
                break;
            case SRC_LFO_2:
                strcpy(buffer, "LFO 2");
                break;
            case SRC_ENV_1:
                strcpy(buffer, "Env 1");
                break;
            case SRC_ENV_2:
                strcpy(buffer, "Env 2");
                break;
            case SRC_SH:
                strcpy(buffer, "SmHld");
                break;
            case SRC_NOTE:
                strcpy(buffer, "Note ");
                break;
            case SRC_VELOCITY:
                strcpy(buffer, "Veloc");
                break;
            case SRC_AFTERTOUCH:
                strcpy(buffer, "AftTo");
                break;
            case SRC_MOD_WHEEL:
                strcpy(buffer, "ModWh");
                break;
            case SRC_PITCH_BEND:
                strcpy(buffer, "PBend");
                break;
            case SRC_EXPRESSION:
                strcpy(buffer, "Expre");
                break;
            case SRC_SUSTAIN:
                strcpy(buffer, "Susta");
                break;
            case SRC_VOICE_NO:
                strcpy(buffer, "Voice");
                break;
        }
    }

    // 2 character value
    void IntValueScreen::GetModDestination(char* buffer, int modNumber) {
        switch (patch->activeSettings->getIntValue((PatchSetting)(MOD_1_DESTINATION + modNumber))) {
            case DST_NONE:
                strcpy(buffer, "None ");
                break;
            case DST_VCOS_FREQ:
                strcpy(buffer, "Frequ");
                break;
            case DST_VCOS_PULSE_WIDTH:
                strcpy(buffer, "PWdth");
                break;
            case DST_VCO_1_FREQ:
                strcpy(buffer, "V1Frq");
                break;
            case DST_VCO_1_PULSE_WIDTH:
                strcpy(buffer, "V1Pwd");
                break;
            case DST_VCO_2_FREQ:
                strcpy(buffer, "V2Frq");
                break;
            case DST_VCO_2_PULSE_WIDTH:
                strcpy(buffer, "V2Pwd");
                break;
            case DST_VCO_3_FREQ:
                strcpy(buffer, "V3Frq");
                break;
            case DST_VCO_3_PULSE_WIDTH:
                strcpy(buffer, "V3Pwd");
                break;
            case DST_NOISE_DENSITY:
                strcpy(buffer, "NsDns");
                break;
            case DST_NOISE_LEVEL:
                strcpy(buffer, "NsLvl");
                break;
            case DST_SH_RATE:
                strcpy(buffer, "SHRat");
                break;
            case DST_VCF_CUTOFF:
                strcpy(buffer, "VCFCt");
                break;
            case DST_VCF_RESONANCE:
                strcpy(buffer, "VCFRs");
                break;
            case DST_VCA_LEVEL:
                strcpy(buffer, "VCALv");
                break;
            case DST_VCA_ENV_1_DEPTH:
                strcpy(buffer, "VCAE1");
                break;
            case DST_LFO_1_FREQ:
                strcpy(buffer, "L1Frq");
                break;
            case DST_LFO_1_PULSE_WIDTH:
                strcpy(buffer, "L1Pwd");
                break;
            case DST_LFO_1_TRIGGER_PHASE:
                strcpy(buffer, "L1Phs");
                break;
            case DST_LFO_2_FREQ:
                strcpy(buffer, "L2Frq");
                break;
            case DST_LFO_2_PULSE_WIDTH:
                strcpy(buffer, "L2Pwd");
                break;
            case DST_LFO_2_TRIGGER_PHASE:
                strcpy(buffer, "L2Phs");
                break;
            case DST_ENV_1_ATTACK:
                strcpy(buffer, "E1Atk");
                break;
            case DST_ENV_1_DECAY:
                strcpy(buffer, "E1Dec");
                break;
            case DST_ENV_1_SUSTAIN:
                strcpy(buffer, "E1Sus");
                break;
            case DST_ENV_1_RELEASE:
                strcpy(buffer, "E1Rel");
                break;
            case DST_ENV_2_ATTACK:
                strcpy(buffer, "E2Atk");
                break;
            case DST_ENV_2_DECAY:
                strcpy(buffer, "E2Dec");
                break;
            case DST_ENV_2_SUSTAIN:
                strcpy(buffer, "E2Sus");
                break;
            case DST_ENV_2_RELEASE:
                strcpy(buffer, "E2Rel");
                break;
            case DST_BALANCE:
                strcpy(buffer, "Balnc");
                break;
        }
    }

} // namespace kiwi_synth
