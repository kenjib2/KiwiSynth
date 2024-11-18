#include "IntValueScreen.h"



using namespace kiwisynth;



void IntValueScreen::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
    selected_ = INT_SCREEN_NONE;
    voiceNumber_ = 0;
}



void IntValueScreen::Display()
{
    int8_t intVal1, intVal2;
    char val1[12], val2[12], val3[12];
    char modSign;
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
    EnumToText::GetWaveformTwoChar(val1, (Waveform)settings->getIntValue(OSC_1_WAVEFORM));
    EnumToText::GetFilterType(val2, (FilterType)settings->getIntValue(FLT_FILTER_TYPE));
    sprintf(buffer_, "W1 %s Flt %s", val1, val2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(114, 0);
    sprintf(buffer_, "V%d", voiceNumber_ + 1);
    display_->WriteString(buffer_, Font_6x8, selected_ != INT_SCREEN_VOICE);

    intVal1 = settings->getIntValue(OSC_2_OCTAVE) - 2;
    intVal2 = settings->getIntValue(OSC_2_INTERVAL) - 11;
    display_->SetCursor(0, 8);
    EnumToText::GetWaveformTwoChar(val1, (Waveform)settings->getIntValue(OSC_2_WAVEFORM));
    sprintf(buffer_, "W2 %s OCT %d INT %d", val1, intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    intVal1 = settings->getIntValue(OSC_3_OCTAVE) - 2;
    intVal2 = settings->getIntValue(OSC_3_INTERVAL) - 11;
    display_->SetCursor(0, 16);
    EnumToText::GetWaveformTwoChar(val1, (Waveform)settings->getIntValue(OSC_3_WAVEFORM));
    sprintf(buffer_, "W3 %s OCT %d INT %d", val1, intVal1, intVal2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 24);
    EnumToText::GetNoiseType(val1, (NoiseType)settings->getIntValue(OSC_NOISE_TYPE));
    EnumToText::GetLfoWaveform(val2, (LfoWaveform)settings->getIntValue(LFO_1_WAVEFORM));
    EnumToText::GetLfoWaveform(val3, (LfoWaveform)settings->getIntValue(LFO_2_WAVEFORM));
    sprintf(buffer_, "N %s L1 %s L2 %s", val1, val2, val3);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 32);
    EnumToText::GetModSource(val1, (ModulationSource)patch_->voice1Settings_->getIntValue(MOD_1_SOURCE));
    modSign = ' ';
    if (patch_->voice1Settings_->modSigns_[0] < 0.0f) {
        modSign = '-';
    }
    sprintf(buffer_, "SRC1%c%s", modSign, val1);
    display_->WriteString(buffer_, Font_6x8, selected_ != INT_SCREEN_SRC1);

    display_->SetCursor(66, 32);
    EnumToText::GetModDestination(val2, (ModulationDestination)patch_->voice1Settings_->getIntValue(MOD_1_DESTINATION));
    sprintf(buffer_, "DST1 %s", val2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 40);
    EnumToText::GetModSource(val1, (ModulationSource)patch_->voice1Settings_->getIntValue(MOD_2_SOURCE));
    modSign = ' ';
    if (patch_->voice1Settings_->modSigns_[1] < 0.0f) {
        modSign = '-';
    }
    sprintf(buffer_, "SRC2%c%s", modSign, val1);
    display_->WriteString(buffer_, Font_6x8, selected_ != INT_SCREEN_SRC2);

    display_->SetCursor(66, 40);
    EnumToText::GetModDestination(val2, (ModulationDestination)patch_->voice1Settings_->getIntValue(MOD_2_DESTINATION));
    sprintf(buffer_, "DST2 %s", val2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 48);
    EnumToText::GetModSource(val1, (ModulationSource)patch_->voice1Settings_->getIntValue(MOD_3_SOURCE));
    modSign = ' ';
    if (patch_->voice1Settings_->modSigns_[2] < 0.0f) {
        modSign = '-';
    }
    sprintf(buffer_, "SRC3%c%s", modSign, val1);
    display_->WriteString(buffer_, Font_6x8, selected_ != INT_SCREEN_SRC3);

    display_->SetCursor(66, 48);
    EnumToText::GetModDestination(val2, (ModulationDestination)patch_->voice1Settings_->getIntValue(MOD_3_DESTINATION));
    sprintf(buffer_, "DST3 %s", val2);
    display_->WriteString(buffer_, Font_6x8, true);

    display_->SetCursor(0, 56);
    EnumToText::GetModSource(val1, (ModulationSource)patch_->voice1Settings_->getIntValue(MOD_4_SOURCE));
    modSign = ' ';
    if (patch_->voice1Settings_->modSigns_[3] < 0.0f) {
        modSign = '-';
    }
    sprintf(buffer_, "SRC4%c%s", modSign, val1);
    display_->WriteString(buffer_, Font_6x8, selected_ != INT_SCREEN_SRC4);

    display_->SetCursor(66, 56);
    EnumToText::GetModDestination(val2, (ModulationDestination)patch_->voice1Settings_->getIntValue(MOD_4_DESTINATION));
    sprintf(buffer_, "DST4 %s", val2);
    display_->WriteString(buffer_, Font_6x8, true);

    if (selected_ != INT_SCREEN_NONE) {
        display_->SetCursor(102, 24);
        sprintf(buffer_, "<--");
        display_->WriteString(buffer_, Font_6x8, selected_ != INT_SCREEN_RETURN);
    }

    display_->Update();
}



void IntValueScreen::Increment() {
    selected_ = (IntScreenSelection)((selected_ + 1) % INT_SCREEN_OPTIONS);
    if (!patch_->GetLiveMode()) {
        if (selected_ < INT_SCREEN_VOICE) {
            selected_ = INT_SCREEN_VOICE;
        }
    }
}

void IntValueScreen::Decrement() {
    selected_ = (IntScreenSelection)((selected_ - 1 + INT_SCREEN_OPTIONS) % INT_SCREEN_OPTIONS);
    if (!patch_->GetLiveMode()) {
        if (selected_ < INT_SCREEN_VOICE) {
            selected_ = INT_SCREEN_RETURN;
        }
    }
}

IntScreenResponse IntValueScreen::Click() {
    switch (selected_) {
        case INT_SCREEN_NONE:
            selected_ = INT_SCREEN_VOICE;
            return INT_SCREEN_RESPONSE_EDIT;

        case INT_SCREEN_SRC1:
            patch_->voice1Settings_->modSigns_[0] *= -1.0f;
            return INT_SCREEN_RESPONSE_EDIT;

        case INT_SCREEN_SRC2:
            patch_->voice1Settings_->modSigns_[1] *= -1.0f;
            return INT_SCREEN_RESPONSE_EDIT;

        case INT_SCREEN_SRC3:
            patch_->voice1Settings_->modSigns_[2] *= -1.0f;
            return INT_SCREEN_RESPONSE_EDIT;

        case INT_SCREEN_SRC4:
            patch_->voice1Settings_->modSigns_[3] *= -1.0f;
            return INT_SCREEN_RESPONSE_EDIT;

        case INT_SCREEN_VOICE:
            voiceNumber_ = voiceNumber_ ^ 1;
            return INT_SCREEN_RESPONSE_EDIT;

        default:
        case INT_SCREEN_RETURN:
            selected_ = INT_SCREEN_NONE;
            return INT_SCREEN_RESPONSE_NOEDIT;
    }
}
