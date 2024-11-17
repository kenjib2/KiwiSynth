#ifndef __KIWI_SYNTH_MODULATION__
#define __KIWI_SYNTH_MODULATION__



#include "Patch/PatchTypes.h"



namespace kiwisynth
{



class Modulation {
    public:
        ModulationDestination destination_;
        ModulationSource source_;
        float depth_;
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_MODULATION__
