#ifndef __KIWI_SYNTH_ENVELOPE_H__
#define __KIWI_SYNTH_ENVELOPE_H__


#include "daisysp.h"

#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwisynth
{
    
    /*
     * The Kiwi Synth envelope module.
     */
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

            /*
             * Updates envelope parameters based on user input.
             */
            void UpdateSettings(PatchSettings* patchSettings);
            /*
             * Updates the sample by multiplying the next envelope modulation. The envelope can be directly applied to the input signal,
             * or if 1.0f is passed in each time, the result will be a modulation source that represents the envelope.
             */
            void Process(float* sample, PatchSettings* patchSettings, float attackMod, float decayMod, float sustainMod, float releaseMod);

            /*
             * Controls triggering the envelops in response to incoming notes.
             */
            inline void NoteOn()
            {
                noteTriggered = true;
                if (env.IsRunning()) {
                    env.Retrigger(false);
                }
            }
            /*
             * The envelope must be notified when a note off is received so that it can be aware of when it is no longer requested to be playing.
             */
            inline void NoteOff() { noteTriggered = false; }
            /*
             * True if the envelope is in attack, decay, sustain, or release phases.
             */
            inline bool IsPlaying() { return env.IsRunning(); }
            /*
             * True if the envelope is in release mode (typically after note off).
             */
            inline bool IsReleasing() { return env.GetCurrentSegment() == ADSR_SEG_RELEASE; }
            /*
             * Quick release is a special exception used for note stealing. When note stealing occurs, there is a risk of an audible pop if the
             * envelope is retriggered since that can cause a sudden, large, jump in level. Quick release is used to set a very short delay time
             * so that when a note needs to be retriggered, it can be faded out very quickly first before the envelope is retriggered, preventing
             * the pop artifacts.
             */
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

} // namespace kiwisynth
#endif // __KIWI_SYNTH_ENVELOPE_H__
