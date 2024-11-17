#ifndef __KIWI_SYNTH_VOICE_H__
#define __KIWI_SYNTH_VOICE_H__



#include <vector>

#include "daisysp.h"

#include "Modulation.h"
#include "Modules/Oscillator.h"
#include "Modules/Noise.h"
#include "Modules/SampleAndHold.h"
#include "Modules/Filter.h"
#include "Modules/Amplifier.h"
#include "Modules/Envelope.h"
#include "Modules/LFO.h"
#include "Patch/PatchSettings.h"



using namespace daisysp;

namespace kiwisynth
{



class Voice
{
    private:
        const static int        NOTE_TRIGGER_SAMPLES = 72;
        constexpr static float  VOICE_ATTENTUATION_CONSTANT = 0.25f;
        const static int        MAX_MODS = 9;

        std::vector<Oscillator> oscillators_;
        Noise                   noise_;
        Envelope                env1_, env2_;
        SampleAndHold           sampleAndHold_;
        LFO                     lfo1_, lfo2_;
        Filter                  filter_;
        Amplifier               amplifier_;
        int                     voiceNumber_;
        int32_t                 noteTriggerCount_;
        bool                    isPortamentoOn_;
        float                   portamentoSpeed_;
        float                   currentPlayingNote_;
        float                   baseBalance_;
        int                     triggerVelocity_;
        bool                    isTriggerResetEnvNeeded_;
        uint8_t                 numMods_ = 0;
        float                   modValues_[NUM_MOD_DESTINATIONS];
        float                   prevSourceValues_[NUM_MOD_SOURCES];
        float                   paraOscMask_[3]; // For paraphonic mode, tracks which oscillators are triggered.

        void initMods();
        void calculateMods(Modulation* modulations, Modulation* systemModulations);
        float getModValue(ModulationSource source, float depth);

    public:
        int maxOscillators_;
        int numOscillators_;
        int currentMidiNote_;
        int triggerNote_;
        int currentVelocity_;
        bool isNoteTriggered_;
        bool isHardSyncOn_;
        bool isSampleAndHoldAvailable_;
        PmMode pmMode_;

        Voice() {}
        ~Voice() {}
        void Init(int maxOscillators, float sampleRate, int voiceNumber);

        void UpdateSettings(PatchSettings* patchSettings);
        void Process(float* sample, PatchSettings* patchSettings, Modulation* modulations, Modulation* systemModulations, int numVoices);
        bool IsAvailable();
        bool IsReleasing();
        void NoteOn(int note, int velocity, bool reset = true);
        void NoteOff(int note, int velocity);
        void ParaNoteOn(int oscillator, uint8_t note, uint8_t velocity);
        void ParaNoteOff(int oscillator, bool noteOff);
        void ParaAllNotesOff();



        inline void ParaphonicMode(bool isActive) {
            if (isActive) {
                paraOscMask_[0] = 0.0f;
                paraOscMask_[1] = 0.0f;
                paraOscMask_[2] = 0.0f;

                currentMidiNote_ = 0;
                for (int i = 0; i < 3; i++) {
                    oscillators_[i].midiNote_ = 0;
                }
            } else {
                paraOscMask_[0] = 1.0f;
                paraOscMask_[1] = 1.0f;
                paraOscMask_[2] = 1.0f;
                oscillators_[0].paraOffset_ = 0.0f;
                oscillators_[1].paraOffset_ = 0.0f;
                oscillators_[2].paraOffset_ = 0.0f;
            }
        }
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_VOICE_H__
