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
            bool                    noteTriggered;

            Envelope() {}
            ~Envelope() {}
            void Init(float sampleRate, uint8_t envNumber);

            void UpdateSettings(PatchSettings* patchSettings);
            void Process(float* sample, PatchSettings* patchSettings, float attackMod, float decayMod, float sustainMod, float releaseMod);

            inline void NoteOn()
            {
                noteTriggered = true;
                if (env.IsRunning()) {
                    env.Retrigger(false);
                }
            }
            inline void NoteOff() { noteTriggered = false; }
            inline bool IsPlaying() { return env.IsRunning(); }
            inline bool IsReleasing() { return env.GetCurrentSegment() == ADSR_SEG_RELEASE; }
            inline void SetQuickRelease(bool quickRelease)
            {
                this->quickRelease = quickRelease;
                if (quickRelease) {
                    env.SetReleaseTime(0.0015f);
                } else {
                    env.SetReleaseTime(releaseValue);
                }
            }
    };
}


#endif // __KIWI_SYNTH_ENVELOPE_H__
