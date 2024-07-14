#ifndef __KIWI_SYNTH_ENVELOPE_H__
#define __KIWI_SYNTH_ENVELOPE_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    
    class Envelope
    {
        private:
            bool                    noteTriggered;
            PatchSettings*          patchSettings;
            Adsr                    env;

        public:
            Envelope() {}
            ~Envelope() {}
            void Init(PatchSettings* patchSettings, float sampleRate);
            void Process(AudioHandle::InterleavingOutputBuffer out, size_t size);
            void NoteOn();
            void NoteOff();
            bool IsPlaying();
            bool IsReleasing();
    };
}


#endif // __KIWI_SYNTH_ENVELOPE_H__
