#ifndef __KIWI_SYNTH_CONTROL_H__
#define __KIWI_SYNTH_CONTROL_H__


#include "ControlListener.h"

namespace kiwi_synth
{
    class Control
    {
        public:
            Control(){}
            virtual ~Control(){};

            virtual void RegisterControlListener(ControlListener* controlListener);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_CONTROL_H__
