#include "PatchScreen.h"
#include "IntValueScreen.h"

namespace kiwi_synth
{

    void PatchScreen::Init(KiwiDisplay* display, KiwiSynth* kiwiSynth, Patch* patch)
    {
        this->display = display;
        this->kiwiSynth = kiwiSynth;
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
        GetPatchType(value);
        sprintf(buffer, "Type: %s", value);
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_TYPE);

        display->SetCursor(0, 16);
        GetVoiceMode(value);
        sprintf(buffer, "Voices: %s", value);
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_VOICES);
        if (patch->activeSettings->getIntValue(VCO_VOICES) == VOICE_MODE_SPLIT) {
            GetMidiNote(value);
            display->SetCursor(84, 16);
            sprintf(buffer, "on %s", value);
            display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_SPLIT_NOTE);
        }

        display->SetCursor(0, 24);
        GetFxType(value);
        sprintf(buffer, "FX: %s", value);
        display->WriteString(buffer, Font_6x8, selected != PATCH_SCREEN_FX);

        display->SetCursor(0, 32);
        GetReverbType(value);
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
            if (nameEditBuffer[letterSelected] > 126) {
                nameEditBuffer[letterSelected] = 32;
            }
        } else {
            selected = (PatchScreenSelection)((selected + 1) % PATCH_SCREEN_OPTIONS);

            // Valid patch name characters are 32-126 inclusive
            if (patch->GetLiveMode() && selected == PATCH_SCREEN_LIVE) {
                selected = PATCH_SCREEN_LOAD;
            }

            if (!patch->GetLiveMode() && selected > PATCH_SCREEN_TYPE && selected < PATCH_SCREEN_LIVE) {
                selected = PATCH_SCREEN_LIVE;
            }
            
            if (selected == PATCH_SCREEN_SPLIT_NOTE && patch->GetVoiceMode() != VOICE_MODE_SPLIT) {
                selected = (PatchScreenSelection)(selected + 1);
            }
        }
    }

    void PatchScreen::Decrement() {

        if (editingName) {
            nameEditBuffer[letterSelected] = nameEditBuffer[letterSelected] - 1;
            if (nameEditBuffer[letterSelected] < 32) {
                nameEditBuffer[letterSelected] = 126;
            }
        } else {
            selected = (PatchScreenSelection)((selected - 1 + PATCH_SCREEN_OPTIONS) % PATCH_SCREEN_OPTIONS);
            
            if (patch->GetLiveMode() && selected == PATCH_SCREEN_LIVE) {
                selected = PATCH_SCREEN_REVERB;
            }
            
            if (!patch->GetLiveMode() && selected > PATCH_SCREEN_TYPE && selected < PATCH_SCREEN_LIVE) {
                selected = PATCH_SCREEN_TYPE;
            }
            
            if (selected == PATCH_SCREEN_SPLIT_NOTE && patch->GetVoiceMode() != VOICE_MODE_SPLIT) {
                selected = (PatchScreenSelection)(selected - 1);
            }
        }
    }

    /*
     * Returns whether or not the menu should remain active.
     */
    bool PatchScreen::Click()
    {
        switch (selected) {
            case PATCH_SCREEN_NONE:
                if (!patch->GetLiveMode()) {
                    selected = PATCH_SCREEN_LIVE;
                } else {
                    selected = PATCH_SCREEN_LOAD;
                }
                return true;
            case PATCH_SCREEN_SPLIT_NOTE:
            case PATCH_SCREEN_LOAD:
            case PATCH_SCREEN_SAVE:
                // TO DO
                return true;
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
                return true;
            case PATCH_SCREEN_TYPE:
                patch->SetPatchType((PatchType)(patch->GetPatchType() + 1));
                return true;
            case PATCH_SCREEN_VOICES:
                patch->SetVoiceMode((VoiceMode)(patch->GetVoiceMode() + 1));
                return true;
            case PATCH_SCREEN_FX:
                patch->SetEffectsMode((EffectsMode)(patch->GetEffectsMode() + 1));
                return true;
            case PATCH_SCREEN_REVERB:
                patch->SetReverbMode((ReverbMode)(patch->GetReverbMode() + 1));
                return true;
            case PATCH_SCREEN_LIVE:
                patch->SetLiveMode(true);
                selected = PATCH_SCREEN_NONE;
                return false;
            case PATCH_SCREEN_RETURN:
                selected = PATCH_SCREEN_NONE;
                return false;
        }

        return true;
    }

    // 13 character value
    void PatchScreen::GetPatchType(char* buffer)
    {
        switch (patch->GetPatchType()) {
            case PATCH_ARP:
                strcpy(buffer, "Arpeggiated");
                break;
            case PATCH_BASS:
                strcpy(buffer, "Bass");
                break;
            case PATCH_BRASS:
                strcpy(buffer, "Brass");
                break;
            case PATCH_DRONE:
                strcpy(buffer, "Drone");
                break;
            case PATCH_EFFECT:
                strcpy(buffer, "Effect");
                break;
            case PATCH_KEY:
                strcpy(buffer, "Key");
                break;
            case PATCH_LEAD:
                strcpy(buffer, "Lead");
                break;
            case PATCH_OTHER:
                strcpy(buffer, "Other");
                break;
            case PATCH_PAD:
                strcpy(buffer, "Pad");
                break;
            case PATCH_PERCUSSION:
                strcpy(buffer, "Percussion");
                break;
            case PATCH_PLUCK:
                strcpy(buffer, "Pluck");
                break;
            case PATCH_STRING:
                strcpy(buffer, "String");
                break;
            case PATCH_SYNTH:
                strcpy(buffer, "Synth");
                break;
        }
    }

    // 11 character value
    void PatchScreen::GetVoiceMode(char* buffer)
    {
        switch (patch->activeSettings->getIntValue(VCO_VOICES)) {
            case VOICE_MODE_POLY:
                strcpy(buffer, "Polyphonic");
                break;
            case VOICE_MODE_MONO:
                strcpy(buffer, "Monophonic");
                break;
            case VOICE_MODE_MULTI:
                strcpy(buffer, "Layered");
                break;
            case VOICE_MODE_SPLIT:
                strcpy(buffer, "Split");
                break;
        }
    }

    // 3 character value
    void PatchScreen::GetMidiNote(char* buffer)
    {
        char notes[12][3] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
        int noteNum = patch->GetSplitNote();
        int octave = noteNum / 12 - 1;
        char* note = notes[noteNum % 12];
        sprintf(buffer, "%s%d", note, octave);
    }

    // 17 character value
    void PatchScreen::GetFxType(char* buffer) {
        switch (patch->GetEffectsMode()) {
            case FX_DISTORTION_DELAY:
                strcpy(buffer, "Distortion-Delay");
                break;
            case FX_CHORUS_DELAY:
                strcpy(buffer, "Chorus-Delay");
                break;
            case FX_PHASER_DELAY:
                strcpy(buffer, "Phaser-Delay");
                break;
            case FX_FLANGER_DELAY:
                strcpy(buffer, "Flanger-Delay");
                break;
            case FX_DISTORTION_BITCRUSH:
                strcpy(buffer, "Distort-Bitcrush");
                break;
        }
    }

    // 13 character value
    void PatchScreen::GetReverbType(char* buffer) {
        switch (patch->GetReverbMode()) {
            case REVERB_ROOM:
                strcpy(buffer, "Room");
                break;
            case REVERB_HALL:
                strcpy(buffer, "Hall");
                break;
            case REVERB_CHAMBER:
                strcpy(buffer, "Chamber");
                break;
            case REVERB_CATHEDRAL:
                strcpy(buffer, "Cathedral");
                break;
            case REVERB_BLOOM:
                strcpy(buffer, "Bloom");
                break;
            case REVERB_BRIGHT:
                strcpy(buffer, "Bright");
                break;
        }
    }

} // namespace kiwi_synth
