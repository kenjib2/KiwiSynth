#ifndef __KIWI_SYNTH_VOICE_H__
#define __KIWI_SYNTH_VOICE_H__

#include <vector>

#include "daisysp.h"
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
    const static float VOICE_ATTENTUATION_CONSTANT = 0.3f;
    const static int NOTE_TRIGGER_SAMPLES = 72;
    const static int MOD_SOURCES = 4;
    const static int MOD_DESTINATIONS = 4;
    
    class Voice
    {
        private:
            static const int MAX_MODS = 9;
            int numVcos;
            std::vector<VCO> vcos;
            Noise noise;
            Envelope env1, env2;
            SampleAndHold sampleAndHold;
            LFO lfo1, lfo2;
            VCF vcf;
            VCA vca;
            PatchSettings* patchSettings;
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
            float modMatrix[MOD_SOURCES][MOD_DESTINATIONS];

        public:
            bool noiseAndSHOn;
            int currentMidiNote;
            bool noteTriggered;

            Voice() {}
            ~Voice() {}
            void Init(int numVCOs, PatchSettings* patchSettings, float sampleRate);

            void UpdateSettings();
            void Process(float* sample);
            float GetMatrixMods(int destination);
            bool IsAvailable();
            bool IsReleasing();
            void NoteOn(int note, int velocity, bool reset = true);
            void NoteOff(int note, int velocity);
    };
}


#endif // __KIWI_SYNTH_VOICE_H__
