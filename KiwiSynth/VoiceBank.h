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
            std::vector<Voice*> voices;
            PatchSettings* patchSettings;

        public:
            VoiceBank(int numVoices, PatchSettings* patchSettings);
            ~VoiceBank();
            void Process(AudioHandle::OutputBuffer out, size_t size);

            void NoteOn(int note, int velocity);
            void NoteOff(int note, int velocity);
    };
}


#endif // __KIWI_SYNTH_VOICE_BANK_H__
