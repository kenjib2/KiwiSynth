#include "SettingsScreen2.h"

namespace kiwisynth
{

    void SettingsScreen2::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        voiceNumber = 0;
    }

    void SettingsScreen2::Display()
    {
        int intVal1, intVal2;
        char charVal1[12];
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
        boolVal1 = settings->getBoolValue(OSC_NOISE_ON);
        if (!boolVal1) {
            if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
                sprintf(buffer, "Noise Off--------- V%d", voiceNumber + 1);
            } else {
                sprintf(buffer, "Noise Off------------");
            }
        } else {
            EnumToText::GetNoiseType(charVal1, (NoiseType)settings->getIntValue(OSC_NOISE_TYPE));
            intVal1 = settings->getFloatValue(OSC_NOISE_LEVEL) * 1000;
            if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
                sprintf(buffer, "Noise-Tp %s-Lv %3d-V%d", charVal1, intVal1, voiceNumber + 1);
            } else {
                sprintf(buffer, "Noise--Typ %s-Lvl %3d", charVal1, intVal1);
            }
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        boolVal1 = settings->getBoolValue(OSC_INPUT_ON);
        if (!boolVal1) {
            sprintf(buffer, "Ext In Off-----------");
        } else {
            intVal1 = settings->getFloatValue(OSC_INPUT_THRESHOLD) * 1000;
            intVal2 = settings->getFloatValue(OSC_INPUT_LEVEL) * 1000;
            sprintf(buffer, "ExtIn--Thr %3d-Lv %3d", intVal1, intVal2);
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        sprintf(buffer, "Filter---------------");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        EnumToText::GetFilterType(charVal1, (FilterType)settings->getIntValue(FLT_FILTER_TYPE));
        intVal1 = settings->getFloatValue(FLT_TRACKING) * 1000;
        sprintf(buffer, "Typ %s KTrk %3d", charVal1, intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        intVal1 = settings->getFloatValue(FLT_CUTOFF) * 1000;
        intVal2 = settings->getFloatValue(FLT_RESONANCE) * 1000;
        sprintf(buffer, "FreqCut %3d  Reso %3d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        intVal1 = settings->getFloatValue(FLT_ENV_1_DEPTH) * 1000;
        intVal2 = settings->getFloatValue(FLT_ENV_2_DEPTH) * 1000;
        sprintf(buffer, "Env1 %3d     Env2 %3d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        sprintf(buffer, "Sample & Hold--------");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        intVal1 = settings->getFloatValue(SH_TO_FLT_CUTOFF) * 1000;
        intVal2 = settings->getFloatValue(SH_RATE) * 1000;
        sprintf(buffer, "ToFrCt %3d   Rate %3d", intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

    void SettingsScreen2::Click() {
        if (patch->GetVoiceMode() == VOICE_MODE_MULTI || patch->GetVoiceMode() == VOICE_MODE_SPLIT) {
            voiceNumber = voiceNumber ^ 1;
        } else {
            voiceNumber = 0;
        }
    }

} // namespace kiwisynth
