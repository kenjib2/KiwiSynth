#ifndef __KIWI_SYNTH_ENVELOPE_H__
#define __KIWI_SYNTH_ENVELOPE_H__


#include "daisysp.h"

#include "../../KUtils.h"
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
            PatchSettings*          patchSettings;
            Adsr                    env;
            uint8_t                 envNumber;

        public:
            Envelope() {}
            ~Envelope() {}
            void Init(PatchSettings* patchSettings, float sampleRate, uint8_t envNumber);
            void UpdateSettings();
            #ifdef __FUNCTIONALITY_OPTION__
            void Process(float* sample, float attackMod, float decayMod, float sustainMod, float releaseMod, bool fullFunctionality);
            #else
            void Process(float* sample, float attackMod, float decayMod, float sustainMod, float releaseMod);
            #endif // __FUNCTIONALITY_OPTION__
            void NoteOn();
            void NoteOff();
            bool IsPlaying();
            bool IsReleasing();
            void SetQuickRelease(bool quickRelease);
    };
}


#endif // __KIWI_SYNTH_ENVELOPE_H__
