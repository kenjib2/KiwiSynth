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
        int intVal1, intVal2;
        bool boolVal1;
        PatchSettings* settings;

        if (voiceNumber == 0) {
            settings = patch->voice1Settings;
        } else {
            settings = patch->voice2Settings;
        }

        display->Fill(false);

        display->SetCursor(0, 0);
        boolVal1 = settings->getBoolValue(VCO_PORTAMENTO_ON);
        intVal1 = (int)(settings->getFloatValue(VCO_MASTER_TUNE) * 1000.f);
        intVal2 = (int)(settings->getFloatValue(VCO_PORTAMENTO_SPEED) * 1000.f);
        if (!boolVal1) {
            sprintf(buffer, "Vc %d  MTn %03d Prt Off", voiceNumber + 1, intVal1);
        } else {
            sprintf(buffer, "Vc %d  MTn %03d Prt %03d", voiceNumber + 1, intVal1, intVal2);
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        intVal1 = settings->getFloatValue(VCO_1_LEVEL) * 1000;
        sprintf(buffer, "VCO 1       Level %03d", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        intVal1 = settings->getFloatValue(VCO_1_PULSE_WIDTH) * 1000;
        sprintf(buffer, "Pulse Width %03d", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        boolVal1 = settings->getBoolValue(VCO_2_ON);
        if (!boolVal1) {
            sprintf(buffer, "VCO 2 Off");
            display->WriteString(buffer, Font_6x8, true);
        } else {
            intVal1 = settings->getFloatValue(VCO_2_LEVEL) * 1000;
            sprintf(buffer, "VCO 2       Level %03d", intVal1);
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 32);
            intVal1 = settings->getFloatValue(VCO_2_PULSE_WIDTH) * 1000;
            intVal2 = settings->getFloatValue(VCO_2_FINE_TUNE) * 1000;
            sprintf(buffer, "PWidth %03d FineTn %03d", intVal1, intVal2);
            display->WriteString(buffer, Font_6x8, true);
        }

        display->SetCursor(0, 40);
        boolVal1 = settings->getBoolValue(VCO_3_ON);
        if (!boolVal1) {
            sprintf(buffer, "VCO 3 Off");
            display->WriteString(buffer, Font_6x8, true);
        } else {
            intVal1 = settings->getFloatValue(VCO_3_LEVEL) * 1000;
            sprintf(buffer, "VCO 3       Level %03d", intVal1);
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 48);
            intVal1 = settings->getFloatValue(VCO_3_PULSE_WIDTH) * 1000;
            intVal2 = settings->getFloatValue(VCO_3_FINE_TUNE) * 1000;
            sprintf(buffer, "PWidth %03d FineTn %03d", intVal1, intVal2);
            display->WriteString(buffer, Font_6x8, true);
        }

        display->SetCursor(0, 56);
        boolVal1 = settings->getBoolValue(VCO_NOISE_ON);
        if (!boolVal1) {
            sprintf(buffer, "Ns Off");
        } else {
            intVal1 = settings->getFloatValue(VCO_NOISE_LEVEL) * 1000;
            sprintf(buffer, "NsL %03d", intVal1);
        }
        display->WriteString(buffer, Font_6x8, true);
        display->SetCursor(48, 56);
        boolVal1 = settings->getBoolValue(VCO_INPUT_ON);
        if (!boolVal1) {
            sprintf(buffer, "In Off");
        } else {
            intVal1 = settings->getFloatValue(VCO_INPUT_THRESHOLD) * 1000;
            intVal2 = settings->getFloatValue(VCO_INPUT_LEVEL) * 1000;
            sprintf(buffer, "InTh %03d L%03d", intVal1, intVal2);
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
        intVal1 = settings->getFloatValue(VCA_LEVEL) * 1000;
        intVal2 = settings->getFloatValue(VCA_ENV_1_DEPTH) * 1000;
        sprintf(buffer, "VCA Lvl %03d Env1 %03d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);*/

        display->Update();
    }

    void FloatValue1Screen::Click() {
        voiceNumber = voiceNumber ^ 1;
    }

} // namespace kiwi_synth
