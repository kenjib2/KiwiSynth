#ifndef __KIWI_SYNTH_SAVED_PATCH_H__
#define __KIWI_SYNTH_SAVED_PATCH_H__

#include "PatchSettings.h"

namespace kiwi_synth
{
    typedef struct
    {
        VoiceMode               voiceMode;
        int                     splitNote;
        char                    name[MAX_PATCH_NAME_LENGTH + 1];
        EffectsMode             effectsMode;
        ReverbMode              reverbMode;

        float voice1FloatValues[NUM_PATCH_SETTINGS]; // was float floatValues[59];
        int8_t voice1IntValues[NUM_PATCH_SETTINGS]; // was int8_t intValues[31];
        bool voice1BoolValues[NUM_PATCH_SETTINGS]; // was bool boolValues[10];

        float voice2FloatValues[NUM_PATCH_SETTINGS]; // was float floatValues[59];
        int8_t voice2IntValues[NUM_PATCH_SETTINGS]; // was int8_t intValues[31];
        bool voice2BoolValues[NUM_PATCH_SETTINGS]; // was bool boolValues[10];
    } SavedPatch;

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_SAVED_PATCH_H__
