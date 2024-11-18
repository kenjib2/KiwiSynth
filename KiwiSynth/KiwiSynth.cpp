#include "KiwiSynth.h"



using namespace kiwisynth;



int patchHeaderSort(PatchHeader* a, PatchHeader* b) {
    return strcasecmp(a->name, b->name) < 0;
}



void KiwiSynth::Init(DaisySeed* hw, float sampleRate)
{
    hw_ = hw;
    numVoices_ = MAX_VOICES;
    SetMidiChannel(0);

    ConfigureMultiPots(hw);
    ConfigureGpioExpansion();

    patch.Init(&multiPots_, &ge_);

    voiceBank_.Init(numVoices_, NUM_OSCS, &patch, sampleRate);
    effectsEngine_.Init(&patch, sampleRate);

    storage_.Init(hw->qspi);
    LoadPatchBanks();

    InitMidi();
    sysexManager_.Init(&midi_);
}



void KiwiSynth::LoadPatchBanks() {
    for (int i = 0; i < NUM_PATCH_BANKS; i++) {
        for (int j = 0; j < PATCHES_PER_BANK; j++) {
            SavedPatch nextPatch = storage_.LoadPatch(i, j);
            patchBanks[i][j].bankNumber = i;
            patchBanks[i][j].patchNumber = j;
            strcpy(patchBanks[i][j].name, nextPatch.name);
            patchBanks[i][j].type = nextPatch.type;
            patchBanks[i][j].voiceMode = nextPatch.voiceMode;

            // Ignore initialized default patches
            if (nextPatch.type != PATCH_INIT) {
                patchTypes[nextPatch.type].push_back(&patchBanks[i][j]);
            }
        }
    }

    for (int i = 0; i < PATCH_TYPE_MAX; i++) {
        std::sort( patchTypes[i].begin(), patchTypes[i].end(), patchHeaderSort );
    }
}



void KiwiSynth::ConfigureMultiPots(DaisySeed* hw)
{
    std::vector<Pin> mpPins;
    mpPins.push_back(A0);
    mpPins.push_back(A1);
    mpPins.push_back(A2);

    std::vector<Pin> directPins;
    directPins.push_back(A4);
    directPins.push_back(A5);

    MultiPotsConfig multiPotsConfig;
    multiPotsConfig.pinA0 = D7;
    multiPotsConfig.pinA1 = D8;
    multiPotsConfig.pinA2 = D9;
    multiPotsConfig.pinA3 = D10;
    multiPotsConfig.pinsSignal = mpPins;
    multiPotsConfig.pinsDirect = directPins;

    multiPots_.Init(hw, &multiPotsConfig);
}



void KiwiSynth::ConfigureGpioExpansion()
{
    ge_.Init();
}



void KiwiSynth::InitMidi()
{
    MidiUartHandler::Config midi_config;
    midi_config.transport_config.tx = D13;
    midi_config.transport_config.rx = D14;
    midi_.Init(midi_config);
    midi_.StartReceive();
    
    gpioMidiActivity_.Init(seed::D22, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL);
    gpioMidiActivity_.Write(false);
    midiCounter_ = 0;

    gpioSustain_.Init(seed::A6, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);
}



void KiwiSynth::SetMidiChannel(int midiChannel)
{
    midiChannel_ = midiChannel;
}



void KiwiSynth::ClearMidi() {
    midi_.Listen();
    while(midi_.HasEvents())
    {
        midi_.PopEvent();
    }
}



void KiwiSynth::Panic() {
    AllNotesOff();
    ClearMidi();
}



void KiwiSynth::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
{
    ProcessMidi();
    isMidiLedNeeded_ = (midiCounter_ < MIDI_LED_DURATION);
    midiCounter_++;

    for(size_t i = 0; i < size; i += 2)
    {
        voiceBank_.Process(&out[i]);

        effectsEngine_.Process(&(out[i]));
    }
}



void KiwiSynth::ProcessMidi()
{
    midi_.Listen();
    while(midi_.HasEvents())
    {
        MidiEvent event = midi_.PopEvent();
        HandleMidiMessage(&event);
    }

    float sustain = (float)!gpioSustain_.Read();
    patch.voice1Settings_->forceSetValue(GEN_SUSTAIN, sustain);
    patch.voice2Settings_->forceSetValue(GEN_SUSTAIN, sustain);
    
    float expression = hw_->adc.GetFloat(4);
    patch.voice1Settings_->forceSetValue(GEN_EXPRESSION, expression);
    patch.voice2Settings_->forceSetValue(GEN_EXPRESSION, expression);
}



void KiwiSynth::ProcessInputs(bool forceGeRead)
{
    if (isMidiLedNeeded_ != isMidiLedOn_) {
        isMidiLedOn_ = isMidiLedNeeded_;
        gpioMidiActivity_.Write(isMidiLedNeeded_);
    }

    multiPots_.Process();
    if (!ge_.Process(forceGeRead)) {
        // If not reading GPIO, we need to delay to give the Multiplexer time to switch channels.
        System::DelayUs(MULTIPLEX_CHANNEL_DELAY);
    }
}



