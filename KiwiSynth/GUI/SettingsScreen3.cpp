#include "SettingsScreen3.h"



using namespace kiwisynth;



void SettingsScreen3::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
    voiceNumber_ = 0;
}



void SettingsScreen3::Display()
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
        sprintf(buffer_, "Amplifier--------- V%d", voiceNumber_ + 1);
    } else {
        sprintf(buffer_, "Amplifier------------");
    }
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 8);
    intVal1 = settings->getFloatValue(AMP_LEVEL) * 1000;
    intVal2 = settings->getFloatValue(AMP_ENV_1_DEPTH) * 1000;
    sprintf(buffer_, "Level %3d    Env1 %3d", intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 24);
    sprintf(buffer_, "General--------------");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 32);
    EnumToText::GetVoiceMode(charVal1, voiceMode);
    sprintf(buffer_, "Mode: %s", charVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 40);
    intVal1 = (int)(settings->getFloatValue(GEN_BALANCE) * 1000.f);
    intVal2 = (int)(settings->getFloatValue(OSC_MASTER_TUNE) * 1000.f);
    sprintf(buffer_, "Balance %3d  Tune %3d", intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 48);
    boolVal1 = settings->getBoolValue(OSC_PORTAMENTO_ON);
    intVal1 = (int)(settings->getFloatValue(OSC_PORTAMENTO_SPEED) * 1000.f);
    if (boolVal1) {
        sprintf(buffer_, "Portamento On  Rt %3d", intVal1);
    } else {
        sprintf(buffer_, "Portamento Off");
    }
    display_->WriteString(buffer_, Font_6x8, true);

    display_->Update();
}



void SettingsScreen3::Click() {
    if (patch_->GetVoiceMode() == VOICE_MODE_MULTI || patch_->GetVoiceMode() == VOICE_MODE_SPLIT) {
        voiceNumber_ = voiceNumber_ ^ 1;
    } else {
        voiceNumber_ = 0;
    }
}
