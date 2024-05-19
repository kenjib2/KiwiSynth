#include "VoiceBank.h"

namespace kiwi_synth
{
    VoiceBank::VoiceBank(int numVoices, PatchSettings* patchSettings) : numVoices(numVoices), patchSettings(patchSettings)
    {
        for (int i = 0; i < numVoices; i++)
        {
            Voice* nextVoice = new Voice(numVoices, patchSettings);
            voices.push_back(nextVoice);
        }
    }

    VoiceBank::~VoiceBank()
    {
        for (const Voice* nextVoice : voices) {
            delete nextVoice;
        }
    }

    void VoiceBank::Process(AudioHandle::OutputBuffer out, size_t size)
    {
        // FOR NOW JUST TAKE THE OUTPUT OF THE FIRST VOICE
        voices[0]->Process(out, size);
    }

    void VoiceBank::NoteOn(int note, int velocity)
    {
        // Voice = RequestVoice...
        // if (voice) voice->NoteOn and add to list of playing notes...
    }

    void VoiceBank::NoteOff(int note, int velocity)
    {
        /*for (size_t i = 0; i < voices.size(); i++) {
            Voice* voice = voices->FindVoice(); //if (voices[i]->noteTriggered && voices[i]->currentMidiNote == off.note) {
            if (voice) {
                voice->NoteOff(off.note, off.velocity);
                voiceBank->RemovePlayingVoice(voice);
                break;
            }
        }*/
    }
}