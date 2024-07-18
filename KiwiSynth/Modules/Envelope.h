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
            uint8_t                 envNumber;

        public:
            Envelope() {}
            ~Envelope() {}
            void Init(PatchSettings* patchSettings, float sampleRate, uint8_t envNumber);
            void UpdateSettings();
            void Process(float* sample);
            void NoteOn(bool retrigger);
            void NoteOff();
            bool IsPlaying();
            bool IsReleasing();
    };
}


#endif // __KIWI_SYNTH_ENVELOPE_H__
