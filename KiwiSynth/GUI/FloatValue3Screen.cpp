#include "FloatValue3Screen.h"

namespace kiwi_synth
{

    void FloatValue3Screen::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        this->voiceNumber = 0;
    }

    void FloatValue3Screen::Display()
    {
        bool boolVal1;
        int intVal1, intVal2, intVal3;
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
        intVal1 = settings->getFloatValue(LFO_1_PULSE_WIDTH) * 1000;
        sprintf(buffer, "LFO1---PW %03d------V%d", intVal1, voiceNumber + 1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        boolVal1 = settings->getBoolValue(LFO_1_TRIGGER_RESET_ON);
        intVal1 = settings->getFloatValue(LFO_1_RATE) * 1000;
        intVal2 = settings->getFloatValue(LFO_1_TRIGGER_PHASE) * 1000;
        intVal3 = settings->getFloatValue(LFO_1_TO_MASTER_TUNE) * 1000;
        if (boolVal1) {
            sprintf(buffer, "Rt %03d Ph %03d Frq %03d", intVal1, intVal2, intVal3);
        } else {
            sprintf(buffer, "Rt %03d Ph Off Frq %03d", intVal1, intVal3);
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        intVal1 = settings->getFloatValue(LFO_2_PULSE_WIDTH) * 1000;
        sprintf(buffer, "LFO2---PW %03d--------", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        boolVal1 = settings->getBoolValue(LFO_2_TRIGGER_RESET_ON);
        intVal1 = settings->getFloatValue(LFO_2_RATE) * 1000;
        intVal2 = settings->getFloatValue(LFO_2_TRIGGER_PHASE) * 1000;
        intVal3 = settings->getFloatValue(LFO_2_TO_VCF_CUTOFF) * 1000;
        if (boolVal1) {
            sprintf(buffer, "Rt %03d Ph %03d FCt %03d", intVal1, intVal2, intVal3);
        } else {
            sprintf(buffer, "Rt %03d Ph Off FCt %03d", intVal1, intVal3);
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        boolVal1 = settings->getBoolValue(ENV_1_REVERSE_PHASE_ON);
        intVal1 = settings->getFloatValue(ENV_1_ATTACK) * 1000;
        sprintf(buffer, "ENV1----Rv %s-At %03d", boolVal1 ? "On-" : "Off", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        intVal1 = settings->getFloatValue(ENV_1_DECAY) * 1000;
        intVal2 = settings->getFloatValue(ENV_1_SUSTAIN) * 1000;
        intVal3 = settings->getFloatValue(ENV_1_RELEASE) * 1000;
        sprintf(buffer, "Dc %03d  Su %03d Rl %03d", intVal1, intVal2, intVal3);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        boolVal1 = settings->getBoolValue(ENV_2_REVERSE_PHASE_ON);
        intVal1 = settings->getFloatValue(ENV_2_ATTACK) * 1000;
        sprintf(buffer, "ENV2----Rv %s-At %03d", boolVal1 ? "On-" : "Off", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        intVal1 = settings->getFloatValue(ENV_2_DECAY) * 1000;
        intVal2 = settings->getFloatValue(ENV_2_SUSTAIN) * 1000;
        intVal3 = settings->getFloatValue(ENV_2_RELEASE) * 1000;
        sprintf(buffer, "Dc %03d  Su %03d Rl %03d", intVal1, intVal2, intVal3);
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

    void FloatValue3Screen::Click() {
        voiceNumber = voiceNumber ^ 1;
    }

} // namespace kiwi_synth
