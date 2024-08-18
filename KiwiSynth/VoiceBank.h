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
            uint8_t maxVoices;
            uint8_t numVoices;
            std::vector<Voice> voices;
            PatchSettings* patchSettings;
            std::vector<uint8_t>     playingIndices;
            std::vector<uint8_t>     playingNotes;
            std::vector<uint8_t>     monoNotes;

            Voice* RequestVoice(uint8_t midiNote);
            void AddPlayingVoice(uint8_t index, uint8_t midiNote);
            void RemovePlayingVoice(uint8_t index);
 
        public:
            std::vector<int> playingVoices;

            VoiceBank() {}
            ~VoiceBank() {}
            void Init(uint8_t maxVoices, uint8_t numVcos, PatchSettings* patchSettings, float sampleRate);
            void Process(float* sample);

            void UpdateSettings();
            void NoteOn(uint8_t note, uint8_t velocity);
            void NoteOff(uint8_t note, uint8_t velocity);
    };
}


#endif // __KIWI_SYNTH_VOICE_BANK_H__
