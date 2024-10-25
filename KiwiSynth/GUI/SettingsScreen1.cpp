#include "SettingsScreen1.h"

namespace kiwi_synth
{

    void SettingsScreen1::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        voiceNumber = 0;
    }

    void SettingsScreen1::Display()
    {
        int intVal1, intVal2, intVal3;
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
        if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
            sprintf(buffer, "Osc 1------------- V%d", voiceNumber + 1);
        } else {
            sprintf(buffer, "Osc 1----------------");
        }
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 8);
        EnumToText::GetWaveform(charVal1, (Waveform)settings->getIntValue(OSC_1_WAVEFORM));
        intVal1 = settings->getFloatValue(OSC_1_PULSE_WIDTH) * 1000;
        intVal2 = settings->getFloatValue(OSC_1_LEVEL) * 1000;
        sprintf(buffer, "Wv %s PW %3d Lv %3d", charVal1, intVal1, intVal2);
        display->WriteString(buffer, Font_6x8, true);

        display->SetCursor(0, 16);
        boolVal1 = settings->getBoolValue(OSC_2_ON);
        if (!boolVal1) {
            sprintf(buffer, "Osc 2 Off------------");
            display->WriteString(buffer, Font_6x8, true);
        } else {
            sprintf(buffer, "Osc 2----------------");
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 24);
            EnumToText::GetWaveform(charVal1, (Waveform)settings->getIntValue(OSC_2_WAVEFORM));
            intVal1 = settings->getFloatValue(OSC_2_PULSE_WIDTH) * 1000;
            intVal2 = settings->getFloatValue(OSC_2_LEVEL) * 1000;
            sprintf(buffer, "Wv %s PW %3d Lv %3d", charVal1, intVal1, intVal2);
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 32);
            intVal1 = settings->getIntValue(OSC_2_OCTAVE) - 2;
            intVal2 = settings->getIntValue(OSC_2_INTERVAL) - 11;
            intVal3 = settings->getFloatValue(OSC_2_FINE_TUNE) * 1000;
            sprintf(buffer, "Oct%2d Int%3d Fine %3d", intVal1, intVal2, intVal3);
            display->WriteString(buffer, Font_6x8, true);
        }

        display->SetCursor(0, 40);
        boolVal1 = settings->getBoolValue(OSC_3_ON);
        if (!boolVal1) {
            sprintf(buffer, "Osc 3 Off------------");
            display->WriteString(buffer, Font_6x8, true);
        } else {
            sprintf(buffer, "Osc 3----------------");
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 48);
            EnumToText::GetWaveform(charVal1, (Waveform)settings->getIntValue(OSC_3_WAVEFORM));
            intVal1 = settings->getFloatValue(OSC_3_PULSE_WIDTH) * 1000;
            intVal2 = settings->getFloatValue(OSC_3_LEVEL) * 1000;
            sprintf(buffer, "Wv %s PW %3d Lv %3d", charVal1, intVal1, intVal2);
            display->WriteString(buffer, Font_6x8, true);

            display->SetCursor(0, 56);
            intVal1 = settings->getIntValue(OSC_3_OCTAVE) - 2;
            intVal2 = settings->getIntValue(OSC_3_INTERVAL) - 11;
            intVal3 = settings->getFloatValue(OSC_3_FINE_TUNE) * 1000;
            sprintf(buffer, "Oct%2d Int%3d Fine %3d", intVal1, intVal2, intVal3);
            display->WriteString(buffer, Font_6x8, true);
        }

        display->Update();
    }

    void SettingsScreen1::Click() {
        if (patch->GetVoiceMode() == VOICE_MODE_MULTI || patch->GetVoiceMode() == VOICE_MODE_SPLIT) {
            voiceNumber = voiceNumber ^ 1;
        } else {
            voiceNumber = 0;
        }
    }

} // namespace kiwi_synth
