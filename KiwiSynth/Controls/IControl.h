#ifndef __KIWI_SYNTH_ICONTROL_H__
#define __KIWI_SYNTH_ICONTROL_H__


#include "IControlListener.h"

namespace kiwisynth
{
    class IControl
    {
        public:
            IControl(){}
            virtual ~IControl(){};

            /*
             * This interface is used by controller listeners so that they can register themselves
             * with a controller.
             */
            virtual void RegisterControlListener(IControlListener* controlListener);
    };
} // namespace kiwisynth


#endif // __KIWI_SYNTH_ICONTROL_H__
