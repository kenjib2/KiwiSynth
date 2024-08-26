#ifndef __KIWI_SYNTH_VOICE_BANK_H__
#define __KIWI_SYNTH_VOICE_BANK_H__


#include "daisysp.h"
#include "Voice.h"
#include "Patch/PatchSettings.h"
#include "Modulation.h"

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
            // 0-7: mod matrix, 8: LFO1 > VCOs, 9: ENV1 > VCA, 10: NOTE > Cutoff, 11: ENV1 > Cutoff, 12: ENV2 > Cutoff, 13: LFO2 > Cutoff, 14: S&H > Cutoff
            Modulation modulations[NUM_MODULATIONS];

            VoiceBank() {}
            ~VoiceBank() {}
            void Init(uint8_t maxVoices, uint8_t numVcos, PatchSettings* patchSettings, float sampleRate);
            void Process(float* sample);

            void UpdateSettings();
            void InitModulations();
            void UpdateModulations();
            void NoteOn(uint8_t note, uint8_t velocity);
            void NoteOff(uint8_t note, uint8_t velocity);
    };
}


#endif // __KIWI_SYNTH_VOICE_BANK_H__
