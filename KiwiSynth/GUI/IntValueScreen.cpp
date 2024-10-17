#include "IntValueScreen.h"

namespace kiwi_synth
{

    void IntValueScreen::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        selected = INT_SCREEN_NONE;
        voiceNumber = 0;
    }

    void IntValueScreen::Display()
    {
        int8_t intVal1, intVal2;
        char val1[12], val2[12], val3[12];
        char modSign;
        PatchSettings* settings;
        VoiceMode voiceMode;

        voiceMode = (VoiceMode)patch->voice1Settings->getIntValue(VCO_VOICES);
        if (voiceNumber == 0 || (voiceMode != VOICE_MODE_MULTI && voiceMode != VOICE_MODE_SPLIT)) {
            settings = patch->voice1Settings;
        } else {
            settings = patch->voice2Settings;
        }

        display->Fill(false);

        display->SetCursor(0, 0);
        EnumToText::GetWaveform(val1, (Waveform)settings->getIntValue(VCO_1_WAVEFORM));
        EnumToText::GetVcfType(val2, (FilterType)settings->getIntValue(VCF_FILTER_TYPE));
        sprintf(buffer, "W1 %s VCF %s", val1, val2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(114, 0);
        sprintf(buffer, "V%d", voiceNumber + 1);
        display->WriteString(buffer, Font_6x8, selected != INT_SCREEN_VOICE);

        intVal1 = settings->getIntValue(VCO_2_OCTAVE) - 2;
        intVal2 = settings->getIntValue(VCO_2_INTERVAL) - 11;
        display->SetCursor(0, 8);
        EnumToText::GetWaveform(val1, (Waveform)settings->getIntValue(VCO_2_WAVEFORM));
        sprintf(buffer, "W2 %s OCT %d INT %d", val1, intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        intVal1 = settings->getIntValue(VCO_3_OCTAVE) - 2;
        intVal2 = settings->getIntValue(VCO_3_INTERVAL) - 11;
        display->SetCursor(0, 16);
        EnumToText::GetWaveform(val1, (Waveform)settings->getIntValue(VCO_3_WAVEFORM));
        sprintf(buffer, "W3 %s OCT %d INT %d", val1, intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        EnumToText::GetNoiseType(val1, (NoiseType)settings->getIntValue(VCO_NOISE_TYPE));
        EnumToText::GetLfoWaveform(val2, (LfoWaveform)settings->getIntValue(LFO_1_WAVEFORM));
        EnumToText::GetLfoWaveform(val3, (LfoWaveform)settings->getIntValue(LFO_2_WAVEFORM));
        sprintf(buffer, "N %s L1 %s L2 %s", val1, val2, val3);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        EnumToText::GetModSource(val1, (ModulationSource)patch->voice1Settings->getIntValue(MOD_1_SOURCE));
        modSign = ' ';
        if (patch->voice1Settings->modSigns[0] < 0.0f) {
            modSign = '-';
        }
        sprintf(buffer, "SRC1%c%s", modSign, val1);
        display->WriteString(buffer, Font_6x8, selected != INT_SCREEN_SRC1);

        display->SetCursor(66, 32);
        EnumToText::GetModDestination(val2, (ModulationDestination)patch->voice1Settings->getIntValue(MOD_1_DESTINATION));
        sprintf(buffer, "DST1 %s", val2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        EnumToText::GetModSource(val1, (ModulationSource)patch->voice1Settings->getIntValue(MOD_2_SOURCE));
        modSign = ' ';
        if (patch->voice1Settings->modSigns[1] < 0.0f) {
            modSign = '-';
        }
        sprintf(buffer, "SRC2%c%s", modSign, val1);
        display->WriteString(buffer, Font_6x8, selected != INT_SCREEN_SRC2);

        display->SetCursor(66, 40);
        EnumToText::GetModDestination(val2, (ModulationDestination)patch->voice1Settings->getIntValue(MOD_2_DESTINATION));
        sprintf(buffer, "DST2 %s", val2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        EnumToText::GetModSource(val1, (ModulationSource)patch->voice1Settings->getIntValue(MOD_3_SOURCE));
        modSign = ' ';
        if (patch->voice1Settings->modSigns[2] < 0.0f) {
            modSign = '-';
        }
        sprintf(buffer, "SRC3%c%s", modSign, val1);
        display->WriteString(buffer, Font_6x8, selected != INT_SCREEN_SRC3);

        display->SetCursor(66, 48);
        EnumToText::GetModDestination(val2, (ModulationDestination)patch->voice1Settings->getIntValue(MOD_3_DESTINATION));
        sprintf(buffer, "DST3 %s", val2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        EnumToText::GetModSource(val1, (ModulationSource)patch->voice1Settings->getIntValue(MOD_4_SOURCE));
        modSign = ' ';
        if (patch->voice1Settings->modSigns[3] < 0.0f) {
            modSign = '-';
        }
        sprintf(buffer, "SRC4%c%s", modSign, val1);
        display->WriteString(buffer, Font_6x8, selected != INT_SCREEN_SRC4);

        display->SetCursor(66, 56);
        EnumToText::GetModDestination(val2, (ModulationDestination)patch->voice1Settings->getIntValue(MOD_4_DESTINATION));
        sprintf(buffer, "DST4 %s", val2);
        display->WriteString(buffer, Font_6x8, true);

        if (selected != INT_SCREEN_NONE) {
            display->SetCursor(102, 24);
            sprintf(buffer, "<--");
            display->WriteString(buffer, Font_6x8, selected != INT_SCREEN_RETURN);
        }

        display->Update();
    }

    void IntValueScreen::Increment() {
        selected = (IntScreenSelection)((selected + 1) % INT_SCREEN_OPTIONS);
        if (!patch->GetLiveMode()) {
            if (selected < INT_SCREEN_VOICE) {
                selected = INT_SCREEN_VOICE;
            }
        }
    }

    void IntValueScreen::Decrement() {
        selected = (IntScreenSelection)((selected - 1 + INT_SCREEN_OPTIONS) % INT_SCREEN_OPTIONS);
        if (!patch->GetLiveMode()) {
            if (selected < INT_SCREEN_VOICE) {
                selected = INT_SCREEN_RETURN;
            }
        }
    }

    IntScreenResponse IntValueScreen::Click() {
        switch (selected) {
            case INT_SCREEN_NONE:
                selected = INT_SCREEN_VOICE;
                return INT_SCREEN_RESPONSE_EDIT;

            case INT_SCREEN_SRC1:
                patch->voice1Settings->modSigns[0] *= -1.0f;
                return INT_SCREEN_RESPONSE_EDIT;

            case INT_SCREEN_SRC2:
                patch->voice1Settings->modSigns[1] *= -1.0f;
                return INT_SCREEN_RESPONSE_EDIT;

            case INT_SCREEN_SRC3:
                patch->voice1Settings->modSigns[2] *= -1.0f;
                return INT_SCREEN_RESPONSE_EDIT;

            case INT_SCREEN_SRC4:
                patch->voice1Settings->modSigns[3] *= -1.0f;
                return INT_SCREEN_RESPONSE_EDIT;

            case INT_SCREEN_VOICE:
                voiceNumber = voiceNumber ^ 1;
                return INT_SCREEN_RESPONSE_EDIT;

            default:
            case INT_SCREEN_RETURN:
                selected = INT_SCREEN_NONE;
                return INT_SCREEN_RESPONSE_NOEDIT;
        }
    }

} // namespace kiwi_synth
