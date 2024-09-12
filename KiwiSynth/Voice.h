#ifndef __KIWI_SYNTH_VOICE_H__
#define __KIWI_SYNTH_VOICE_H__

#include <vector>

#include "daisysp.h"

#include "../KUtils.h"
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
    const static float VOICE_ATTENTUATION_CONSTANT = 0.2f;
    const static int NOTE_TRIGGER_SAMPLES = 72;
    
    class Voice
    {
        private:
            static const int MAX_MODS = 9;
            std::vector<VCO> vcos;
            Noise noise;
            Envelope env1, env2;
            SampleAndHold sampleAndHold;
            LFO lfo1, lfo2;
            VCF vcf;
            VCA vca;
            PatchSettings* patchSettings;
            int voiceNumber;
            int noteTriggerCount;
            bool noteOffNeeded;
            bool portamentoOn;
            float portamentoSpeed;
            float currentPlayingNote;
            int triggerNote;
            int triggerVelocity;
            float mods[MAX_MODS];
            float vcoMods[MAX_MODS];
            uint8_t numVcoMods;
            uint8_t numMods = 0;
            float modValues[NUM_MOD_DESTINATIONS];
            float prevSourceValues[NUM_MOD_SOURCES];

            void initMods();
            void calculateMods(Modulation* modulations);
            float getModValue(ModulationSource source, float depth);

        public:
            #ifdef __FUNCTIONALITY_OPTION__
            bool fullFunctionality;
            #endif // __FUNCTIONALITY_OPTION__
            bool mono;
            int maxVcos;
            int numVcos;
            int currentMidiNote;
            int currentVelocity;
            bool noteTriggered;

            Voice() {}
            ~Voice() {}
            void Init(int maxVcos, PatchSettings* patchSettings, float sampleRate, int voiceNumber);

            void UpdateSettings(PatchSettings* patchSettings);
            void Process(float* sample, PatchSettings* patchSettings, Modulation* modulations, int numVoices);
            bool IsAvailable();
            bool IsReleasing();
            void NoteOn(int note, int velocity, bool reset = true);
            void NoteOff(int note, int velocity);
    };
}


#endif // __KIWI_SYNTH_VOICE_H__
