#ifndef __KIWI_SYNTH_KIWI_SYNTH_H__
#define __KIWI_SYNTH_KIWI_SYNTH_H__
//#define __DEBUG__


#include<vector>

#include "daisy_seed.h"
#include "Controls/MultiPots.h"
#include "Patch/PatchSettings.h"
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
            static const int DEFAULT_NUM_VOICES = 1;
            MultiPots multiPots;
            GpioExpansion ge;
            PatchSettings patchSettings;
            int numVoices;
            int midiChannel;
	        GPIO gpioMidiActivity;
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
            void TestOutput(DaisySeed* hw);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_H__
