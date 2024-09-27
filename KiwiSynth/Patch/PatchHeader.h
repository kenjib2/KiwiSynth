#ifndef __KIWI_SYNTH_PATCH_HEADER_H__
#define __KIWI_SYNTH_PATCH_HEADER_H__

#include <string.h>

#include "PatchTypes.h"

namespace kiwi_synth
{      

    struct PatchHeader
    {
        int                     bankNumber;
        int                     patchNumber;
        PatchType               type;
        VoiceMode               voiceMode;
        char                    name[MAX_PATCH_NAME_LENGTH + 1];
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_PATCH_HEADER_H__
