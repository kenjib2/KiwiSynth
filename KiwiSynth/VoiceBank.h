#ifndef __KIWI_SYNTH_VOICE_BANK_H__
#define __KIWI_SYNTH_VOICE_BANK_H__



#include "daisysp.h"

#include "Voice.h"
#include "Patch/Patch.h"
#include "Modulation.h"



using namespace daisysp;

namespace kiwisynth
{



const static int OSC_TRIGGER_SAMPLES = 1024;



class VoiceBank
{
    private:
        VoiceMode               voiceMode_;
        uint8_t                 maxVoices_;
        std::vector<Voice>      voices_;
        Patch*                  patch_;
        Modulation              systemModulations_[6]; // These are system-wide defaults that take place unless overridden by another mod using the same sources.
        std::vector<uint8_t>    playingNotes_;
        int8_t                  paraOscNotes_[6];
        bool                    isParaOscPlaying_[6];
        bool                    isParaOscOffRequested_[6]; // For paraphonic mode, set to true when an oscillator needs to be stopped after a slight delay to help with envelope release.
        int32_t                 oscTriggerCount_;
        bool                    isSampleAndHoldInputAvailable_;

        Voice* RequestVoice(uint8_t midiNote);
        int RequestOscillator(uint8_t note);
        inline bool IsOscillatorAvailable(int oscillator) { return (!isParaOscPlaying_[oscillator] || isParaOscOffRequested_[oscillator]); }



    public:
        std::vector<int> playingVoices_;
        Modulation** modulations_;
        uint8_t numVoices_;

        VoiceBank() {}
        ~VoiceBank() {}
        void Init(uint8_t maxVoices, uint8_t numOscillators, Patch* patch, float sampleRate);
        void Process(float* sample);

        inline VoiceMode GetVoiceMode() { return voiceMode_; }
        void UpdateSettings();
        void InitModulations();
        void UpdateModulations();
        void NoteOn(uint8_t note, uint8_t velocity);
        void NoteOff(uint8_t note, uint8_t velocity);
        void AllNotesOff();
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_VOICE_BANK_H__
