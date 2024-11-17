#ifndef __KIWI_SYNTH_PATCH_HEADER_H__
#define __KIWI_SYNTH_PATCH_HEADER_H__

#include <string.h>

#include "PatchTypes.h"

namespace kiwisynth
{      

    struct PatchHeader
    {
        int                     bankNumber;
        int                     patchNumber;
        PatchType               type;
        VoiceMode               voiceMode;
        char                    name[MAX_PATCH_NAME_LENGTH + 1];
    };

} // namespace kiwisynth
#endif // __KIWI_SYNTH_PATCH_HEADER_H__
