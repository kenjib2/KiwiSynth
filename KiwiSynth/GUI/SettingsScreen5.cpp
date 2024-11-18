#include "SettingsScreen5.h"



using namespace kiwisynth;



void SettingsScreen5::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
    voiceNumber_ = 0;
}



void SettingsScreen5::Display()
{
    int intVal1, intVal2;
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
        sprintf(buffer_, "Envelope 1-------- V%d", voiceNumber_ + 1);
    } else {
        sprintf(buffer_, "Envelope 1-----------");
    }
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 8);
    intVal1 = settings->getFloatValue(ENV_1_ATTACK) * 1000;
    intVal2 = settings->getFloatValue(ENV_1_DECAY) * 1000;
    sprintf(buffer_, "Attack %3d  Decay %3d", intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 16);
    intVal1 = settings->getFloatValue(ENV_1_SUSTAIN) * 1000;
    intVal2 = settings->getFloatValue(ENV_1_RELEASE) * 1000;
    sprintf(buffer_, "Sustn %3d  Releas %3d", intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 24);
    boolVal1 = settings->getBoolValue(ENV_1_REVERSE_PHASE_ON);
    sprintf(buffer_, "Inverted %s", boolVal1 ? "On" : "Off");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 32);
    sprintf(buffer_, "Envelope 2-----------");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 40);
    intVal1 = settings->getFloatValue(ENV_2_ATTACK) * 1000;
    intVal2 = settings->getFloatValue(ENV_2_DECAY) * 1000;
    sprintf(buffer_, "Attack %3d  Decay %3d", intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 48);
    intVal1 = settings->getFloatValue(ENV_2_SUSTAIN) * 1000;
    intVal2 = settings->getFloatValue(ENV_2_RELEASE) * 1000;
    sprintf(buffer_, "Sustn %3d  Releas %3d", intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 56);
    boolVal1 = settings->getBoolValue(ENV_2_REVERSE_PHASE_ON);
    sprintf(buffer_, "Inverted %s", boolVal1 ? "On" : "Off");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->Update();
}



void SettingsScreen5::Click() {
    if (patch_->GetVoiceMode() == VOICE_MODE_MULTI || patch_->GetVoiceMode() == VOICE_MODE_SPLIT) {
        voiceNumber_ = voiceNumber_ ^ 1;
    } else {
        voiceNumber_ = 0;
    }
}
