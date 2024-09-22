#ifndef __KIWI_SYNTH_VOICE_BANK_H__
#define __KIWI_SYNTH_VOICE_BANK_H__


#include "daisysp.h"

#include "Voice.h"
#include "Patch/Patch.h"
#include "Modulation.h"

using namespace daisysp;

namespace kiwi_synth
{
    typedef enum {
        MODS_MOD_MATRIX_1,
        MODS_MOD_MATRIX_2,
        MODS_MOD_MATRIX_3,
        MODS_MOD_MATRIX_4,
        MODS_MOD_MATRIX_5,
        MODS_MOD_MATRIX_6,
        MODS_MOD_MATRIX_7,
        MODS_MOD_MATRIX_8,
        MODS_LFO_1_TO_VCOS,
        MODS_ENV_1_TO_VCA,
        MODS_NOTE_TO_VCF_FREQ,
        MODS_ENV_1_TO_VCF_FREQ,
        MODS_ENV_2_TO_VCF_FREQ,
        MODS_LFO_2_TO_VCF_FREQ,
        MODS_SH_TO_VCF_FREQ
    } Modulations;
    
    class VoiceBank
    {
        private:
            int8_t voiceMode;
            uint8_t maxVoices;
            std::vector<Voice> voices;
            Patch* patch;
            std::vector<uint8_t>     monoNotes;

            Voice* RequestVoice(uint8_t midiNote);
 
        public:
            std::vector<int> playingVoices;
            Modulation** modulations;
            uint8_t numVoices;

            VoiceBank() {}
            ~VoiceBank() {}
            void Init(uint8_t maxVoices, uint8_t numVcos, Patch* patch, float sampleRate);
            void Process(float* sample);

            void UpdateSettings();
            void InitModulations();
            void UpdateModulations();
            void NoteOn(uint8_t note, uint8_t velocity);
            void NoteOff(uint8_t note, uint8_t velocity);
            void AllNotesOff();
    };
}


#endif // __KIWI_SYNTH_VOICE_BANK_H__
