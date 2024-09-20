#ifndef __KIWI_SYNTH_PATCH_H__
#define __KIWI_SYNTH_PATCH_H__


#include "daisysp.h"

#include "PatchSettings.h"
#include "SavedPatch.h"

using namespace daisysp;

namespace kiwi_synth
{
    class Patch
    {
        private:
            MultiPots*              multiPots;
            GpioExpansion*          ge;

            PatchSettings           settings1;
            PatchSettings           settings2;
            char                    name[MAX_PATCH_NAME_LENGTH + 1];

        public:
            int                     splitNote;
            VoiceMode               voiceMode;
            EffectsMode             effectsMode;
            ReverbMode              reverbMode;

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
            inline ReverbMode getReverbMode() { return reverbMode; }
            inline void setReverbMode(ReverbMode newReverbMode) {
                if (newReverbMode < 0) reverbMode = (ReverbMode)(newReverbMode + REVERB_MODE_MAX);
                else reverbMode = (ReverbMode)(newReverbMode % REVERB_MODE_MAX);
            }
            void SetVoiceMode(VoiceMode voiceMode);
            void DefaultSettings();
            void Load(SavedPatch savedPatch);
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_VCA_H__
