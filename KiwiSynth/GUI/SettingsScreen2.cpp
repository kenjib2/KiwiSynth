#include "SettingsScreen2.h"



using namespace kiwisynth;



void SettingsScreen2::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
    voiceNumber_ = 0;
}



void SettingsScreen2::Display()
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
    boolVal1 = settings->getBoolValue(OSC_NOISE_ON);
    if (!boolVal1) {
        if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
            sprintf(buffer_, "Noise Off--------- V%d", voiceNumber_ + 1);
        } else {
            sprintf(buffer_, "Noise Off------------");
        }
    } else {
        EnumToText::GetNoiseType(charVal1, (NoiseType)settings->getIntValue(OSC_NOISE_TYPE));
        intVal1 = settings->getFloatValue(OSC_NOISE_LEVEL) * 1000;
        if (voiceMode == VOICE_MODE_MULTI || voiceMode == VOICE_MODE_SPLIT) {
            sprintf(buffer_, "Noise-Tp %s-Lv %3d-V%d", charVal1, intVal1, voiceNumber_ + 1);
        } else {
            sprintf(buffer_, "Noise--Typ %s-Lvl %3d", charVal1, intVal1);
        }
    }
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 8);
    boolVal1 = settings->getBoolValue(OSC_INPUT_ON);
    if (!boolVal1) {
        sprintf(buffer_, "Ext In Off-----------");
    } else {
        intVal1 = settings->getFloatValue(OSC_INPUT_THRESHOLD) * 1000;
        intVal2 = settings->getFloatValue(OSC_INPUT_LEVEL) * 1000;
        sprintf(buffer_, "ExtIn--Thr %3d-Lv %3d", intVal1, intVal2);
    }
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 16);
    sprintf(buffer_, "Filter---------------");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 24);
    EnumToText::GetFilterType(charVal1, (FilterType)settings->getIntValue(FLT_FILTER_TYPE));
    intVal1 = settings->getFloatValue(FLT_TRACKING) * 1000;
    sprintf(buffer_, "Typ %s KTrk %3d", charVal1, intVal1);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 32);
    intVal1 = settings->getFloatValue(FLT_CUTOFF) * 1000;
    intVal2 = settings->getFloatValue(FLT_RESONANCE) * 1000;
    sprintf(buffer_, "FreqCut %3d  Reso %3d", intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 40);
    intVal1 = settings->getFloatValue(FLT_ENV_1_DEPTH) * 1000;
    intVal2 = settings->getFloatValue(FLT_ENV_2_DEPTH) * 1000;
    sprintf(buffer_, "Env1 %3d     Env2 %3d", intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 48);
    sprintf(buffer_, "Sample & Hold--------");
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 56);
    intVal1 = settings->getFloatValue(SH_TO_FLT_CUTOFF) * 1000;
    intVal2 = settings->getFloatValue(SH_RATE) * 1000;
    sprintf(buffer_, "ToFrCt %3d   Rate %3d", intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->Update();
}



void SettingsScreen2::Click() {
    if (patch_->GetVoiceMode() == VOICE_MODE_MULTI || patch_->GetVoiceMode() == VOICE_MODE_SPLIT) {
        voiceNumber_ = voiceNumber_ ^ 1;
    } else {
        voiceNumber_ = 0;
    }
}
