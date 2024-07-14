#include "VoiceBank.h"

namespace kiwi_synth
{
    void VoiceBank::Init(int numVoices, int numVcos, PatchSettings* patchSettings, float sampleRate) {
        for (int i = 0; i < numVoices; i++)
        {
            Voice nextVoice;
            nextVoice.Init(numVcos, patchSettings, sampleRate);
            voices.push_back(nextVoice);
        }
    }


    void VoiceBank::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
    {
        // FOR NOW JUST TAKE THE OUTPUT OF THE FIRST VOICE
        voices[0].Process(out, size);
    }

    void VoiceBank::NoteOn(int note, int velocity)
    {
        voices[0].NoteOn(note, velocity);
        // Voice = RequestVoice...
        // if (voice) voice->NoteOn and add to list of playing notes...
    }

    void VoiceBank::NoteOff(int note, int velocity)
    {
        voices[0].NoteOff(note, velocity);
        /*for (size_t i = 0; i < voices.size(); i++) {
            Voice* voice = voices->FindVoice(); //if (voices[i]->noteTriggered && voices[i]->currentMidiNote == off.note) {
            if (voice) {
                voice->NoteOff(off.note, off.velocity);
                voiceBank->RemovePlayingVoice(voice);
                break;
            }
        }*/
    }

    Voice* VoiceBank::RequestVoice(int midiNote)
    {
        size_t lowestIndex = 0;
        int lowestNote = 128;
        size_t highestIndex = 0;
        int highestNote = -1;

        // First if that note is already playing, retrigger it to preserve envelope/lfo position.
        for (size_t i = 0; i < voices.size(); i++) {
            if (!voices[i].IsAvailable() && voices[i].currentMidiNote == midiNote) {
                RemovePlayingVoice(i);
                AddPlayingVoice(i, midiNote);
                return &voices[i];
            }
        }

        // Else return first available voice.
        for (size_t i = 0; i < voices.size(); i++) {
            if (voices[i].IsAvailable()) {
                AddPlayingVoice(i, midiNote);
                return &voices[i];
            }
        }

        // Else return first releasing voice.
        for (size_t i = 0; i < voices.size(); i++) {
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
        for (int i = 0; i < (int)playingIndices.size(); i++) {
            int playingIndex = playingIndices[i];
            if (playingIndex != (int)lowestIndex && playingIndex != (int)highestIndex) {
                Voice* voice = &(voices[playingIndex]);
                RemovePlayingVoice(playingIndex);
                AddPlayingVoice(playingIndex, midiNote);
                return voice;
            }
        }

        return NULL;
    }


    void VoiceBank::AddPlayingVoice(int index, int midiNote)
    {
        playingIndices.push_back(index);
        playingNotes.push_back(midiNote);
    }


    void VoiceBank::RemovePlayingVoice(int index)
    {
        for (size_t i = 0; i < playingIndices.size(); i++) {
            if (playingIndices[i] == index) {
                playingIndices.erase(playingIndices.begin()+i);
                playingNotes.erase(playingNotes.begin()+i);
                break;
            }
        }
    }

}