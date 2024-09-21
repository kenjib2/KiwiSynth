#ifndef __KIWI_SYNTH_IEFFECT_H__
#define __KIWI_SYNTH_IEFFECT_H__

namespace kiwi_synth
{
    class IEffect
    {
        public:
            IEffect(){}
            virtual ~IEffect(){};

            virtual float Process(float in);
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_IEFFECT_H__
