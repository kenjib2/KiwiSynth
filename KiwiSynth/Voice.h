#ifndef __KIWI_SYNTH_VOICE_H__
#define __KIWI_SYNTH_VOICE_H__

#include <vector>

#include "daisysp.h"
#include "Modules/VCO.h"
#include "Modules/VCF.h"
#include "Modules/VCA.h"
#include "Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    
    class Voice
    {
        private:
            int numVCOs;
            std::vector<VCO> vcos;
            VCA vca;
            VCF vcf;
            PatchSettings* patchSettings;

        public:
            int currentMidiNote;

            Voice() {}
            ~Voice() {}
            void Init(int numVCOs, PatchSettings* patchSettings, float sampleRate);

            void Process(AudioHandle::InterleavingOutputBuffer out, size_t size);
            bool IsAvailable();
            bool IsReleasing();
            void NoteOn(int note, int velocity);
            void NoteOff(int note, int velocity);
    };
}


#endif // __KIWI_SYNTH_VOICE_H__
