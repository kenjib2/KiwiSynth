#include "PatchScreen.h"



using namespace kiwisynth;



void PatchScreen::Init(KiwiDisplay* display, Patch* patch)
{
    display_ = display;
    patch_ = patch;
    selected_ = PATCH_SCREEN_NONE;
}



void PatchScreen::Display()
{
    display_->Fill(false);

    if (editingName_) {
        display_->SetCursor(0, 0);
        sprintf(buffer_, "Name: ");
        display_->WriteString(buffer_, Font_6x8, true);
        for (int i = 0; i < MAX_PATCH_NAME_LENGTH; i++) {
            display_->SetCursor(36  + i * 6, 0);
            sprintf(buffer_, "%c", nameEditBuffer_[i]);
            display_->WriteString(buffer_, Font_6x8, letterSelected_ != i);
        }
    } else {
        display_->SetCursor(0, 0);
        patch_->GetName(value_);
        sprintf(buffer_, "Name: %s", value_);
        display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_NAME);
    }

    display_->SetCursor(0, 8);
    EnumToText::GetPatchType(value_, patch_->GetPatchType());
    sprintf(buffer_, "Type: %s", value_);
    display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_TYPE);

    display_->SetCursor(0, 16);
    VoiceMode voiceMode;
    voiceMode = (VoiceMode)patch_->voice1Settings_->getIntValue(OSC_VOICES);
    EnumToText::GetVoiceMode(value_, voiceMode);
    sprintf(buffer_, "Voices: %s", value_);
    display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_VOICES);
    if (voiceMode == VOICE_MODE_SPLIT) {
        // For split voice mode, we have some extra code here to display and update the note upon which the voices are split.
        EnumToText::GetMidiNote(value_, patch_->GetSplitNote());
        if (editingSplitNote_) {
            display_->SetCursor(84, 16);
            sprintf(buffer_, "on ");
            display_->WriteString(buffer_, Font_6x8, true);

            display_->SetCursor(102, 16);
            sprintf(buffer_, "%s", value_);
            display_->WriteString(buffer_, Font_6x8, false);
        } else {
            display_->SetCursor(84, 16);
            sprintf(buffer_, "on %s", value_);
            display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_SPLIT_NOTE);
        }
    }

    display_->SetCursor(0, 24);
    EnumToText::GetEffectsMode(value_, patch_->GetEffectsMode());
    sprintf(buffer_, "FX: %s", value_);
    display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_FX);

    display_->SetCursor(0, 32);
    EnumToText::GetReverbMode(value_, patch_->GetReverbMode());
    sprintf(buffer_, "Reverb: %s", value_);
    display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_REVERB);

    if (!patch_->GetLiveMode()) {
        display_->SetCursor(0, 40);
        sprintf(buffer_, "Live Mode");
        display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_LIVE);

        if (patch_->GetVoiceMode() == VOICE_MODE_MULTI || patch_->GetVoiceMode() == VOICE_MODE_SPLIT) {
            display_->SetCursor(66, 40);
            sprintf(buffer_, "Swap Layers");
            display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_SWAP);
        }
    } else {
        if (patch_->GetVoiceMode() == VOICE_MODE_MULTI || patch_->GetVoiceMode() == VOICE_MODE_SPLIT) {
            display_->SetCursor(0, 40);
            sprintf(buffer_, "Swap Layers");
            display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_SWAP);
        }
    }

    display_->SetCursor(0, 48);
    sprintf(buffer_, "Load Patch");
    display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_LOAD);

    display_->SetCursor(0, 56);
    sprintf(buffer_, "Save Patch");
    display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_SAVE);

    if (selected_ != PATCH_SCREEN_NONE) {
        display_->SetCursor(108, 56);
        sprintf(buffer_, "<--");
        display_->WriteString(buffer_, Font_6x8, selected_ != PATCH_SCREEN_RETURN);
    }

    display_->Update();
}



void PatchScreen::Increment() {
    if (editingName_) {
        nameEditBuffer_[letterSelected_] = nameEditBuffer_[letterSelected_] + 1;
        // Valid patch name characters are 32-126 inclusive
        if (nameEditBuffer_[letterSelected_] > 126) {
            nameEditBuffer_[letterSelected_] = 32;
        }
    } else if (editingSplitNote_) {
        patch_->SetSplitNote((patch_->GetSplitNote() + 1) % 128);
    } else {
        selected_ = (PatchScreenSelection)((selected_ + 1) % PATCH_SCREEN_OPTIONS);

        // If in live mode, we can't select live mode again. Skip the menu item.
        if (patch_->GetLiveMode() && selected_ == PATCH_SCREEN_LIVE) {
            selected_ = PATCH_SCREEN_SWAP;
        }

        // Must come after the check for PATCH_SCREEN_LIVE
        if (selected_ == PATCH_SCREEN_SWAP && !(patch_->GetVoiceMode() == VOICE_MODE_MULTI || patch_->GetVoiceMode() == VOICE_MODE_SPLIT)) {
            selected_ = PATCH_SCREEN_LOAD;
        }

        // If the voice mode is not split mode, then skip selecction of the split note (since it is not displayed).
        if (selected_ == PATCH_SCREEN_SPLIT_NOTE && patch_->GetVoiceMode() != VOICE_MODE_SPLIT) {
            selected_ = (PatchScreenSelection)(selected_ + 1);
        }
    }
}



