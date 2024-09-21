#ifndef __KIWI_SYNTH_ICONTROL_LISTENER_H__
#define __KIWI_SYNTH_ICONTROL_LISTENER_H__


namespace kiwi_synth
{
    class IControlListener
    {
        public:
            IControlListener(){}
            virtual ~IControlListener(){};

            virtual void controlMpUpdate(int controlNumber);
            virtual void controlGpioUpdate(int controlNumber);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_ICONTROL_LISTENER_H__
