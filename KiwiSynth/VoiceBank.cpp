#include "VoiceBank.h"

namespace kiwi_synth
{
    void VoiceBank::Init(uint8_t maxVoices, uint8_t numVcos, PatchSettings* patchSettings, float sampleRate) {
        this->patchSettings = patchSettings;
        this->maxVoices = maxVoices;
        this->numVoices = maxVoices;
        for (uint8_t i = 0; i < maxVoices; i++)
        {
            Voice nextVoice;
            nextVoice.Init(numVcos, patchSettings, sampleRate);
            voices.push_back(nextVoice);
        }
        InitModulations();
    }

    
    void VoiceBank::UpdateSettings()
    {
        for (int i = 0; i < maxVoices; i++) {
            voices[i].UpdateSettings();
        }

        switch (patchSettings->getIntValue(PatchSetting::VCO_VOICES))
        {
            case 0:
            default:
                for (int i = 0; i < maxVoices; i++) {
                    voices[i].fullFunctionality = true;
                    voices[i].mono = false;
                    voices[i].numVcos = voices[i].maxVcos;
                }
                numVoices = 2;
                break;
            case 1:
                for (int i = 0; i < maxVoices; i++) {
                    voices[i].fullFunctionality = false;
                    voices[i].mono = false;
                    voices[i].numVcos = 1;
                }
                numVoices = maxVoices;
                break;
            case 2:
                for (int i = 0; i < maxVoices; i++) {
                    voices[i].fullFunctionality = true;
                    voices[i].mono = true;
                    voices[i].numVcos = voices[i].maxVcos;
                }
                numVoices = 1;
                break;
        }

        UpdateModulations();
    }

    void VoiceBank::Process(float* sample)
    {
        float nextVoice = 0.0f;
        *sample = 0.0f;

        for (int i = 0; i < numVoices; i++) {
            voices[i].Process(&nextVoice, modulations);
            if (i < numVoices) {
                *sample += nextVoice;
            }
        }
    }

