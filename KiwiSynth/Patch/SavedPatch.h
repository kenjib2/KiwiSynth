#ifndef __KIWI_SYNTH_SAVED_PATCH_H__
#define __KIWI_SYNTH_SAVED_PATCH_H__

#include "PatchTypes.h"

namespace kiwi_synth
{
    struct SavedPatch
    {
        PatchType               type;
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
        
        //Overloading the != operator
        //This is necessary as this operator is used in the PersistentStorage source code
        bool operator!=(const SavedPatch& a) const {
            return !(!strcmp(a.name, name) &&
                     a.type==type &&
                     a.voiceMode==voiceMode &&
                     a.splitNote==splitNote &&
                     a.effectsMode==effectsMode &&
                     a.reverbMode==reverbMode
                     );
        }
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_SAVED_PATCH_H__
