#include "SettingsScreen1.h"



using namespace kiwisynth;



void SettingsScreen1::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
    voiceNumber_ = 0;
}



void SettingsScreen1::Display()
{
    int intVal1, intVal2, intVal3;
    char charVal1[12];
    bool boolVal1;
    PatchSettings* settings;
    VoiceMode voiceMode;

    voiceMode = (VoiceMode)patch_->voice1Settings_->getIntValue(OSC_VOICES);
    if (voiceNumber_ == 0 || (voiceMode != VOICE_MODE_MULTI && voiceMode != VOICE_MODE_SPLIT)) {
        settings = patch_->voice1Settings_;
    } else {
        settings = patch_->voice2Settings_;
    }

    display_->Fill(false);

    display_->SetCursor(0, 0);
    if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
        sprintf(buffer_, "Osc 1------------- V%d", voiceNumber_ + 1);
    } else {
        sprintf(buffer_, "Osc 1----------------");
    }
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 8);
    EnumToText::GetWaveform(charVal1, (Waveform)settings->getIntValue(OSC_1_WAVEFORM));
    intVal1 = settings->getFloatValue(OSC_1_PULSE_WIDTH) * 1000;
    intVal2 = settings->getFloatValue(OSC_1_LEVEL) * 1000;
    sprintf(buffer_, "Wv %s PW %3d Lv %3d", charVal1, intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 16);
    boolVal1 = settings->getBoolValue(OSC_2_ON);
    if (!boolVal1) {
        sprintf(buffer_, "Osc 2 Off------------");
        display_->WriteString(buffer_, Font_6x8, true);
    } else {
        sprintf(buffer_, "Osc 2----------------");
        display_->WriteString(buffer_, Font_6x8, true);

        display_->SetCursor(0, 24);
        EnumToText::GetWaveform(charVal1, (Waveform)settings->getIntValue(OSC_2_WAVEFORM));
        intVal1 = settings->getFloatValue(OSC_2_PULSE_WIDTH) * 1000;
        intVal2 = settings->getFloatValue(OSC_2_LEVEL) * 1000;
        sprintf(buffer_, "Wv %s PW %3d Lv %3d", charVal1, intVal1, intVal2);
        display_->WriteString(buffer_, Font_6x8, true);

        display_->SetCursor(0, 32);
        intVal1 = settings->getIntValue(OSC_2_OCTAVE) - 2;
        intVal2 = settings->getIntValue(OSC_2_INTERVAL) - 11;
        intVal3 = settings->getFloatValue(OSC_2_FINE_TUNE) * 1000;
        sprintf(buffer_, "Oct%2d Int%3d Fine %3d", intVal1, intVal2, intVal3);
        display_->WriteString(buffer_, Font_6x8, true);
    }

    display_->SetCursor(0, 40);
    boolVal1 = settings->getBoolValue(OSC_3_ON);
    if (!boolVal1) {
        sprintf(buffer_, "Osc 3 Off------------");
        display_->WriteString(buffer_, Font_6x8, true);
    } else {
        sprintf(buffer_, "Osc 3----------------");
        display_->WriteString(buffer_, Font_6x8, true);

        display_->SetCursor(0, 48);
        EnumToText::GetWaveform(charVal1, (Waveform)settings->getIntValue(OSC_3_WAVEFORM));
        intVal1 = settings->getFloatValue(OSC_3_PULSE_WIDTH) * 1000;
        intVal2 = settings->getFloatValue(OSC_3_LEVEL) * 1000;
        sprintf(buffer_, "Wv %s PW %3d Lv %3d", charVal1, intVal1, intVal2);
        display_->WriteString(buffer_, Font_6x8, true);

        display_->SetCursor(0, 56);
        intVal1 = settings->getIntValue(OSC_3_OCTAVE) - 2;
        intVal2 = settings->getIntValue(OSC_3_INTERVAL) - 11;
        intVal3 = settings->getFloatValue(OSC_3_FINE_TUNE) * 1000;
        sprintf(buffer_, "Oct%2d Int%3d Fine %3d", intVal1, intVal2, intVal3);
        display_->WriteString(buffer_, Font_6x8, true);
    }

    display_->Update();
}



void SettingsScreen1::Click() {
    if (patch_->GetVoiceMode() == VOICE_MODE_MULTI || patch_->GetVoiceMode() == VOICE_MODE_SPLIT) {
        voiceNumber_ = voiceNumber_ ^ 1;
    } else {
        voiceNumber_ = 0;
    }
}
