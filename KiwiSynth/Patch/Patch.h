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
            PatchSettings*          activeSettings;
            PatchSettings*          voice1Settings;
            PatchSettings*          voice2Settings;

        public:
            Patch() {}
            ~Patch() {}
            void Init(MultiPots* multiPots, GpioExpansion* ge);

            void SetVoiceMode(VoiceMode voiceMode);

            inline PatchSettings* getActiveSettings() {
                return activeSettings;
            }

            inline PatchSettings* getVoice1Settings() {
                return voice1Settings;
            }

            inline PatchSettings* getVoice2Settings() {
                return voice2Settings;
            }

            void UpdateSettings();
    };
}


#endif // __KIWI_SYNTH_VCA_H__
