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
            bool                    isReversed_;
            bool                    isQuickRelease_;
            float                   releaseValue_;
            float                   prevAttackMod_;
            float                   prevDecayMod_;
            float                   prevSustainMod_;
            float                   prevReleaseMod_;
            Adsr                    env_;
            uint8_t                 envNumber_;

        public:
            bool                    isNoteTriggered_;

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
                isNoteTriggered_ = true;
                if (env_.IsRunning()) {
                    env_.Retrigger(false);
                }
            }



            /*
             * The envelope must be notified when a note off is received so that it can be aware of when it is no longer requested to be playing.
             */
            inline void NoteOff() { isNoteTriggered_ = false; }



            /*
             * True if the envelope is in attack, decay, sustain, or release phases.
             */
            inline bool IsPlaying() { return env_.IsRunning(); }



            /*
             * True if the envelope is in release mode (typically after note off).
             */
            inline bool IsReleasing() { return env_.GetCurrentSegment() == ADSR_SEG_RELEASE; }



            /*
             * Quick release is a special exception used for note stealing. When note stealing occurs, there is a risk of an audible pop if the
             * envelope is retriggered since that can cause a sudden, large, jump in level. Quick release is used to set a very short delay time
             * so that when a note needs to be retriggered, it can be faded out very quickly first before the envelope is retriggered, preventing
             * the pop artifacts.
             */
            inline void SetQuickRelease(bool quickRelease)
            {
                isQuickRelease_ = quickRelease;
                if (quickRelease) {
                    env_.SetReleaseTime(0.0015f);
                } else {
                    env_.SetReleaseTime(releaseValue_);
                }
            }
    };



} // namespace kiwisynth
#endif // __KIWI_SYNTH_ENVELOPE_H__
