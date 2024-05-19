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
            MultiPots* multiPots;
            PatchSettings* patchSettings;
            int numVoices;
            int midiChannel;

            MidiUartHandler midi;
            VoiceBank* voiceBank;

            void ProcessMidi();
            void HandleMidiMessage(MidiEvent* midiEvent);

        public:
            KiwiSynth(DaisySeed* hw);
            ~KiwiSynth();

            void ConfigureMultiPots();

            void InitMidi();
            void SetMidiChannel(int midiChannel);
            
            void Process(AudioHandle::OutputBuffer out, size_t size);
            void TestOutput();
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_H__
