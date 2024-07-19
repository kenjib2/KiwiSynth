#ifndef __KIWI_SYNTH_VOICE_H__
#define __KIWI_SYNTH_VOICE_H__

#include <vector>

#include "daisysp.h"
#include "Modules/VCO.h"
#include "Modules/Noise.h"
#include "Modules/VCF.h"
#include "Modules/VCA.h"
#include "Modules/Envelope.h"
#include "Modules/LFO.h"
#include "Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    
    class Voice
    {
        private:
            static const int MAX_MODS = 9;
            int numVcos;
            std::vector<VCO> vcos;
            Noise noise;
            Envelope env1, env2;
            LFO lfo1, lfo2;
            VCF vcf;
            VCA vca;
            PatchSettings* patchSettings;

        public:
            int currentMidiNote;
            bool noteTriggered;

            Voice() {}
            ~Voice() {}
            void Init(int numVCOs, PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void Process(float* sample);
            bool IsAvailable();
            bool IsReleasing();
            void NoteOn(int note, int velocity);
            void NoteOff(int note, int velocity);
    };
}


#endif // __KIWI_SYNTH_VOICE_H__
