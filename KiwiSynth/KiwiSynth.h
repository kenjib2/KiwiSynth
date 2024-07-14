#ifndef __KIWI_SYNTH_KIWI_SYNTH_H__
#define __KIWI_SYNTH_KIWI_SYNTH_H__


#include<vector>

#include "daisy_seed.h"
#include "Controls/MultiPots.h"
#include "Patch/PatchSettings.h"
#include "VoiceBank.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    class KiwiSynth
    {
        private:
            static const int NUM_VCOS = 3;
            static const int DEFAULT_NUM_VOICES = 4;
            DaisySeed* hw;
            MultiPots multiPots;
            GpioExpansion ge;
            PatchSettings patchSettings;
            int numVoices;
            int midiChannel;

            MidiUartHandler midi;
            VoiceBank voiceBank;

            void ProcessMidi();
            void HandleMidiMessage(MidiEvent* midiEvent);

        public:
            KiwiSynth() {}
            ~KiwiSynth() {}
            void Init(DaisySeed* hw, float sampleRate);

            void ProcessInputs();
            void ConfigureMultiPots();
            void ConfigureGpioExpansion();

            void InitMidi();
            void SetMidiChannel(int midiChannel);

            bool BootLoaderRequested();
            void Process(AudioHandle::InterleavingOutputBuffer out, size_t size);
            void TestOutput();
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_H__
