#ifndef __KIWI_SYNTH_PATCH_H__
#define __KIWI_SYNTH_PATCH_H__


#include "daisysp.h"

#include "../Patch/PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    typedef enum {
        VOICE_MODE_POLY,
        VOICE_MODE_MONO,
        VOICE_MODE_MULTI
    } VoiceMode;

    class Patch
    {
        private:
            VoiceMode               voiceMode;
            MultiPots*              multiPots;
            GpioExpansion*          ge;
            PatchSettings           settings1;
            PatchSettings           settings2;
            char                    name[MAX_PATCH_NAME_LENGTH + 1];

        public:
            PatchSettings*          activeSettings;
            PatchSettings*          voice1Settings;
            PatchSettings*          voice2Settings;

            Patch() {}
            ~Patch() {}
            void Init(MultiPots* multiPots, GpioExpansion* ge);

            // BUFFER MUST BE MAX_PATCH_NAME_LENGTH IN SIZE!!! Not validating because we need speed
            inline void getName(char* buffer) { strcpy(buffer, name); }
            inline void setValue(char* buffer) { strcpy(name, buffer); }
            void SetVoiceMode(VoiceMode voiceMode);
    };
}


#endif // __KIWI_SYNTH_VCA_H__
