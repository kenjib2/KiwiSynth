#ifndef __KIWI_SYNTH_VOICE_BANK_H__
#define __KIWI_SYNTH_VOICE_BANK_H__


#include "daisysp.h"
#include "Voice.h"
#include "Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    
    class VoiceBank
    {
        private:
            int numVoices;
            std::vector<Voice> voices;
            PatchSettings* patchSettings;
            std::vector<int>     playingIndices;
            std::vector<int>     playingNotes;

            Voice* RequestVoice(int midiNote);
            void AddPlayingVoice(int index, int midiNote);
            void RemovePlayingVoice(int index);
 
        public:
            VoiceBank() {}
            ~VoiceBank() {}
            void Init(int numVoices, PatchSettings* patchSettings, float sampleRate);
            void Process(AudioHandle::OutputBuffer out, size_t size);

            void NoteOn(int note, int velocity);
            void NoteOff(int note, int velocity);
    };
}


#endif // __KIWI_SYNTH_VOICE_BANK_H__
