#ifndef __KIWI_SYNTH_MODULATION__
#define __KIWI_SYNTH_MODULATION__

#include "Patch/PatchTypes.h"

namespace kiwisynth
{

    class Modulation {
        public:
            ModulationDestination destination;
            ModulationSource source;
            float depth;
    };

} // namespace kiwisynth

#endif // __KIWI_SYNTH_MODULATION__
