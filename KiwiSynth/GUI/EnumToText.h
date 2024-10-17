#ifndef __KIWI_SYNTH_ENUM_TO_TEXT_H__
#define __KIWI_SYNTH_ENUM_TO_TEXT_H__

#include "daisy_seed.h"
#include "../Patch/PatchTypes.h"

namespace kiwi_synth
{

    class EnumToText
    {
        public:
            /*
             * Buffer should be at least 3 in size.
             */
            static void GetWaveform(char* buffer, Waveform waveform);
            /*
             * Buffer should be at least 3 in size.
             */
            static void GetNoiseType(char* buffer, NoiseType noiseType);
            /*
             * Buffer should be at least 3 in size.
             */
            static void GetLfoWaveform(char* buffer, LfoWaveform lfoWaveform);
            /*
             * Buffer should be at least 9 in size.
             */
            static void GetVcfType(char* buffer, FilterType vcfType);
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
             * Buffer should be at least 11 in size.
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
             * Buffer should be at least 13 in size.
             */
            static void GetReverbMode(char* buffer, ReverbMode reverbMode);
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_ENUM_TO_TEXT_H__
