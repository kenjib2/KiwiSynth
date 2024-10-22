#ifndef __KIWI_SYNTH_PATCH_H__
#define __KIWI_SYNTH_PATCH_H__


#include "daisysp.h"

#include "PatchSettings.h"
#include "PatchHeader.h"
#include "SavedPatch.h"

using namespace daisysp;

namespace kiwi_synth
{
    class Patch
    {
        private:
            MultiPots*              multiPots;
            GpioExpansion*          ge;

            char                    name[MAX_PATCH_NAME_LENGTH + 1];
            bool                    liveMode;
            int                     bankNumber;
            int                     patchNumber;
            VoiceMode               voiceMode;
            int                     splitNote;
            EffectsMode             effectsMode;
            ReverbMode              reverbMode;
            PatchType               type;

        public:
            PatchSettings           settings1;
            PatchSettings           settings2;
            PatchSettings*          voice1Settings;
            PatchSettings*          voice2Settings;

            Patch() {}
            ~Patch() {}
            void Init(MultiPots* multiPots, GpioExpansion* ge);

            // BUFFER MUST BE MAX_PATCH_NAME_LENGTH IN SIZE!!! Not validating because we need speed
            inline void GetName(char* buffer) { strcpy(buffer, name); }
            inline void SetName(char* buffer) { strcpy(name, buffer); }

            inline PatchType GetPatchType() { return type; }
            inline void SetPatchType(PatchType newPatchType) {
                if (newPatchType < 0) type = (PatchType)(newPatchType + PATCH_TYPE_MAX);
                else type = (PatchType)(newPatchType % PATCH_TYPE_MAX);
            }

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

            inline int GetSplitNote() { return splitNote; }
            inline void SetSplitNote(int newSplitNote) { splitNote = newSplitNote; }

            inline bool GetLiveMode() { return liveMode; }
            void SetLiveMode(bool isLive, int bankNumber = -1, int patchNumber = -1);
            inline int GetBankNumber() { return bankNumber; }
            inline int GetPatchNumber() { return patchNumber; }

            void DefaultSettings();
            void Load(SavedPatch savedPatch);
            void Save(SavedPatch* savedPatch);
            void GetPatchHeader(PatchHeader* patchHeader);
    };

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_PATCH_H__
