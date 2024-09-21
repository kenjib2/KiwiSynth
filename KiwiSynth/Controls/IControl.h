#ifndef __KIWI_SYNTH_ICONTROL_H__
#define __KIWI_SYNTH_ICONTROL_H__


#include "IControlListener.h"

namespace kiwi_synth
{
    class IControl
    {
        public:
            IControl(){}
            virtual ~IControl(){};

            virtual void RegisterControlListener(IControlListener* controlListener);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_ICONTROL_H__
