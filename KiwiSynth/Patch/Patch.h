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
            bool                    liveMode;

        public:
            int                     splitNote;
            VoiceMode               voiceMode;
            EffectsMode             effectsMode;
            ReverbMode              reverbMode;
            PatchType               type;

            PatchSettings*          activeSettings;
            PatchSettings*          voice1Settings;
            PatchSettings*          voice2Settings;

            Patch() {}
            ~Patch() {}
            void Init(MultiPots* multiPots, GpioExpansion* ge);

            // BUFFER MUST BE MAX_PATCH_NAME_LENGTH IN SIZE!!! Not validating because we need speed
            inline void GetName(char* buffer) { strcpy(buffer, name); }
            inline void SetValue(char* buffer) { strcpy(name, buffer); }
            inline EffectsMode GetEffectsMode() { return effectsMode; }
            inline void SetEffectsMode(EffectsMode newEffectsMode) {
                if (newEffectsMode < 0) effectsMode = (EffectsMode)(newEffectsMode + FX_MODE_MAX);
                else effectsMode = (EffectsMode)(newEffectsMode % FX_MODE_MAX);
            }
            inline ReverbMode GetReverbMode() { return reverbMode; }
            inline void SetReverbMode(ReverbMode newReverbMode) {
                if (newReverbMode < 0) reverbMode = (ReverbMode)(newReverbMode + REVERB_MODE_MAX);
                else reverbMode = (ReverbMode)(newReverbMode % REVERB_MODE_MAX);
            }
            inline VoiceMode GetVoiceMode() { return voiceMode; }
            void SetVoiceMode(VoiceMode voiceMode);
            void DefaultSettings();
            inline bool GetLiveMode() { return liveMode; }
            void SetLiveMode(bool liveModeActive);
            void Load(SavedPatch savedPatch);
            void Save(SavedPatch* savedPatch);
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_VCA_H__
