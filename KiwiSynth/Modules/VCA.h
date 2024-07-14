#ifndef __KIWI_SYNTH_VCA_H__
#define __KIWI_SYNTH_VCA_H__


#include "daisysp.h"
#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    
    class VCA
    {
        private:
            bool                    noteTriggered;
            PatchSettings*          patchSettings;
            Adsr                    env;

        public:
            VCA() {}
            ~VCA() {}
            void Init(PatchSettings* patchSettings, float sampleRate);
            void Process(AudioHandle::InterleavingOutputBuffer out, size_t size);
            void NoteOn();
            void NoteOff();
            bool IsPlaying();
            bool IsReleasing();
    };
}


#endif // __KIWI_SYNTH_VCA_H__
