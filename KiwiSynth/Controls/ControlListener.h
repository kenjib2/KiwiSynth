#ifndef __KIWI_SYNTH_CONTROL_LISTENER_H__
#define __KIWI_SYNTH_CONTROL_LISTENER_H__


namespace kiwi_synth
{
    class ControlListener
    {
        public:
            ControlListener(){}
            virtual ~ControlListener(){};

            virtual void controlUpdate(int controlNumber, int controlId);
    };
} // namespace kiwi_synth


#endif // __KIWI_SYNTH_CONTROL_LISTENER_H__
