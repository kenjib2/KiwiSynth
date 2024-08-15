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
                numVoices = maxVoices;
                break;
            case 1:
                numVoices = 1;
                break;
        }
    }

    void VoiceBank::Process(float* sample)
    {
        float nextVoice = 0.0f;
        *sample = 0.0f;

        for (int i = 0; i < maxVoices; i++) {
            voices[i].Process(&nextVoice);
            if (i < numVoices) {
                *sample += nextVoice;
            }
        }
    }

    void VoiceBank::NoteOn(uint8_t note, uint8_t velocity)
    {
        Voice* voice = RequestVoice(note);
        if (voice != NULL) {
            voice->NoteOn(note, velocity);
        }
    }

    void VoiceBank::NoteOff(uint8_t note, uint8_t velocity)
    {
        for (size_t i = 0; i < voices.size(); i++) {
            if (voices[i].noteTriggered && voices[i].currentMidiNote == note) {
                voices[i].NoteOff(note, velocity);
                RemovePlayingVoice(i);
                break;
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

    std::vector<uint8_t>* VoiceBank::getPlayingNotes()
    {
        return &playingIndices;
    }

}