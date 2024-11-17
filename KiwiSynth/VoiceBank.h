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
            VoiceMode               voiceMode;
            uint8_t                 maxVoices;
            std::vector<Voice>      voices;
            Patch*                  patch;
            Modulation              systemModulations[6]; // These are system-wide defaults that take place unless overridden by another mod using the same sources.
            std::vector<uint8_t>    playingNotes;
            int8_t                  paraOscNotes[6];
            bool                    paraOscPlaying[6];
            bool                    paraOscOffRequested[6]; // For paraphonic mode, set to true when an oscillator needs to be stopped after a slight delay to help with envelope release.
            int32_t                 oscTriggerCount;
            bool                    sampleAndHoldInputAvailable;

            Voice* RequestVoice(uint8_t midiNote);
            int RequestOscillator(uint8_t note);
            inline bool OscillatorIsAvailable(int oscillator) { return (!paraOscPlaying[oscillator] || paraOscOffRequested[oscillator]); }
 
        public:
            std::vector<int> playingVoices;
            Modulation** modulations;
            uint8_t numVoices;

            VoiceBank() {}
            ~VoiceBank() {}
            void Init(uint8_t maxVoices, uint8_t numOscillators, Patch* patch, float sampleRate);
            void Process(float* sample);

            inline VoiceMode GetVoiceMode() { return voiceMode; }
            void UpdateSettings();
            void InitModulations();
            void UpdateModulations();
            void NoteOn(uint8_t note, uint8_t velocity);
            void NoteOff(uint8_t note, uint8_t velocity);
            void AllNotesOff();
    };

} // namespace kiwisynth
#endif // __KIWI_SYNTH_VOICE_BANK_H__
