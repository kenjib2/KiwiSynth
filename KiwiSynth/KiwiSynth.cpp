#include "KiwiSynth.h"

namespace kiwi_synth
{
    int patchHeaderSort(PatchHeader* a, PatchHeader* b) {
        return strcmp(a->name, b->name);
    }

    void KiwiSynth::Init(DaisySeed* hw, float sampleRate)
    {
        this->hw = hw;
        numVoices = MAX_VOICES;
        SetMidiChannel(0);

        ConfigureMultiPots(hw);
        ConfigureGpioExpansion();

        patch.Init(&multiPots, &ge);

        voiceBank.Init(numVoices, NUM_VCOS, &patch, sampleRate);
        effectsEngine.Init(&patch, sampleRate);

        storage.Init(hw->qspi);
        LoadPatchBanks();

        InitMidi();
    }

    void KiwiSynth::LoadPatchBanks() {
        for (int i = 0; i < NUM_PATCH_BANKS; i++) {
            for (int j = 0; j < PATCHES_PER_BANK; j++) {
                SavedPatch nextPatch = storage.LoadPatch(i, j);
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

        multiPots.Init(hw, &multiPotsConfig);
    }

    void KiwiSynth::ConfigureGpioExpansion()
    {
    	ge.Init();
    }

    void KiwiSynth::InitMidi()
    {
        MidiUartHandler::Config midi_config;
        midi_config.transport_config.tx = D13;
        midi_config.transport_config.rx = D14;
        midi.Init(midi_config);
        midi.StartReceive();
	    
        gpioMidiActivity.Init(seed::D22, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL);
        gpioMidiActivity.Write(false);
        midiCounter = 0;

        gpioSustain.Init(seed::A6, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);
    }

    void KiwiSynth::SetMidiChannel(int midiChannel)
    {
        this->midiChannel = midiChannel;
    }

    void KiwiSynth::ClearMidi() {
        midi.Listen();
        while(midi.HasEvents())
        {
            midi.PopEvent();
        }
    }

    void KiwiSynth::Panic() {
        AllNotesOff();
        ClearMidi();
    }

    void KiwiSynth::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
    {
        ProcessMidi();
        midiLEDOn = (midiCounter < MIDI_LED_DURATION);
        midiCounter++;

        for(size_t i = 0; i < size; i += 2)
        {
            voiceBank.Process(&out[i]);

            effectsEngine.Process(&(out[i]));
        }
    }

    void KiwiSynth::ProcessMidi()
    {
        midi.Listen();
        while(midi.HasEvents())
        {
            MidiEvent event = midi.PopEvent();
            HandleMidiMessage(&event);
        }

        float sustain = (float)!gpioSustain.Read();
        patch.voice1Settings->setValue(GEN_SUSTAIN, sustain);
        patch.voice2Settings->setValue(GEN_SUSTAIN, sustain);
        
        float expression = hw->adc.GetFloat(4);
        patch.voice1Settings->setValue(GEN_EXPRESSION, expression);
        patch.voice2Settings->setValue(GEN_EXPRESSION, expression);
    }

    void KiwiSynth::ProcessInputs(bool readGpio)
    {
        if (midiLEDOn != midiLEDState) {
            midiLEDState = midiLEDOn;
            gpioMidiActivity.Write(midiLEDOn);
        }

        multiPots.Process();
        if (readGpio) {
            ge.Process();
        } else {
            // If not reading GPIO, we need to delay to give the Multiplexer time to switch channels.
            System::DelayUs(MULTIPLEX_CHANNEL_DELAY);
        }
    }

    void KiwiSynth::HandleMidiMessage(MidiEvent* midiEvent)
    {
        if (midiChannel == midiEvent->channel) {
            NoteOnEvent on;
            NoteOffEvent off;
            ControlChangeEvent cc;
            float value;
            switch(midiEvent->type)
            {
                case NoteOn:
                    midiCounter = 0;
                    on = midiEvent->AsNoteOn();
                    if(midiEvent->data[1] != 0) // This is to avoid Max/MSP Note outs for now..
                    {
                        voiceBank.NoteOn(on.note, on.velocity);
                        break;
                    }
                    break;

                case NoteOff:
                    midiCounter = 0;
                    off = midiEvent->AsNoteOff();
                    voiceBank.NoteOff(off.note, off.velocity);
                    break;

                case PitchBend:
                    midiCounter = 0;
                    value = (float)midiEvent->AsPitchBend().value / 8192.0f;
                    patch.voice1Settings->setValue(GEN_PITCH_BEND, value);
                    patch.voice2Settings->setValue(GEN_PITCH_BEND, value);
                    break;

                // Unimplemented message types
                case ChannelPressure:
                    midiCounter = 0;
                    value = (float)midiEvent->AsChannelPressure().pressure / 127.0f;
                    patch.voice1Settings->setValue(GEN_AFTERTOUCH, value);
                    patch.voice2Settings->setValue(GEN_AFTERTOUCH, value);
                    break;

                case ControlChange:
                    cc = midiEvent->AsControlChange();
                    switch (cc.control_number) {
                        case 1:  // Mod Wheel
                            midiCounter = 0;
                            value = (float)cc.value / 127.0f;
                            patch.voice1Settings->setValue(GEN_MOD_WHEEL, value);
                            patch.voice2Settings->setValue(GEN_MOD_WHEEL, value);
                            break;

                        case 11: // Expression
                            midiCounter = 0;
                            value = (float)cc.value / 127.0f;
                            patch.voice1Settings->setValue(GEN_EXPRESSION, value);
                            patch.voice2Settings->setValue(GEN_EXPRESSION, value);
                            break;

                        case 64: // Sustain Pedal
                            midiCounter = 0;

                            value = (float)((cc.value & 0b1000000) >> 6); // 0.0 if 0-63 or less, 1.0 if 64-127
                            patch.voice1Settings->setValue(GEN_SUSTAIN, value); // Off
                            patch.voice2Settings->setValue(GEN_SUSTAIN, value); // Off
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
        voiceBank.AllNotesOff();
    }

    bool KiwiSynth::BootLoaderRequested()
    {
        return patch.voice1Settings->getBoolValue(PatchSetting::GEN_SELECT_BUTTON);
    }

    void KiwiSynth::LoadPatch(int bankNumber, int patchNumber)
    {
        patch.SetLiveMode(false, bankNumber, patchNumber);
        SavedPatch savedPatch = storage.LoadPatch(bankNumber, patchNumber);
        patch.Load(savedPatch);
        voiceBank.UpdateSettings();
        patch.Load(savedPatch);     // For some reason multitimbral voice modes require loading/updating the patch twice.
        voiceBank.UpdateSettings(); // I need to debug this properly but for now it's working via this workaround.
        effectsEngine.UpdateSettings();
    }

    void KiwiSynth::SavePatch(int bankNumber, int patchNumber)
    {
        PatchHeader oldPatchHeader = patchBanks[bankNumber][patchNumber];
        PatchHeader patchHeader = patch.GetPatchHeader();

        // Update the patch bank header data
        patchBanks[bankNumber][patchNumber] = patchHeader;

        // Remove the element from the patch type vector using erase function and iterators
        auto it = std::find(patchTypes[oldPatchHeader.type].begin(), patchTypes[oldPatchHeader.type].end(),
                            &oldPatchHeader);
        // If element is found found, erase it
        if (it != patchTypes[oldPatchHeader.type].end()) {
            patchTypes[oldPatchHeader.type].erase(it);
        }
        // Add the new element and sort it
        patchTypes[oldPatchHeader.type].push_back(&patchHeader);
        std::sort( patchTypes[oldPatchHeader.type].begin(), patchTypes[oldPatchHeader.type].end(), patchHeaderSort );

        storage.SavePatch(&patch, bankNumber, patchNumber);
        patch.SetLiveMode(false, bankNumber, patchNumber);
        patch.settings1.Copy(patch.voice1Settings);
        patch.settings2.Copy(patch.voice2Settings);
    }

    void KiwiSynth::UpdateSettings()
    {
        voiceBank.UpdateSettings();
        effectsEngine.UpdateSettings();
    }

} // namespace kiwi_synth