void KiwiSynth::HandleMidiMessage(MidiEvent* midiEvent)
{
    if (midiChannel_ == midiEvent->channel) {
        NoteOnEvent on;
        NoteOffEvent off;
        ControlChangeEvent cc;
        float value;
        switch(midiEvent->type)
        {
            case NoteOn:
                midiCounter_ = 0;
                on = midiEvent->AsNoteOn();
                if(midiEvent->data[1] != 0) // This is to avoid Max/MSP Note outs for now..
                {
                    voiceBank_.NoteOn(on.note, on.velocity);
                    break;
                }
                break;

            case NoteOff:
                midiCounter_ = 0;
                off = midiEvent->AsNoteOff();
                voiceBank_.NoteOff(off.note, off.velocity);
                break;

            case PitchBend:
                midiCounter_ = 0;
                value = (float)midiEvent->AsPitchBend().value / 8192.0f;
                patch.voice1Settings_->forceSetValue(GEN_PITCH_BEND, value);
                patch.voice2Settings_->forceSetValue(GEN_PITCH_BEND, value);
                break;

            case ChannelPressure:
                midiCounter_ = 0;
                value = (float)midiEvent->AsChannelPressure().pressure / 127.0f;
                patch.voice1Settings_->forceSetValue(GEN_AFTERTOUCH, value);
                patch.voice2Settings_->forceSetValue(GEN_AFTERTOUCH, value);
                break;

            case ControlChange:
                cc = midiEvent->AsControlChange();
                switch (cc.control_number) {
                    case 1:  // Mod Wheel
                        midiCounter_ = 0;
                        value = (float)cc.value / 127.0f;
                        patch.voice1Settings_->forceSetValue(GEN_MOD_WHEEL, value);
                        patch.voice2Settings_->forceSetValue(GEN_MOD_WHEEL, value);
                        break;

                    case 11: // Expression
                        midiCounter_ = 0;
                        value = (float)cc.value / 127.0f;
                        patch.voice1Settings_->forceSetValue(GEN_EXPRESSION, value);
                        patch.voice2Settings_->forceSetValue(GEN_EXPRESSION, value);
                        break;

                    case 64: // Sustain Pedal
                        midiCounter_ = 0;

                        value = (float)((cc.value & 0b1000000) >> 6); // 0.0 if 0-63 or less, 1.0 if 64-127
                        patch.voice1Settings_->forceSetValue(GEN_SUSTAIN, value); // Off
                        patch.voice2Settings_->forceSetValue(GEN_SUSTAIN, value); // Off
                        break;

                }
                break;

            case PolyphonicKeyPressure:
            case ProgramChange:
            case SystemCommon:
            case SystemRealTime:
            case ChannelMode:
            case MessageLast:
            default:
                break;
        }
    }
}



void KiwiSynth::AllNotesOff()
{
    voiceBank_.AllNotesOff();
}



bool KiwiSynth::BootLoaderRequested()
{
    return patch.voice1Settings_->getBoolValue(PatchSetting::GEN_SELECT_BUTTON);
}



void KiwiSynth::LoadPatch(int bankNumber, int patchNumber)
{
    patch.SetLiveMode(false, bankNumber, patchNumber);
    SavedPatch savedPatch = storage_.LoadPatch(bankNumber, patchNumber);
    patch.Load(savedPatch);
    voiceBank_.UpdateSettings();
    patch.Load(savedPatch);     // For some reason multitimbral voice modes require loading/updating the patch twice.
    voiceBank_.UpdateSettings(); // I need to debug this properly but for now it's working via this workaround.
    effectsEngine_.UpdateSettings();
}



void KiwiSynth::SavePatch(int bankNumber, int patchNumber)
{
    PatchHeader oldPatchHeader = patchBanks[bankNumber][patchNumber];
    PatchHeader patchHeader;
    patch.GetPatchHeader(&patchHeader);

    // Update the patch bank header data
    patchBanks[bankNumber][patchNumber] = patchHeader;

    // Find and remove the element from the patch type vector
    for (unsigned int i = 0; i < patchTypes[oldPatchHeader.type].size(); i++) {
        if (patchTypes[oldPatchHeader.type][i]->bankNumber == oldPatchHeader.bankNumber
            && patchTypes[oldPatchHeader.type][i]->patchNumber == oldPatchHeader.patchNumber) {
                patchTypes[oldPatchHeader.type].erase(patchTypes[oldPatchHeader.type].begin() + i);
                break;
            }
    }

    // Add the new element and sort it
    patchTypes[patchHeader.type].push_back(&patchBanks[bankNumber][patchNumber]);
    std::sort( patchTypes[patchHeader.type].begin(), patchTypes[patchHeader.type].end(), patchHeaderSort );

    storage_.SavePatch(&patch, bankNumber, patchNumber);
    patch.SetLiveMode(false, bankNumber, patchNumber);
}



void KiwiSynth::SendSysex()
{
    sysexManager_.Send(&patch);
}



void KiwiSynth::ReceiveSysex()
{
    sysexManager_.Receive(&patch, &ge_);
}



void KiwiSynth::UpdateSettings()
{
    voiceBank_.UpdateSettings();
    effectsEngine_.UpdateSettings();
}
