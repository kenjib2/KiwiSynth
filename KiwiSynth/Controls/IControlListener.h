#ifndef __KIWI_SYNTH_ICONTROL_LISTENER_H__
#define __KIWI_SYNTH_ICONTROL_LISTENER_H__



namespace kiwisynth
{



/*
 * This interface is used to notify the listener when the synth controls are updated. When analog values
 * are updated, the controlMpUpdate callback will be invoked. For digital updates, controlGpioUpdate will
 * be invoked.
 */
class IControlListener
{
    public:
        IControlListener(){}
        virtual ~IControlListener(){};

        /*
         * The implementation of this function will be used for callbacks when the analog controls are updated,
         * including the multiplexers and any directly attached pots.
         */
        virtual void controlMpUpdate(int controlNumber);
        /*
         * The implementation of this function will be used for callbacks when the GPIO Expanders are updated,
         * which control all digital controls such as buttons and encoders.
         */
        virtual void controlGpioUpdate(int controlNumber);
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_ICONTROL_LISTENER_H__
