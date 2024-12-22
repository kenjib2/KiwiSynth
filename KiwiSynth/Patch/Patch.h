#ifndef __KIWI_SYNTH_PATCH_H__
#define __KIWI_SYNTH_PATCH_H__



#include "daisysp.h"

#include "PatchSettings.h"
#include "PatchHeader.h"
#include "SavedPatch.h"



using namespace daisysp;

namespace kiwisynth
{



class Patch
{
    private:
        MultiPots*              multiPots_;
        GpioExpansion*          ge_;

        char                    name_[MAX_PATCH_NAME_LENGTH + 1];
        bool                    isLiveModeActive_;
        int                     bankNumber_;
        int                     patchNumber_;
        VoiceMode               voiceMode_;
        int                     splitNote_;
        EffectsMode             effectsMode_;
        ReverbMode              reverbMode_;
        PatchType               type_;

    public:
        PatchSettings           settings1_;
        PatchSettings           settings2_;
        PatchSettings*          voice1Settings_;
        PatchSettings*          voice2Settings_;

        Patch() {}
        ~Patch() {}
        void Init(MultiPots* multiPots, GpioExpansion* ge);

        void DefaultSettings();
        void ReadFromSavedPatch(SavedPatch savedPatch);
        void WriteToSavedPatch(SavedPatch* savedPatch);
        void GetPatchHeader(PatchHeader* patchHeader);
        void SwapLayers();



        // BUFFER MUST BE MAX_PATCH_NAME_LENGTH IN SIZE!!! Not validating because we need speed
        inline void GetName(char* buffer) { strcpy(buffer, name_); }



        inline void SetName(char* buffer) { strcpy(name_, buffer); }



        inline PatchType GetPatchType() { return type_; }



        inline void SetPatchType(PatchType newPatchType) {
            if (newPatchType < 0) type_ = (PatchType)(newPatchType + PATCH_TYPE_MAX);
            else type_ = (PatchType)(newPatchType % PATCH_TYPE_MAX);
        }



        inline EffectsMode GetEffectsMode() { return effectsMode_; }



        inline void SetEffectsMode(EffectsMode newEffectsMode) {
            if (newEffectsMode < 0) effectsMode_ = (EffectsMode)(newEffectsMode + FX_MODE_MAX);
            else effectsMode_ = (EffectsMode)(newEffectsMode % FX_MODE_MAX);
        }



        inline ReverbMode GetReverbMode() { return reverbMode_; }



        inline void SetReverbMode(ReverbMode newReverbMode) {
            if (newReverbMode < 0) reverbMode_ = (ReverbMode)(newReverbMode + REVERB_MODE_MAX);
            else reverbMode_ = (ReverbMode)(newReverbMode % REVERB_MODE_MAX);
        }



        inline VoiceMode GetVoiceMode() { return voiceMode_; }



        void SetVoiceMode(VoiceMode voiceMode);



        inline int GetSplitNote() { return splitNote_; }



        inline void SetSplitNote(int newSplitNote) { splitNote_ = newSplitNote; }



        inline bool GetLiveMode() { return isLiveModeActive_; }



        void SetLiveMode(bool isLive, int bankNumber = -1, int patchNumber = -1);



        inline int GetBankNumber() { return bankNumber_; }



        inline int GetPatchNumber() { return patchNumber_; }
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_PATCH_H__