void PatchScreen::Decrement() {

    if (editingName_) {
        nameEditBuffer_[letterSelected_] = nameEditBuffer_[letterSelected_] - 1;
        // Valid patch name characters are 32-126 inclusive
        if (nameEditBuffer_[letterSelected_] < 32) {
            nameEditBuffer_[letterSelected_] = 126;
        }
    } else if (editingSplitNote_) {
        patch_->SetSplitNote((patch_->GetSplitNote() - 1 + 128) % 128);
    } else {
        selected_ = (PatchScreenSelection)((selected_ - 1 + PATCH_SCREEN_OPTIONS) % PATCH_SCREEN_OPTIONS);
        
        // Must come before the check for PATCH_SCREEN_LIVE
        if (selected_ == PATCH_SCREEN_SWAP && !(patch_->GetVoiceMode() == VOICE_MODE_MULTI || patch_->GetVoiceMode() == VOICE_MODE_SPLIT)) {
            selected_ = PATCH_SCREEN_LIVE;
        }

        // If in live mode, we can't select live mode again. Skip the menu item.
        if (patch_->GetLiveMode() && selected_ == PATCH_SCREEN_LIVE) {
            selected_ = PATCH_SCREEN_REVERB;
        }
        
        // If a patch is loaded, we can not edit voice mode, fx, or reverb. Skip them.
        if (!patch_->GetLiveMode() && selected_ > PATCH_SCREEN_TYPE && selected_ < PATCH_SCREEN_LIVE) {
            selected_ = PATCH_SCREEN_TYPE;
        }
        
        // If the voice mode is not split mode, then skip selecction of the split note (since it is not displayed).
        if (selected_ == PATCH_SCREEN_SPLIT_NOTE && patch_->GetVoiceMode() != VOICE_MODE_SPLIT) {
            selected_ = (PatchScreenSelection)(selected_ - 1);
        }
    }
}



PatchScreenResponse PatchScreen::Click()
{
    switch (selected_) {
        case PATCH_SCREEN_NONE:
            if (!patch_->GetLiveMode()) {
                selected_ = PATCH_SCREEN_LIVE;
            } else {
                selected_ = PATCH_SCREEN_LOAD;
            }
            return PATCH_SCREEN_RESPONSE_EDIT;

        case PATCH_SCREEN_NAME:
            if (!editingName_) { // Start editing
                editingName_ = true;
                patch_->GetName(nameEditBuffer_);
                letterSelected_ = 0;
            } else if (letterSelected_ >= MAX_PATCH_NAME_LENGTH - 1) { // Done editing
                patch_->SetName(nameEditBuffer_);
                editingName_ = false;
            } else { // Next letter
                letterSelected_++;
            }
            return PATCH_SCREEN_RESPONSE_EDIT;

        case PATCH_SCREEN_TYPE:
            patch_->SetPatchType((PatchType)(patch_->GetPatchType() + 1)); // This function handles wrapping around overflow
            return PATCH_SCREEN_RESPONSE_EDIT;

        case PATCH_SCREEN_VOICES:
            patch_->SetVoiceMode((VoiceMode)(patch_->GetVoiceMode() + 1)); // This function handles wrapping around overflow
            return PATCH_SCREEN_RESPONSE_EDIT;

        case PATCH_SCREEN_SPLIT_NOTE:
            if (!editingSplitNote_) { // Start editing
                editingSplitNote_ = true;
            } else { // Next letter
                editingSplitNote_ = false;
            }
            return PATCH_SCREEN_RESPONSE_EDIT;

        case PATCH_SCREEN_FX:
            patch_->SetEffectsMode((EffectsMode)(patch_->GetEffectsMode() + 1)); // This function handles wrapping around overflow
            return PATCH_SCREEN_RESPONSE_EDIT;

        case PATCH_SCREEN_REVERB:
            patch_->SetReverbMode((ReverbMode)(patch_->GetReverbMode() + 1)); // This function handles wrapping around overflow
            return PATCH_SCREEN_RESPONSE_EDIT;

        case PATCH_SCREEN_LIVE:
            patch_->SetLiveMode(true);
            selected_ = PATCH_SCREEN_NONE;
            return PATCH_SCREEN_RESPONSE_PLAY;

        case PATCH_SCREEN_SWAP:
            patch_->SwapLayers();
            selected_ = PATCH_SCREEN_NONE;
            return PATCH_SCREEN_RESPONSE_PLAY;

        case PATCH_SCREEN_LOAD:
            selected_ = PATCH_SCREEN_NONE;
            return PATCH_SCREEN_RESPONSE_LOAD;

        case PATCH_SCREEN_SAVE:
            selected_ = PATCH_SCREEN_NONE;
            return PATCH_SCREEN_RESPONSE_SAVE;

        case PATCH_SCREEN_RETURN:
            selected_ = PATCH_SCREEN_NONE;
            return PATCH_SCREEN_RESPONSE_NOEDIT;
    }

    return PATCH_SCREEN_RESPONSE_EDIT;
}
