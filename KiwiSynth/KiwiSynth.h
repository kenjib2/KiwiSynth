#ifndef __KIWI_SYNTH_H__
#define __KIWI_SYNTH_H__


#include "daisy_seed.h"
#include "Controls/MultiPots.h"
#include "Patch/PatchSettings.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    class KiwiSynth
    {
        private:
            DaisySeed* hw;
            MultiPots* multiPots;
            PatchSettings* patchSettings;

        public:
            KiwiSynth(DaisySeed* hw);

            void ConfigureMultiPots();
            void Process();
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_H__
