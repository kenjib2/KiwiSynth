#ifndef __KIWI_SYNTH_PATCH_H__
#define __KIWI_SYNTH_PATCH_H__


#include "daisysp.h"

#include "PatchSettings.h"

using namespace daisysp;

namespace kiwi_synth
{
    typedef enum {
        VOICE_MODE_POLY,
        VOICE_MODE_MONO,
        VOICE_MODE_MULTI
    } VoiceMode;

    typedef enum {
        FX_DIST_DLY_HLRVB,
        FX_DIST_DLY_CHRVB,
        FX_DIST_DLY_CARVB,
        FX_DIST_DLY_RMRVB,
        FX_DIST_DLY_BLRVB
    } EffectsMode;
    static const int FX_MODE_MAX = 5;

    class Patch
    {
        private:
            VoiceMode               voiceMode;
            MultiPots*              multiPots;
            GpioExpansion*          ge;
            PatchSettings           settings1;
            PatchSettings           settings2;
            char                    name[MAX_PATCH_NAME_LENGTH + 1];
            EffectsMode             effectsMode;

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
            inline EffectsMode getEffectsMode() { return effectsMode; }
            inline void setEffectsMode(EffectsMode newEffectsMode) {
                if (newEffectsMode < 0) effectsMode = (EffectsMode)(newEffectsMode + FX_MODE_MAX);
                else effectsMode = (EffectsMode)(newEffectsMode % FX_MODE_MAX);
            }
            void SetVoiceMode(VoiceMode voiceMode);
    };
}


#endif // __KIWI_SYNTH_VCA_H__
