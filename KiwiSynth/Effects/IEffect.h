#ifndef __KIWI_SYNTH_IEFFECT_H__
#define __KIWI_SYNTH_IEFFECT_H__



namespace kiwisynth
{



/*
 * An interface that offers the Process function that all effects use. IEffects are mono. The left and right
 * channels will need to be processed separately using two instances of an effect. This gives maximum
 * flexibility for stereo effects.
 */
class IEffect
{
    public:
        IEffect(){}
        virtual ~IEffect(){};

        /*
         * Processes a single sample and returns the processed value.
         */
        virtual float Process(float in);
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_IEFFECT_H__