    void VoiceBank::InitModulations() {
        if (numVoices != maxVoices) {
            for (int i = 0; i < 8; i++) {
                modulations[i].source = SRC_NONE;
                modulations[i].destination = DST_NONE;
                modulations[i].depth = 0.0f;
            }
        }
        modulations[MODS_LFO_1_TO_VCOS].source = SRC_LFO_1;
        modulations[MODS_LFO_1_TO_VCOS].destination = DST_VCOS_FREQ;
        modulations[MODS_LFO_1_TO_VCOS].depth = 0.0f;
        modulations[MODS_ENV_1_TO_VCA].source = SRC_ENV_1;
        modulations[MODS_ENV_1_TO_VCA].destination = DST_VCA_LEVEL;
        modulations[MODS_ENV_1_TO_VCA].depth = 0.0f;
        modulations[MODS_NOTE_TO_VCF_FREQ].source = SRC_NOTE;
        modulations[MODS_NOTE_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
        modulations[MODS_NOTE_TO_VCF_FREQ].depth = 0.0f;
        modulations[MODS_ENV_1_TO_VCF_FREQ].source = SRC_ENV_1;
        modulations[MODS_ENV_1_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
        modulations[MODS_ENV_1_TO_VCF_FREQ].depth = 0.0f;
        modulations[MODS_ENV_2_TO_VCF_FREQ].source = SRC_ENV_2;
        modulations[MODS_ENV_2_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
        modulations[MODS_ENV_2_TO_VCF_FREQ].depth = 0.0f;
        modulations[MODS_LFO_2_TO_VCF_FREQ].source = SRC_LFO_2;
        modulations[MODS_LFO_2_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
        modulations[MODS_LFO_2_TO_VCF_FREQ].depth = 0.0f;
        modulations[MODS_SH_TO_VCF_FREQ].source = SRC_SH;
        modulations[MODS_SH_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
        modulations[MODS_SH_TO_VCF_FREQ].depth = 0.0f;
    }

    void VoiceBank::UpdateModulations() {
        for (int i = 0; i < 8; i++) {
            modulations[i].source = (ModulationSource)patchSettings->getIntValue((PatchSetting)(MOD_1_SOURCE + i * 3));
            modulations[i].destination = (ModulationDestination)patchSettings->getIntValue((PatchSetting)(MOD_1_DESTINATION + i * 3));
            modulations[i].depth = patchSettings->getFloatValue((PatchSetting)(MOD_1_DEPTH + i * 3));
        }
        modulations[MODS_LFO_1_TO_VCOS].depth = patchSettings->getFloatValue(LFO_1_TO_MASTER_TUNE);
        modulations[MODS_ENV_1_TO_VCA].depth = patchSettings->getFloatValue(VCA_ENV_1_DEPTH);
        modulations[MODS_NOTE_TO_VCF_FREQ].depth = patchSettings->getFloatValue(VCF_TRACKING);
        modulations[MODS_ENV_1_TO_VCF_FREQ].depth = patchSettings->getFloatValue(VCF_ENV_1_DEPTH, 0.0f, 1.0f, EXPONENTIAL);
        modulations[MODS_ENV_2_TO_VCF_FREQ].depth = patchSettings->getFloatValue(VCF_ENV_2_DEPTH, 0.0f, 1.0f, EXPONENTIAL);
        modulations[MODS_LFO_2_TO_VCF_FREQ].depth = patchSettings->getFloatValue(LFO_2_TO_VCF_CUTOFF, 0.0f, 1.0f, EXPONENTIAL);
        modulations[MODS_SH_TO_VCF_FREQ].depth = patchSettings->getFloatValue(SH_TO_VCF_CUTOFF);
    }

    void VoiceBank::NoteOn(uint8_t note, uint8_t velocity)
    {
        if (numVoices == 1) {
            monoNotes.push_back(note);
            if (voices[0].noteTriggered) {
                voices[0].NoteOn(note, velocity, false);
            } else {
                voices[0].NoteOn(note, velocity);
            }
        } else {
            Voice* voice = RequestVoice(note);
            if (voice != NULL) {
                voice->NoteOn(note, velocity);
            }
        }
    }

    void VoiceBank::NoteOff(uint8_t note, uint8_t velocity)
    {
        if (numVoices == 1) {
            if (note == monoNotes.back()) {
                monoNotes.pop_back();
                if (monoNotes.size() > 0) {
                    int prevNote = monoNotes.back();
                    voices[0].NoteOn(prevNote, velocity, false);
                } else {
                    voices[0].NoteOff(note, velocity);
                }
            } else {
                for(unsigned int i = 0; i < monoNotes.size(); i++) {
                    if (monoNotes[i] == note) {
                        monoNotes.erase(monoNotes.begin() + i);
                        break;
                    }
                }
            }
        } else {
            for (size_t i = 0; i < voices.size(); i++) {
                if (voices[i].noteTriggered && voices[i].currentMidiNote == note) {
                    voices[i].NoteOff(note, velocity);
                    RemovePlayingVoice(i);
                    break;
                }
            }
        }
    }

    Voice* VoiceBank::RequestVoice(uint8_t midiNote)
    {
        uint8_t lowestIndex = 0;
        uint8_t lowestNote = 128;
        uint8_t highestIndex = 0;
        uint8_t highestNote = 0;

        // First if that note is already playing, retrigger it to preserve envelope/lfo position.
        for (uint8_t i = 0; i < numVoices; i++) {
            if (!voices[i].IsAvailable() && voices[i].currentMidiNote == midiNote) {
                RemovePlayingVoice(i);
                AddPlayingVoice(i, midiNote);
                return &voices[i];
            }
        }

        // Else return first available voice.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsAvailable()) {
                AddPlayingVoice(i, midiNote);
                return &voices[i];
            }
        }

        // Else return first releasing voice.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsReleasing()) {
                RemovePlayingVoice(i);
                AddPlayingVoice(i, midiNote);
                return &voices[i];
            } else {
                // Find the lowest and highest playing notes to make sure that we don't return them in the next step.
                // These will be the most audibly conspicuous notes to steal.
                if (voices[i].currentMidiNote < lowestNote) {
                    lowestIndex = i;
                    lowestNote = voices[i].currentMidiNote;
                }
                if (voices[i].currentMidiNote > highestNote) {
                    highestIndex = i;
                    highestNote = voices[i].currentMidiNote;
                }
            }
        }

        // If no voices are available, return the oldest note.
        // We have to replace a playing note that is neither lowest nor highest.
        for (uint8_t i = 0; i < numVoices; i++) {
            uint8_t playingIndex = playingIndices[i];
            if (playingIndex != lowestIndex && playingIndex != highestIndex) {
                RemovePlayingVoice(playingIndex);
                AddPlayingVoice(playingIndex, midiNote);
                return &(voices[playingIndex]);
            }
        }

        // If we get here it means we have 1 or 2 voice polyphony. Return the highest note.
        RemovePlayingVoice(highestIndex);
        AddPlayingVoice(highestIndex, midiNote);
        return &(voices[highestIndex]); 
    }


    void VoiceBank::AddPlayingVoice(uint8_t index, uint8_t midiNote)
    {
        playingIndices.push_back(index);
        playingNotes.push_back(midiNote);
    }


    void VoiceBank::RemovePlayingVoice(uint8_t index)
    {
        for (uint8_t i = 0; i < playingIndices.size(); i++) {
            if (playingIndices[i] == index) {
                playingIndices.erase(playingIndices.begin()+i);
                playingNotes.erase(playingNotes.begin()+i);
                break;
            }
        }
    }

}