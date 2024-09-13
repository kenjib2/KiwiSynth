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
            bool                    reversed;
            bool                    quickRelease;
            float                   releaseValue;
            float                   prevAttackMod;
            float                   prevDecayMod;
            float                   prevSustainMod;
            float                   prevReleaseMod;
            Adsr                    env;
            uint8_t                 envNumber;

        public:
            Envelope() {}
            ~Envelope() {}
            void Init(float sampleRate, uint8_t envNumber);
            void UpdateSettings(PatchSettings* patchSettings);
            void Process(float* sample, PatchSettings* patchSettings, float attackMod, float decayMod, float sustainMod, float releaseMod);
            void NoteOn();
            void NoteOff();
            bool IsPlaying();
            bool IsReleasing();
            void SetQuickRelease(bool quickRelease);
    };
}


#endif // __KIWI_SYNTH_ENVELOPE_H__
