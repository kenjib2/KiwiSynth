#include "VoiceBank.h"

namespace kiwi_synth
{
    void VoiceBank::Init(uint8_t maxVoices, uint8_t numVcos, Patch* patch, float sampleRate) {
        modulations = new Modulation*[2];
        modulations[0] = new Modulation[NUM_MODULATIONS];
        modulations[1] = new Modulation[NUM_MODULATIONS];
        voiceMode = 255;
        InitModulations();
        for (int i = 0; i < maxVoices; i++) {
            voices[i].numVcos = voices[i].maxVcos;
        }

        this->patch = patch;
        this->maxVoices = maxVoices;
        this->numVoices = maxVoices;
        for (uint8_t i = 0; i < maxVoices; i++)
        {
            Voice nextVoice;
            nextVoice.Init(numVcos, sampleRate, i);
            voices.push_back(nextVoice);
        }
    }

    
    void VoiceBank::UpdateSettings()
    {
        int8_t newVoiceMode = patch->activeSettings->getIntValue(PatchSetting::VCO_VOICES);
        if (newVoiceMode != voiceMode) {
            AllNotesOff();
            voiceMode = newVoiceMode;

            if (voiceMode == VOICE_MODE_MONO) {
                numVoices = 1;
            } else {
                numVoices = 2;
            }

            patch->SetVoiceMode((VoiceMode)voiceMode);
        }
        voices[0].UpdateSettings(patch->voice1Settings);
        voices[1].UpdateSettings(patch->voice2Settings);

        UpdateModulations();
    }

    void VoiceBank::Process(float* sample)
    {
        sample[0] = 0.0f;
        sample[1] = 0.0f;

        float nextVoice[2];

        voices[0].Process(nextVoice, patch->voice1Settings, modulations[0], numVoices);
        sample[0] += nextVoice[0];
        sample[1] += nextVoice[1];

        if (voiceMode != VOICE_MODE_MONO) {
            voices[1].Process(nextVoice, patch->voice2Settings, modulations[1], numVoices);
            sample[0] += nextVoice[0];
            sample[1] += nextVoice[1];
        }
    }

