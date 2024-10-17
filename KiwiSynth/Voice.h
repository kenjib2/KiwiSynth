#ifndef __KIWI_SYNTH_VOICE_H__
#define __KIWI_SYNTH_VOICE_H__

#include <vector>

#include "daisysp.h"

#include "Modulation.h"
#include "Modules/VCO.h"
#include "Modules/Noise.h"
#include "Modules/SampleAndHold.h"
#include "Modules/VCF.h"
#include "Modules/VCA.h"
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
            std::vector<VCO> vcos;
            Noise       noise;
            Envelope    env1, env2;
            SampleAndHold sampleAndHold;
            LFO         lfo1, lfo2;
            VCF         vcf;
            VCA         vca;
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
            float       paraVcoMask[3]; // For paraphonic mode, tracks which VCOs are triggered.

            void initMods();
            void calculateMods(Modulation* modulations, Modulation* systemModulations);
            float getModValue(ModulationSource source, float depth);

        public:
            int maxVcos;
            int numVcos;
            int currentMidiNote;
            int triggerNote;
            int currentVelocity;
            bool noteTriggered;
            bool hardSync;

            Voice() {}
            ~Voice() {}
            void Init(int maxVcos, float sampleRate, int voiceNumber);

            void UpdateSettings(PatchSettings* patchSettings);
            void Process(float* sample, PatchSettings* patchSettings, Modulation* modulations, Modulation* systemModulations, int numVoices);
            bool IsAvailable();
            bool IsReleasing();
            void NoteOn(int note, int velocity, bool reset = true);
            void NoteOff(int note, int velocity);
            void ParaNoteOn(int vco, uint8_t note, uint8_t velocity);
            void ParaNoteOff(int vco, bool noteOff);
            void ParaAllNotesOff();
            inline void ParaphonicMode(bool isActive) {
                if (isActive) {
                    paraVcoMask[0] = 0.0f;
                    paraVcoMask[1] = 0.0f;
                    paraVcoMask[2] = 0.0f;

                    currentMidiNote = 0;
                    for (int i = 0; i < 3; i++) {
                        vcos[i].midiNote = 0;
                    }
                } else {
                    paraVcoMask[0] = 1.0f;
                    paraVcoMask[1] = 1.0f;
                    paraVcoMask[2] = 1.0f;
                    vcos[0].paraOffset = 0.0f;
                    vcos[1].paraOffset = 0.0f;
                    vcos[2].paraOffset = 0.0f;
                }
            }

    };
}


#endif // __KIWI_SYNTH_VOICE_H__
