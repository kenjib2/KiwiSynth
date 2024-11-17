#ifndef __KIWI_SYNTH_SAVED_PATCH_H__
#define __KIWI_SYNTH_SAVED_PATCH_H__



#include "PatchTypes.h"



namespace kiwisynth
{



const static int NUM_PATCH_BANKS = 8;
const static int PATCHES_PER_BANK = 128;
const static int TOTAL_PATCHES = NUM_PATCH_BANKS * PATCHES_PER_BANK;



struct SavedPatch
{
    PatchType               type;
    VoiceMode               voiceMode;
    int                     splitNote;
    char                    name[MAX_PATCH_NAME_LENGTH + 1];
    EffectsMode             effectsMode;
    ReverbMode              reverbMode;

    float voice1FloatValues[NUM_PATCH_SETTINGS_SAVED];
    int8_t voice1IntValues[NUM_PATCH_SETTINGS_SAVED];
    bool voice1BoolValues[NUM_PATCH_SETTINGS_SAVED];

    float voice2FloatValues[NUM_PATCH_SETTINGS_SAVED];
    int8_t voice2IntValues[NUM_PATCH_SETTINGS_SAVED];
    bool voice2BoolValues[NUM_PATCH_SETTINGS_SAVED];
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_SAVED_PATCH_H__
