#ifndef __KIWI_SYNTH_KIWI_SYNTH_H__
#define __KIWI_SYNTH_KIWI_SYNTH_H__

#include<vector>

#include "daisy_seed.h"

#include "../KUtils.h"
#include "Controls/MultiPots.h"
#include "Patch/Patch.h"
#include "VoiceBank.h"
#include "Effects/EffectsEngine.h"
#include "Patch/Storage.h"
#include "Patch/PatchHeader.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    static const int MIDI_LED_DURATION = 20;
    static const int MULTIPLEX_CHANNEL_DELAY = 550;

    class KiwiSynth
    {
        private:
            static const int NUM_VCOS = 3;
            static const int MAX_VOICES = 2;

            DaisySeed* hw;
            MultiPots multiPots;
            GpioExpansion ge;
            Storage storage;
            int numVoices;
            int midiChannel;
	        GPIO gpioMidiActivity;
            GPIO gpioSustain;
	        int midiCounter = 0;
            bool midiLEDState = false;
            bool midiLEDOn = false;

            MidiUartHandler midi;
            VoiceBank voiceBank;
            EffectsEngine effectsEngine;

            void ProcessMidi();
            void HandleMidiMessage(MidiEvent* midiEvent);

        public:
            Patch patch;
            PatchHeader patchBanks[NUM_PATCH_BANKS][PATCHES_PER_BANK];
            std::vector<PatchHeader*> patchTypes[PATCH_TYPE_MAX];

            KiwiSynth() {}
            ~KiwiSynth() {}
            void Init(DaisySeed* hw, float sampleRate);
            void LoadPatchBanks();

            void ProcessInputs(bool readGpio);
            void ConfigureMultiPots(DaisySeed* hw);
            void ConfigureGpioExpansion();

            void InitMidi();
            void SetMidiChannel(int midiChannel);

            bool BootLoaderRequested();
            void LoadPatch(int bankNumber, int patchNumber);
            void SavePatch(int bankNumber, int patchNumber);
            void UpdateSettings();
            void Process(AudioHandle::InterleavingOutputBuffer out, size_t size);
            void AllNotesOff();
            void TestOutput(DaisySeed* hw);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_H__
