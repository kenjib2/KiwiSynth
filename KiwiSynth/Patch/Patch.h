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
        VOICE_MODE_MULTI,
        VOICE_MODE_3V
    } VoiceMode;

    class Patch
    {
        private:
            VoiceMode               voiceMode;
            MultiPots*              multiPots;
            GpioExpansion*          ge;

            void SetActivePatchSettings(int voiceNumber);

        public:
            #ifdef __FUNCTIONALITY_OPTION__
            PatchSettings           patchSettings[3];
            #else
            PatchSettings           patchSettings[2];
            #endif // __FUNCTIONALITY_OPTION__
            PatchSettings*          activeSettings;

            Patch() {}
            ~Patch() {}
            void Init(MultiPots* multiPots, GpioExpansion* ge);

            void UpdateSettings();
    };
}


#endif // __KIWI_SYNTH_VCA_H__
