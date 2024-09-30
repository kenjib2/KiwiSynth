#include "PatchScreen.h"

namespace kiwi_synth
{

    void PatchScreen::Init(KiwiDisplay* display, Patch* patch)
    {
        this->display = display;
        this->patch = patch;
        selected = PATCH_SCREEN_NONE;
    }

    void PatchScreen::Display()
    {
        display->Fill(false);

        if (editingName) {
            display->SetCursor(0, 0);
            sprintf(buffer, "Name: ");
            display->WriteString(buffer, Font_6x8, true);
            for (int i = 0; i < MAX_PATCH_NAME_LENGTH; i++) {
                display->SetCursor(36  + i * 6, 0);
                sprintf(buffer, "%c", nameEditBuffer[i]);
                display->WriteString(buffer, Font_6x8, letterSelected != i);
            }
        } else {
            display->SetCursor(0, 0);
            patch->GetName(value);
            sprintf(buffer, "Name: %s", value);
            display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_NAME);
        }

        display->SetCursor(0, 8);
        EnumToText::GetPatchType(buffer, patch->GetPatchType());
        sprintf(buffer, "Type: %s", value);
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_TYPE);

        display->SetCursor(0, 16);
        VoiceMode voiceMode;
        if (patch->GetLiveMode()) {
            voiceMode = (VoiceMode)patch->activeSettings->getIntValue(VCO_VOICES);
        } else {
            voiceMode = (VoiceMode)patch->loadedPatchSettings.getIntValue(VCO_VOICES);
        }
        EnumToText::GetVoiceMode(value, voiceMode);
        sprintf(buffer, "Voices: %s", value);
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_VOICES);
        if (voiceMode == VOICE_MODE_SPLIT) {
            // For split voice mode, we have some extra code here to display and update the note upon which the voices are split.
            EnumToText::GetMidiNote(value, patch->GetSplitNote());
            if (editingSplitNote) {
                display->SetCursor(84, 16);
                sprintf(buffer, "on ");
                display->WriteString(buffer, Font_6x8, true);

                display->SetCursor(102, 16);
                sprintf(buffer, "%s", value);
                display->WriteString(buffer, Font_6x8, false);
            } else {
                display->SetCursor(84, 16);
                sprintf(buffer, "on %s", value);
                display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_SPLIT_NOTE);
            }
        }

        display->SetCursor(0, 24);
        EnumToText::GetEffectsMode(value, patch->GetEffectsMode());
        sprintf(buffer, "FX: %s", value);
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_FX);

        display->SetCursor(0, 32);
        EnumToText::GetReverbMode(value, patch->GetReverbMode());
        sprintf(buffer, "Reverb: %s", value);
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_REVERB);

        display->SetCursor(0, 40);
        sprintf(buffer, "Live Mode");
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_LIVE);

        display->SetCursor(0, 48);
        sprintf(buffer, "Load Patch");
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_LOAD);

        display->SetCursor(0, 56);
        sprintf(buffer, "Save Patch");
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_SAVE);

        if (selected != PATCH_SCREEN_NONE) {
            display->SetCursor(108, 56);
            sprintf(buffer, "<--");
            display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_RETURN);
        }

        display->Update();
    }

    void PatchScreen::Increment() {
        if (editingName) {
            nameEditBuffer[letterSelected] = nameEditBuffer[letterSelected] + 1;
            // Valid patch name characters are 32-126 inclusive
            if (nameEditBuffer[letterSelected] > 126) {
                nameEditBuffer[letterSelected] = 32;
            }
        } else if (editingSplitNote) {
            patch->SetSplitNote((patch->GetSplitNote() + 1) % 128);
        } else {
            selected = (PatchScreenSelection)((selected + 1) % PATCH_SCREEN_OPTIONS);

            // If in live mode, we can't select live mode again. Skip the menu item.
            if (patch->GetLiveMode() && selected == PATCH_SCREEN_LIVE) {
                selected = PATCH_SCREEN_LOAD;
            }

            // If a patch is loaded, we can not edit voice mode, fx, or reverb. Skip them.
            if (!patch->GetLiveMode() && selected > PATCH_SCREEN_TYPE && selected < PATCH_SCREEN_LIVE) {
                selected = PATCH_SCREEN_LIVE;
            }

            // If the voice mode is not split mode, then skip selecction of the split note (since it is not displayed).
            if (selected == PATCH_SCREEN_SPLIT_NOTE && patch->GetVoiceMode() != VOICE_MODE_SPLIT) {
                selected = (PatchScreenSelection)(selected + 1);
            }
        }
    }

    void PatchScreen::Decrement() {

        if (editingName) {
            nameEditBuffer[letterSelected] = nameEditBuffer[letterSelected] - 1;
            // Valid patch name characters are 32-126 inclusive
            if (nameEditBuffer[letterSelected] < 32) {
                nameEditBuffer[letterSelected] = 126;
            }
        } else if (editingSplitNote) {
            patch->SetSplitNote((patch->GetSplitNote() - 1 + 128) % 128);
        } else {
            selected = (PatchScreenSelection)((selected - 1 + PATCH_SCREEN_OPTIONS) % PATCH_SCREEN_OPTIONS);
            
            // If in live mode, we can't select live mode again. Skip the menu item.
            if (patch->GetLiveMode() && selected == PATCH_SCREEN_LIVE) {
                selected = PATCH_SCREEN_REVERB;
            }
            
            // If a patch is loaded, we can not edit voice mode, fx, or reverb. Skip them.
            if (!patch->GetLiveMode() && selected > PATCH_SCREEN_TYPE && selected < PATCH_SCREEN_LIVE) {
                selected = PATCH_SCREEN_TYPE;
            }
            
            // If the voice mode is not split mode, then skip selecction of the split note (since it is not displayed).
            if (selected == PATCH_SCREEN_SPLIT_NOTE && patch->GetVoiceMode() != VOICE_MODE_SPLIT) {
                selected = (PatchScreenSelection)(selected - 1);
            }
        }
    }

    PatchScreenResponse PatchScreen::Click()
    {
        switch (selected) {
            case PATCH_SCREEN_NONE:
                if (!patch->GetLiveMode()) {
                    selected = PATCH_SCREEN_LIVE;
                } else {
                    selected = PATCH_SCREEN_LOAD;
                }
                return PATCH_SCREEN_RESPONSE_EDIT;

            case PATCH_SCREEN_NAME:
                if (!editingName) { // Start editing
                    editingName = true;
                    patch->GetName(nameEditBuffer);
                    letterSelected = 0;
                } else if (letterSelected >= MAX_PATCH_NAME_LENGTH - 1) { // Done editing
                    patch->SetName(nameEditBuffer);
                    editingName = false;
                } else { // Next letter
                    letterSelected++;
                }
                return PATCH_SCREEN_RESPONSE_EDIT;

            case PATCH_SCREEN_TYPE:
                patch->SetPatchType((PatchType)(patch->GetPatchType() + 1)); // This function handles wrapping around overflow
                return PATCH_SCREEN_RESPONSE_EDIT;

            case PATCH_SCREEN_VOICES:
                patch->SetVoiceMode((VoiceMode)(patch->GetVoiceMode() + 1)); // This function handles wrapping around overflow
                return PATCH_SCREEN_RESPONSE_EDIT;

            case PATCH_SCREEN_SPLIT_NOTE:
                if (!editingSplitNote) { // Start editing
                    editingSplitNote = true;
                } else { // Next letter
                    editingSplitNote = false;
                }
                return PATCH_SCREEN_RESPONSE_EDIT;

            case PATCH_SCREEN_FX:
                patch->SetEffectsMode((EffectsMode)(patch->GetEffectsMode() + 1)); // This function handles wrapping around overflow
                return PATCH_SCREEN_RESPONSE_EDIT;

            case PATCH_SCREEN_REVERB:
                patch->SetReverbMode((ReverbMode)(patch->GetReverbMode() + 1)); // This function handles wrapping around overflow
                return PATCH_SCREEN_RESPONSE_EDIT;

            case PATCH_SCREEN_LIVE:
                patch->SetLiveMode(true);
                selected = PATCH_SCREEN_NONE;
                return PATCH_SCREEN_RESPONSE_PLAY;

            case PATCH_SCREEN_LOAD:
                selected = PATCH_SCREEN_NONE;
                return PATCH_SCREEN_RESPONSE_LOAD;

            case PATCH_SCREEN_SAVE:
                selected = PATCH_SCREEN_NONE;
                return PATCH_SCREEN_RESPONSE_SAVE;

            case PATCH_SCREEN_RETURN:
                selected = PATCH_SCREEN_NONE;
                return PATCH_SCREEN_RESPONSE_NOEDIT;
        }

        return PATCH_SCREEN_RESPONSE_EDIT;
    }

} // namespace kiwi_synth
