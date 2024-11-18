#include "SettingsScreen4.h"



using namespace kiwisynth;



void SettingsScreen4::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
    voiceNumber_ = 0;
}



void SettingsScreen4::Display()
{
    int intVal1, intVal2;
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
        sprintf(buffer_, "LFO 1------------- V%d", voiceNumber_ + 1);
    } else {
        sprintf(buffer_, "LFO 1----------------");
    }
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 8);
    EnumToText::GetLfoWaveform(charVal1, (LfoWaveform)(settings->getIntValue(LFO_1_WAVEFORM)));
    intVal1 = settings->getFloatValue(LFO_1_PULSE_WIDTH) * 1000;
    sprintf(buffer_, "Wave %s   PWdth %3d", charVal1, intVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 16);
    boolVal1 = settings->getBoolValue(LFO_1_TRIGGER_RESET_ON);
    intVal1 = settings->getFloatValue(LFO_1_RATE) * 1000;
    intVal2 = settings->getFloatValue(LFO_1_TRIGGER_PHASE) * 1000;
    if (boolVal1) {
        sprintf(buffer_, "Rate %3d  RstPhs %3d", intVal1, intVal2);
    } else {
        sprintf(buffer_, "Rate %3d  RstPhs Off", intVal1);
    }
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 24);
    intVal1 = settings->getFloatValue(LFO_1_TO_MASTER_TUNE) * 1000;
    sprintf(buffer_, "ToMasterTune %3d", intVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 32);
    sprintf(buffer_, "LFO 2----------------");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 40);
    EnumToText::GetLfoWaveform(charVal1, (LfoWaveform)(settings->getIntValue(LFO_2_WAVEFORM)));
    intVal1 = settings->getFloatValue(LFO_2_PULSE_WIDTH) * 1000;
    sprintf(buffer_, "Wave %s   PWdth %3d", charVal1, intVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 48);
    boolVal1 = settings->getBoolValue(LFO_2_TRIGGER_RESET_ON);
    intVal1 = settings->getFloatValue(LFO_2_RATE) * 1000;
    intVal2 = settings->getFloatValue(LFO_2_TRIGGER_PHASE) * 1000;
    if (boolVal1) {
        sprintf(buffer_, "Rate %3d  RstPhs %3d", intVal1, intVal2);
    } else {
        sprintf(buffer_, "Rate %3d  RstPhs Off", intVal1);
    }
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 56);
    intVal1 = settings->getFloatValue(LFO_2_TO_FLT_CUTOFF) * 1000;
    sprintf(buffer_, "ToFilterCutoff %3d", intVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->Update();
}



void SettingsScreen4::Click() {
    if (patch_->GetVoiceMode() == VOICE_MODE_MULTI || patch_->GetVoiceMode() == VOICE_MODE_SPLIT) {
        voiceNumber_ = voiceNumber_ ^ 1;
    } else {
        voiceNumber_ = 0;
    }
}
