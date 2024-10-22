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

namespace kiwi_synth
{
    const static float VOICE_ATTENTUATION_CONSTANT = 0.25f;
    const static int NOTE_TRIGGER_SAMPLES = 72;
    
    class Voice
    {
        private:
            static const int MAX_MODS = 9;
            std::vector<Oscillator> oscillators;
            Noise       noise;
            Envelope    env1, env2;
            SampleAndHold sampleAndHold;
            LFO         lfo1, lfo2;
            Filter      filter;
            Amplifier   amplifier;
            int         voiceNumber;
            int32_t     noteTriggerCount;
            bool        portamentoOn;
            float       portamentoSpeed;
            float       currentPlayingNote;
            float       baseBalance;
            int         triggerVelocity;
            bool        triggerResetEnv;
            float       mods[MAX_MODS];
            uint8_t     numMods = 0;
            float       modValues[NUM_MOD_DESTINATIONS];
            float       prevSourceValues[NUM_MOD_SOURCES];
            float       paraOscMask[3]; // For paraphonic mode, tracks which oscillators are triggered.

            void initMods();
            void calculateMods(Modulation* modulations, Modulation* systemModulations);
            float getModValue(ModulationSource source, float depth);

        public:
            int maxOscillators;
            int numOscillators;
            int currentMidiNote;
            int triggerNote;
            int currentVelocity;
            bool noteTriggered;
            bool hardSync;
            bool sampleAndHoldAvailable;
            PmMode pmMode;

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
                    paraOscMask[0] = 0.0f;
                    paraOscMask[1] = 0.0f;
                    paraOscMask[2] = 0.0f;

                    currentMidiNote = 0;
                    for (int i = 0; i < 3; i++) {
                        oscillators[i].midiNote = 0;
                    }
                } else {
                    paraOscMask[0] = 1.0f;
                    paraOscMask[1] = 1.0f;
                    paraOscMask[2] = 1.0f;
                    oscillators[0].paraOffset = 0.0f;
                    oscillators[1].paraOffset = 0.0f;
                    oscillators[2].paraOffset = 0.0f;
                }
            }

    };
}


#endif // __KIWI_SYNTH_VOICE_H__
