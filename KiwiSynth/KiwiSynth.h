#ifndef __KIWI_SYNTH_KIWI_SYNTH_H__
#define __KIWI_SYNTH_KIWI_SYNTH_H__


#include "daisy_seed.h"
#include "Controls/MultiPots.h"
#include "Patch/PatchSettings.h"
#include "Voice.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    class KiwiSynth
    {
        private:
            static const int NUM_VCOS = 3;
            static const int NUM_VOICES = 4;
            DaisySeed* hw;
            MultiPots* multiPots;
            PatchSettings* patchSettings;

            Voice* voices[NUM_VOICES];

        public:
            KiwiSynth(DaisySeed* hw);
            ~KiwiSynth();

            void ConfigureMultiPots();
            void Process(AudioHandle::OutputBuffer out, size_t size);
            void TestOutput();
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_H__
