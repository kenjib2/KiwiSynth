#include "SettingsScreen4.h"

namespace kiwisynth
{

    void SettingsScreen4::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        voiceNumber = 0;
    }

    void SettingsScreen4::Display()
    {
        int intVal1, intVal2;
        char charVal1[12];
        bool boolVal1;
        PatchSettings* settings;
        VoiceMode voiceMode;

        voiceMode = (VoiceMode)patch->voice1Settings_->getIntValue(OSC_VOICES);
        if (voiceNumber == 0 || (voiceMode != VOICE_MODE_MULTI && voiceMode != VOICE_MODE_SPLIT)) {
            settings = patch->voice1Settings_;
        } else {
            settings = patch->voice2Settings_;
        }

        display->Fill(false);

        display->SetCursor(0, 0);
        if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
            sprintf(buffer, "LFO 1------------- V%d", voiceNumber + 1);
        } else {
            sprintf(buffer, "LFO 1----------------");
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        EnumToText::GetLfoWaveform(charVal1, (LfoWaveform)(settings->getIntValue(LFO_1_WAVEFORM)));
        intVal1 = settings->getFloatValue(LFO_1_PULSE_WIDTH) * 1000;
        sprintf(buffer, "Wave %s   PWdth %3d", charVal1, intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        boolVal1 = settings->getBoolValue(LFO_1_TRIGGER_RESET_ON);
        intVal1 = settings->getFloatValue(LFO_1_RATE) * 1000;
        intVal2 = settings->getFloatValue(LFO_1_TRIGGER_PHASE) * 1000;
        if (boolVal1) {
            sprintf(buffer, "Rate %3d  RstPhs %3d", intVal1, intVal2);
        } else {
            sprintf(buffer, "Rate %3d  RstPhs Off", intVal1);
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 24);
        intVal1 = settings->getFloatValue(LFO_1_TO_MASTER_TUNE) * 1000;
        sprintf(buffer, "ToMasterTune %3d", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 32);
        sprintf(buffer, "LFO 2----------------");
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 40);
        EnumToText::GetLfoWaveform(charVal1, (LfoWaveform)(settings->getIntValue(LFO_2_WAVEFORM)));
        intVal1 = settings->getFloatValue(LFO_2_PULSE_WIDTH) * 1000;
        sprintf(buffer, "Wave %s   PWdth %3d", charVal1, intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 48);
        boolVal1 = settings->getBoolValue(LFO_2_TRIGGER_RESET_ON);
        intVal1 = settings->getFloatValue(LFO_2_RATE) * 1000;
        intVal2 = settings->getFloatValue(LFO_2_TRIGGER_PHASE) * 1000;
        if (boolVal1) {
            sprintf(buffer, "Rate %3d  RstPhs %3d", intVal1, intVal2);
        } else {
            sprintf(buffer, "Rate %3d  RstPhs Off", intVal1);
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 56);
        intVal1 = settings->getFloatValue(LFO_2_TO_FLT_CUTOFF) * 1000;
        sprintf(buffer, "ToFilterCutoff %3d", intVal1);
        display->WriteString(buffer, Font_6x8, true);

        display->Update();
    }

    void SettingsScreen4::Click() {
        if (patch->GetVoiceMode() == VOICE_MODE_MULTI || patch->GetVoiceMode() == VOICE_MODE_SPLIT) {
            voiceNumber = voiceNumber ^ 1;
        } else {
            voiceNumber = 0;
        }
    }

} // namespace kiwisynth