    void VoiceBank::InitModulations() {
        for (int i = 0; i < 2; i++) {
            modulations[i][MODS_MOD_MATRIX_1].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_1].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_1].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_2].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_2].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_2].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_3].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_3].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_3].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_4].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_4].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_4].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_5].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_5].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_5].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_6].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_6].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_6].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_7].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_7].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_7].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_8].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_8].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_8].depth = 0.0f;

            modulations[i][MODS_LFO_1_TO_VCOS].source = SRC_LFO_1;
            modulations[i][MODS_LFO_1_TO_VCOS].destination = DST_VCOS_FREQ;
            modulations[i][MODS_LFO_1_TO_VCOS].depth = 0.0f;
            modulations[i][MODS_ENV_1_TO_VCA].source = SRC_ENV_1;
            modulations[i][MODS_ENV_1_TO_VCA].destination = DST_VCA_LEVEL;
            modulations[i][MODS_ENV_1_TO_VCA].depth = 0.0f;
            modulations[i][MODS_NOTE_TO_VCF_FREQ].source = SRC_NOTE;
            modulations[i][MODS_NOTE_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_NOTE_TO_VCF_FREQ].depth = 0.0f;
            modulations[i][MODS_ENV_1_TO_VCF_FREQ].source = SRC_ENV_1;
            modulations[i][MODS_ENV_1_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_ENV_1_TO_VCF_FREQ].depth = 0.0f;
            modulations[i][MODS_ENV_2_TO_VCF_FREQ].source = SRC_ENV_2;
            modulations[i][MODS_ENV_2_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_ENV_2_TO_VCF_FREQ].depth = 0.0f;
            modulations[i][MODS_LFO_2_TO_VCF_FREQ].source = SRC_LFO_2;
            modulations[i][MODS_LFO_2_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_LFO_2_TO_VCF_FREQ].depth = 0.0f;
            modulations[i][MODS_SH_TO_VCF_FREQ].source = SRC_SH;
            modulations[i][MODS_SH_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_SH_TO_VCF_FREQ].depth = 0.0f;
        }
    }

    void VoiceBank::UpdateModulations() {
        modulations[0][MODS_MOD_MATRIX_1].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_1_SOURCE);
        modulations[0][MODS_MOD_MATRIX_1].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_1_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_1].depth = patch->voice1Settings->getFloatValue(MOD_1_DEPTH);
        modulations[0][MODS_MOD_MATRIX_2].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_2_SOURCE);
        modulations[0][MODS_MOD_MATRIX_2].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_2_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_2].depth = patch->voice1Settings->getFloatValue(MOD_2_DEPTH);
        modulations[0][MODS_MOD_MATRIX_3].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_3_SOURCE);
        modulations[0][MODS_MOD_MATRIX_3].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_3_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_3].depth = patch->voice1Settings->getFloatValue(MOD_3_DEPTH);
        modulations[0][MODS_MOD_MATRIX_4].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_4_SOURCE);
        modulations[0][MODS_MOD_MATRIX_4].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_4_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_4].depth = patch->voice1Settings->getFloatValue(MOD_4_DEPTH);
        modulations[0][MODS_MOD_MATRIX_5].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_5_SOURCE);
        modulations[0][MODS_MOD_MATRIX_5].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_5_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_5].depth = patch->voice1Settings->getFloatValue(MOD_5_DEPTH);
        modulations[0][MODS_MOD_MATRIX_6].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_6_SOURCE);
        modulations[0][MODS_MOD_MATRIX_6].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_6_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_6].depth = patch->voice1Settings->getFloatValue(MOD_6_DEPTH);
        modulations[0][MODS_MOD_MATRIX_7].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_7_SOURCE);
        modulations[0][MODS_MOD_MATRIX_7].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_7_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_7].depth = patch->voice1Settings->getFloatValue(MOD_7_DEPTH);
        modulations[0][MODS_MOD_MATRIX_8].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_8_SOURCE);
        modulations[0][MODS_MOD_MATRIX_8].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_8_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_8].depth = patch->voice1Settings->getFloatValue(MOD_8_DEPTH);
        memcpy(modulations[1], modulations[0], sizeof(Modulations) * 8);

        modulations[0][MODS_LFO_1_TO_VCOS].depth = patch->voice1Settings->getFloatValueExponential(LFO_1_TO_MASTER_TUNE);
        modulations[0][MODS_ENV_1_TO_VCA].depth = patch->voice1Settings->getFloatValue(VCA_ENV_1_DEPTH);
        modulations[0][MODS_NOTE_TO_VCF_FREQ].depth = patch->voice1Settings->getFloatValue(VCF_TRACKING);
        modulations[0][MODS_ENV_1_TO_VCF_FREQ].depth = patch->voice1Settings->getFloatValueExponential(VCF_ENV_1_DEPTH);
        modulations[0][MODS_ENV_2_TO_VCF_FREQ].depth = patch->voice1Settings->getFloatValueExponential(VCF_ENV_2_DEPTH);
        modulations[0][MODS_LFO_2_TO_VCF_FREQ].depth = patch->voice1Settings->getFloatValueExponential(LFO_2_TO_VCF_CUTOFF);
        modulations[0][MODS_SH_TO_VCF_FREQ].depth = patch->voice1Settings->getFloatValue(SH_TO_VCF_CUTOFF);

        modulations[1][MODS_LFO_1_TO_VCOS].depth = patch->voice2Settings->getFloatValueExponential(LFO_1_TO_MASTER_TUNE);
        modulations[1][MODS_ENV_1_TO_VCA].depth = patch->voice2Settings->getFloatValue(VCA_ENV_1_DEPTH);
        modulations[1][MODS_NOTE_TO_VCF_FREQ].depth = patch->voice2Settings->getFloatValue(VCF_TRACKING);
        modulations[1][MODS_ENV_1_TO_VCF_FREQ].depth = patch->voice2Settings->getFloatValueExponential(VCF_ENV_1_DEPTH);
        modulations[1][MODS_ENV_2_TO_VCF_FREQ].depth = patch->voice2Settings->getFloatValueExponential(VCF_ENV_2_DEPTH);
        modulations[1][MODS_LFO_2_TO_VCF_FREQ].depth = patch->voice2Settings->getFloatValueExponential(LFO_2_TO_VCF_CUTOFF);
        modulations[1][MODS_SH_TO_VCF_FREQ].depth = patch->voice2Settings->getFloatValue(SH_TO_VCF_CUTOFF);
    }

    void VoiceBank::NoteOn(uint8_t note, uint8_t velocity)
    {
        switch (voiceMode) {
            case VOICE_MODE_MONO:
                voices[1].NoteOff(note, velocity); // Kill any voice 2 notes just in case
                playingNotes.push_back(note);
                if (voices[0].noteTriggered) {
                    voices[0].NoteOn(note, velocity, false);
                } else {
                    voices[0].NoteOn(note, velocity);
                }
                break;
            case VOICE_MODE_MULTI:
                playingNotes.push_back(note);
                if (voices[0].noteTriggered) {
                    voices[0].NoteOn(note, velocity, false);
                    voices[1].NoteOn(note, velocity, false);
                } else {
                    voices[0].NoteOn(note, velocity);
                    voices[1].NoteOn(note, velocity);
                }
                break;
            case VOICE_MODE_SPLIT:
                if (note < patch->splitNote) {
                    if (voices[1].noteTriggered) {
                        voices[1].NoteOn(note, velocity, false);
                    } else {
                        voices[1].NoteOn(note, velocity);
                    }
                } else {
                    playingNotes.push_back(note);
                    if (voices[0].noteTriggered) {
                        voices[0].NoteOn(note, velocity, false);
                    } else {
                        voices[0].NoteOn(note, velocity);
                    }
                }
                break;
            default:
                Voice* voice = RequestVoice(note);
                if (voice != NULL) {
                    playingNotes.push_back(note);
                    voice->NoteOn(note, velocity);
                }
                break;
        }
    }

    void VoiceBank::NoteOff(uint8_t note, uint8_t velocity)
    {
        switch (voiceMode) {
            case VOICE_MODE_MONO:
                voices[1].NoteOff(note, velocity); // Kill any voice 2 notes just in case
                if (note == playingNotes.back()) {
                    playingNotes.pop_back();
                    if (playingNotes.size() > 0) {
                        int prevNote = playingNotes.back();
                        voices[0].NoteOn(prevNote, velocity, false);
                    } else {
                        voices[0].NoteOff(note, velocity);
                    }
                } else {
                    for(unsigned int i = 0; i < playingNotes.size(); i++) {
                        if (playingNotes[i] == note) {
                            playingNotes.erase(playingNotes.begin() + i);
                            break;
                        }
                    }
                }
                break;
            case VOICE_MODE_MULTI:
                if (note == playingNotes.back()) {
                    playingNotes.pop_back();
                    if (playingNotes.size() > 0) {
                        int prevNote = playingNotes.back();
                        voices[0].NoteOn(prevNote, velocity, false);
                        voices[1].NoteOn(prevNote, velocity, false);
                    } else {
                        voices[0].NoteOff(note, velocity);
                        voices[1].NoteOff(note, velocity);
                    }
                } else {
                    for(unsigned int i = 0; i < playingNotes.size(); i++) {
                        if (playingNotes[i] == note) {
                            playingNotes.erase(playingNotes.begin() + i);
                            break;
                        }
                    }
                }
                break;
            case VOICE_MODE_SPLIT:
                if (note < patch->splitNote) {
                    if (voices[1].noteTriggered && voices[1].currentMidiNote == note) {
                        voices[1].NoteOff(note, velocity);
                        break;
                    }
                } else {
                    if (note == playingNotes.back()) {
                        playingNotes.pop_back();
                        if (playingNotes.size() > 0) {
                            int prevNote = playingNotes.back();
                            voices[0].NoteOn(prevNote, velocity, false);
                        } else {
                            voices[0].NoteOff(note, velocity);
                        }
                    } else {
                        for(unsigned int i = 0; i < playingNotes.size(); i++) {
                            if (playingNotes[i] == note) {
                                playingNotes.erase(playingNotes.begin() + i);
                                break;
                            }
                        }
                    }
                }
                break;
            default:
                int numVoicesPlaying = 0;
                int voiceFound = -1;

                // Look for a voice playing the note
                for (size_t i = 0; i < voices.size(); i++) {
                    if (!voices[i].IsAvailable()) {
                        numVoicesPlaying++;
                        if (voices[i].noteTriggered && voices[i].triggerNote == note) {
                            voiceFound = i;
                        }
                    }
                }

                if (voiceFound == -1) {
                    // We didn't find the voice. There is no note to stop.
                } else if (numVoicesPlaying == 2 && playingNotes.size() > 2) {
                    int otherNote = voices[voiceFound ? 0 : 1].triggerNote;

                    // Go backward through the vector to find the last note that isn't playing
                    for (size_t i = 0; i < playingNotes.size(); i++) {
                        if (playingNotes[i] != note && playingNotes[i] != otherNote) {
                            // We have found it. Switch to that note
                            voices[voiceFound].NoteOn(playingNotes[i], velocity, false);
                        }
                    }
                } else {
                    // There are no extra notes on the stack. Just stop the note normally.
                    voices[voiceFound] .NoteOff(note, velocity);
                }

                // Remove the note from the stack if it exists
                for(unsigned int j = 0; j < playingNotes.size(); j++) {
                    if (playingNotes[j] == note) {
                        playingNotes.erase(playingNotes.begin() + j);
                        break;
                    }
                }
                break;
        }
    }

    void VoiceBank::AllNotesOff()
    {
        for (int i = 0; i < maxVoices; i++) {
            if (!voices[i].IsAvailable()) {
                voices[i].NoteOff(0, 0);
            }
        }
        playingNotes.clear();
    }

    Voice* VoiceBank::RequestVoice(uint8_t midiNote)
    {
        uint8_t highestIndex = 0;
        uint8_t highestNote = 0;

        // First if that note is already playing, retrigger it to preserve envelope/lfo position.
        for (uint8_t i = 0; i < numVoices; i++) {
            if (!voices[i].IsAvailable() && voices[i].currentMidiNote == midiNote) {
                return &voices[i];
            }
        }

        // Else return first available voice.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsAvailable()) {
                return &voices[i];
            }
        }

        // Else return first releasing voice that is not triggered.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsReleasing() && !voices[i].noteTriggered) {
                return &voices[i];
            }
        }

        // Else return first releasing voice.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsReleasing()) {
                return &voices[i];
            } else {
                // Find the highest playing note to make sure that we return that note in the next step.
                // The lowest note will be the most audibly conspicuous note to steal so we don't want that one.
                if (voices[i].currentMidiNote > highestNote) {
                    highestIndex = i;
                    highestNote = voices[i].currentMidiNote;
                }
            }
        }

        // Return the highest note.
        return &(voices[highestIndex]); 
    }

}