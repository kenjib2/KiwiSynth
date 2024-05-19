#ifndef __KIWI_SYNTH_VOICE_H__
#define __KIWI_SYNTH_VOICE_H__


#include "daisysp.h"
#include "VCO.h"
#include "Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    
    class Voice
    {
        private:
            int numVCOs;
            VCO** vcos;
            PatchSettings* patchSettings;

        public:
            int currentMidiNote;

            Voice(int numVCOs, PatchSettings* patchSettings);
            ~Voice();

            void Process(AudioHandle::OutputBuffer out, size_t size);
            bool IsAvailable();
            bool IsReleasing();
    };
}


#endif // __KIWI_SYNTH_VOICE_H__
