#ifndef __KIWI_SYNTH_PATCH_H__
#define __KIWI_SYNTH_PATCH_H__


#include "daisysp.h"

#include "../../KUtils.h"
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
            PatchSettings*          voice1Settings;
            PatchSettings*          voice2Settings;
            PatchSettings**         activeSettings;
            #ifdef __FUNCTIONALITY_OPTION__
            PatchSettings           voice3Settings;
            #endif // __FUNCTIONALITY_OPTION__

            void SetActivePatchSettings(int voiceNumber);

        public:
            Patch() {}
            ~Patch() {}
            void Init(PatchSettings* voice1Settings, PatchSettings* voice2Settings, MultiPots* multiPots, GpioExpansion* ge);

            inline PatchSettings* getActiveSettings() {
                return *activeSettings;
            }
            inline PatchSettings* getVoice1Settings() {
                return voice1Settings;
            }
            inline PatchSettings* getVoice2Settings() {
                return voice2Settings;
            }
            #ifdef __FUNCTIONALITY_OPTION__
            inline PatchSettings* getVoice3Settings() {
                return voice3Settings;
            }
            #endif // __FUNCTIONALITY_OPTION__
            void UpdateSettings();
    };
}


#endif // __KIWI_SYNTH_VCA_H__
