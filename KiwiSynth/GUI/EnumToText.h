#ifndef __KIWI_SYNTH_ENUM_TO_TEXT_H__
#define __KIWI_SYNTH_ENUM_TO_TEXT_H__



#include "daisy_seed.h"
#include "../Patch/PatchTypes.h"



namespace kiwisynth
{



class EnumToText
{
    public:
        /*
         * Buffer should be at least 5 in size.
         */
        static void GetWaveform(char* buffer, Waveform waveform);
        /*
         * Buffer should be at least 3 in size.
         */
        static void GetWaveformTwoChar(char* buffer, Waveform waveform);
        /*
         * Buffer should be at least 3 in size.
         */
        static void GetNoiseType(char* buffer, NoiseType noiseType);
        /*
         * Buffer should be at least 5 in size.
         */
        static void GetLfoWaveform(char* buffer, LfoWaveform lfoWaveform);
        /*
         * Buffer should be at least 3 in size.
         */
        static void GetLfoWaveformTwoChar(char* buffer, LfoWaveform lfoWaveform);
        /*
         * Buffer should be at least 9 in size.
         */
        static void GetFilterType(char* buffer, FilterType filterType);
        /*
         * Buffer should be at least 6 in size.
         */
        static void GetModSource(char* buffer, ModulationSource modSource);
        /*
         * Buffer should be at least 6 in size.
         */
        static void GetModDestination(char* buffer, ModulationDestination modDestination);
        /*
         * Buffer should be at least 13 in size.
         */
        static void GetPatchType(char* buffer, PatchType patchType);
        /*
         * Buffer should be at least 14 in size.
         */
        static void GetVoiceMode(char* buffer, VoiceMode voiceMode);
        /*
         * Buffer should be at least 3 in size.
         */
        static void GetMidiNote(char* buffer, uint8_t midiNote);
        /*
         * Buffer should be at least 17 in size.
         */
        static void GetEffectsMode(char* buffer, EffectsMode effectsMode);
        /*
         * Buffer should be at least 22 in size.
         */
        static void GetEffect1(char* buffer, EffectsMode effectsMode);
        /*
         * Buffer should be at least 22 in size.
         */
        static void GetEffect2(char* buffer, EffectsMode effectsMode);
        /*
         * Buffer should be at least 7 in size.
         */
        static void GetEffectSetting(char* buffer, EffectsMode effectsMode, int settingNum);
        /*
         * Buffer should be at least 13 in size.
         */
        static void GetReverbMode(char* buffer, ReverbMode reverbMode);
        /*
         * Buffer should be at least 22 in size.
         */
        static void GetReverbModePadded(char* buffer, ReverbMode reverbMode);
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_ENUM_TO_TEXT_H__
