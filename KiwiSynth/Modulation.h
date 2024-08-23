#ifndef __KIWI_SYNTH_MODULATION__
#define __KIWI_SYNTH_MODULATION__

#include "Patch/PatchSettings.h"

namespace kiwi_synth
{
    class Modulation {
        public:
            ModulationDestination destination;
            ModulationSource source;
            float depth;
    };
}

#endif // __KIWI_SYNTH_MODULATION__
