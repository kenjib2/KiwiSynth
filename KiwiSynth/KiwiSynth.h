#ifndef __KIWI_SYNTH_KIWI_SYNTH_H__
#define __KIWI_SYNTH_KIWI_SYNTH_H__

#include<vector>

#include "daisy_seed.h"

#include "../KUtils.h"
#include "Controls/MultiPots.h"
#include "Patch/Patch.h"
#include "VoiceBank.h"
#include "Effects/EffectsEngine.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    static const int MIDI_LED_DURATION = 50;

    class KiwiSynth
    {
        private:
            static const int NUM_VCOS = 3;
            static const int MAX_VOICES = 2;
            DaisySeed* hw;
            MultiPots multiPots;
            GpioExpansion ge;
            int numVoices;
            int midiChannel;
	        GPIO gpioMidiActivity;
            GPIO gpioSustain;
	        int midiCounter = 0;
            bool midiLEDState = false;
            bool midiLEDOn = false;
            float balance;

            MidiUartHandler midi;
            VoiceBank voiceBank;
            EffectsEngine effectsEngine;

            void ProcessMidi();
            void HandleMidiMessage(MidiEvent* midiEvent);

        public:
            Patch patch;

            KiwiSynth() {}
            ~KiwiSynth() {}
            void Init(DaisySeed* hw, float sampleRate);

            void ProcessInputs();
            void ConfigureMultiPots(DaisySeed* hw);
            void ConfigureGpioExpansion();

            void InitMidi();
            void SetMidiChannel(int midiChannel);

            bool BootLoaderRequested();
            void UpdateSettings();
            void Process(AudioHandle::InterleavingOutputBuffer out, size_t size);
            void AllNotesOff();
            void TestOutput(DaisySeed* hw);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_H__
