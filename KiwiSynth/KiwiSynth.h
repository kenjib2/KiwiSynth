#ifndef __KIWI_SYNTH_KIWI_SYNTH_H__
#define __KIWI_SYNTH_KIWI_SYNTH_H__



#include<vector>
#include<strings.h>

#include "daisy_seed.h"

#include "Util/KUtils.h"
#include "Controls/MultiPots.h"
#include "Controls/KiwiMidi.h"
#include "Patch/Patch.h"
#include "VoiceBank.h"
#include "Effects/EffectsEngine.h"
#include "Patch/Storage.h"
#include "Controls/SysexManager.h"
#include "Patch/PatchHeader.h"



using namespace daisy;
using namespace daisy::seed;

namespace kiwisynth
{


static const int MIDI_LED_DURATION = 20;
static const int MULTIPLEX_CHANNEL_DELAY = 550;



class KiwiSynth
{
    private:
        static const int NUM_OSCS = 3;
        static const int MAX_VOICES = 3;

        DaisySeed* hw_;
        MultiPots multiPots_;
        GpioExpansion ge_;
        Storage storage_;
        SysexManager sysexManager_;
        int numVoices_;
        int midiChannel_;
        GPIO gpioMidiActivity_;
        GPIO gpioSustain_;
        int midiCounter_ = 0;
        bool isMidiLedOn_ = false;
        bool isMidiLedNeeded_ = false;
        KiwiMidiUartHandler midi_;
        VoiceBank voiceBank_;
        EffectsEngine effectsEngine_;

        void ProcessMidi();
        void HandleMidiMessage(KiwiMidiEvent* midiEvent);

    public:
        Patch patch;
        PatchHeader patchBanks[NUM_PATCH_BANKS][PATCHES_PER_BANK];
        std::vector<PatchHeader*> patchTypes[PATCH_TYPE_MAX];

        KiwiSynth() {}
        ~KiwiSynth() {}
        void Init(DaisySeed* hw, float sampleRate);

        void ProcessInputs(bool forceGeRead = false);
        void ConfigureMultiPots(DaisySeed* hw);
        void ConfigureGpioExpansion();

        void InitMidi();
        void SetMidiChannel(int midiChannel);
        /*
         * Clears the listener of all midi events without processing them.
         */
        void ClearMidi();
        void Panic();

        bool BootLoaderRequested();
        void LoadPatchBanks();
        void LoadPatch(int bankNumber, int patchNumber);
        void SavePatch(int bankNumber, int patchNumber);
        void ReceiveSysex(int bankNumber, int patchNumber);
        void SendSysex(int bankNumber, int patchNumber);
        void UpdateSettings();
        void Process(AudioHandle::InterleavingOutputBuffer out, size_t size);
        void AllNotesOff();
};



} // namespace kiwisynth
#endif // __KIWI_SYNTH_H__
