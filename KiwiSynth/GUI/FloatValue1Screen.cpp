#include "FloatValue1Screen.h"

namespace kiwi_synth
{

    void FloatValue1Screen::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        this->voiceNumber = 0;
    }

    void FloatValue1Screen::Display()
    {
        int intVal1, intVal2, intVal3;
        bool boolVal1;
        PatchSettings* settings;
        VoiceMode voiceMode;

        voiceMode = (VoiceMode)patch->voice1Settings->getIntValue(OSC_VOICES);
        if (voiceNumber == 0 || (voiceMode != VOICE_MODE_MULTI && voiceMode != VOICE_MODE_SPLIT)) {
            settings = patch->voice1Settings;
        } else {
            settings = patch->voice2Settings;
        }

        display->Fill(false);

        display->SetCursor(0, 0);
        boolVal1 = settings->getBoolValue(OSC_PORTAMENTO_ON);
        intVal1 = (int)(settings->getFloatValue(GEN_BALANCE) * 1000.f);
        intVal2 = (int)(settings->getFloatValue(OSC_MASTER_TUNE) * 1000.f);
        intVal3 = (int)(settings->getFloatValue(OSC_PORTAMENTO_SPEED) * 1000.f);
        if (!boolVal1) {
            sprintf(buffer, "B %03d T %03d Pt Off V%d", intVal1, intVal2, voiceNumber + 1);
        } else {
            sprintf(buffer, "B %03d T %03d Pt %03d V%d", intVal1, intVal2, intVal3, voiceNumber + 1);
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        intVal1 = settings->getFloatValue(OSC_1_LEVEL) * 1000;
        sprintf(buffer, "Osc 1-------Level %03d", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        intVal1 = settings->getFloatValue(OSC_1_PULSE_WIDTH) * 1000;
        sprintf(buffer, "Pulse Width %03d", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        boolVal1 = settings->getBoolValue(OSC_2_ON);
        if (!boolVal1) {
            sprintf(buffer, "Osc 2 Off------------");
            display->WriteString(buffer, Font_6x8, true);
        } else {
            intVal1 = settings->getFloatValue(OSC_2_LEVEL) * 1000;
            sprintf(buffer, "Osc 2-------Level %03d", intVal1);
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 32);
            intVal1 = settings->getFloatValue(OSC_2_PULSE_WIDTH) * 1000;
            intVal2 = settings->getFloatValue(OSC_2_FINE_TUNE) * 1000;
            sprintf(buffer, "PWidth %03d FineTn %03d", intVal1, intVal2);
            display->WriteString(buffer, Font_6x8, true);
        }

        display->SetCursor(0, 40);
        boolVal1 = settings->getBoolValue(OSC_3_ON);
        if (!boolVal1) {
            sprintf(buffer, "Osc 3 Off------------");
            display->WriteString(buffer, Font_6x8, true);
        } else {
            intVal1 = settings->getFloatValue(OSC_3_LEVEL) * 1000;
            sprintf(buffer, "Osc 3-------Level %03d", intVal1);
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 48);
            intVal1 = settings->getFloatValue(OSC_3_PULSE_WIDTH) * 1000;
            intVal2 = settings->getFloatValue(OSC_3_FINE_TUNE) * 1000;
            sprintf(buffer, "PWidth %03d FineTn %03d", intVal1, intVal2);
            display->WriteString(buffer, Font_6x8, true);
        }

        display->SetCursor(0, 56);
        boolVal1 = settings->getBoolValue(OSC_NOISE_ON);
        if (!boolVal1) {
            sprintf(buffer, "Ns Off--");
        } else {
            intVal1 = settings->getFloatValue(OSC_NOISE_LEVEL) * 1000;
            sprintf(buffer, "NsL %03d-", intVal1);
        }
        display->WriteString(buffer, Font_6x8, true);
        display->SetCursor(48, 56);
        boolVal1 = settings->getBoolValue(OSC_INPUT_ON);
        if (!boolVal1) {
            sprintf(buffer, "In Off-------");
        } else {
            intVal1 = settings->getFloatValue(OSC_INPUT_THRESHOLD) * 1000;
            intVal2 = settings->getFloatValue(OSC_INPUT_LEVEL) * 1000;
            sprintf(buffer, "InTh %03d-L%03d", intVal1, intVal2);
        }
        display->WriteString(buffer, Font_6x8, true);

        /*display->SetCursor(0, 40);
        intVal1 = settings->getFloatValue(VCF_CUTOFF) * 1000;
        intVal2 = settings->getFloatValue(VCF_RESONANCE) * 1000;
        sprintf(buffer, "VCF FCut %03d Res %03d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        intVal1 = settings->getFloatValue(VCF_TRACKING) * 1000;
        intVal2 = settings->getFloatValue(VCF_ENV_1_DEPTH) * 1000;
        intVal3 = settings->getFloatValue(VCF_ENV_2_DEPTH) * 1000;
        sprintf(buffer, "KT %03d E1 %03d E2 %03d", intVal1, intVal2, intVal3);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        intVal1 = settings->getFloatValue(AMP_LEVEL) * 1000;
        intVal2 = settings->getFloatValue(AMP_ENV_1_DEPTH) * 1000;
        sprintf(buffer, "AMP Lvl %03d Env1 %03d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);*/

        display->Update();
    }

    void FloatValue1Screen::Click() {
        voiceNumber = voiceNumber ^ 1;
    }

} // namespace kiwi_synth
