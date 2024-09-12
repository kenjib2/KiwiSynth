#include "KiwiSynth.h"

namespace kiwi_synth
{
    #ifdef __PATCH_SETTINGS__
    char DSY_SDRAM_BSS buff[512];
    #endif // __PATCH_SETTINGS__

    void KiwiSynth::Init(DaisySeed* hw, float sampleRate)
    {
        this->hw = hw;
        numVoices = MAX_VOICES;
        SetMidiChannel(0);

        ConfigureMultiPots(hw);
        ConfigureGpioExpansion();

        //patch.Init(&multiPots, &ge);
        patchSettings.Init(&multiPots, &ge);
        multiPots.RegisterControlListener(&patchSettings);
        ge.RegisterControlListener(&patchSettings);

        voiceBank.Init(numVoices, NUM_VCOS, &patchSettings, sampleRate);
        effectsEngine.Init(&patchSettings, sampleRate);

        InitMidi();
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

    void KiwiSynth::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
    {
        ProcessMidi();
        midiLEDOn = (midiCounter < MIDI_LED_DURATION);
        midiCounter++;

        for(size_t i = 0; i < size; i += 2)
        {
            voiceBank.Process(&out[i]);

            #ifdef __FUNCTIONALITY_OPTION__
            if (voiceBank.numVoices == MAX_VOICES) {
                effectsEngine.ProcessReverbOnly(&(out[i]));
            } else {
            #endif // __FUNCTIONALITY_OPTION__
                effectsEngine.Process(&(out[i]));
            #ifdef __FUNCTIONALITY_OPTION__
            }
            #endif // __FUNCTIONALITY_OPTION__
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

        patchSettings.setValue(GEN_SUSTAIN, gpioSustain.Read() ? 0.0f : 1.0f);
        patchSettings.setValue(GEN_EXPRESSION, multiPots.GetDirectValue(1));
    }

    void KiwiSynth::ProcessInputs()
    {
        if (midiLEDOn != midiLEDState) {
            midiLEDState = midiLEDOn;
            gpioMidiActivity.Write(midiLEDOn);
        }
        multiPots.Process();
        ge.Process();
    }

    void KiwiSynth::HandleMidiMessage(MidiEvent* midiEvent)
    {
        if (midiChannel == midiEvent->channel) {
            NoteOnEvent on;
            NoteOffEvent off;
            ControlChangeEvent cc;
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
                    patchSettings.setValue(GEN_PITCH_BEND, (float)midiEvent->AsPitchBend().value / 8192.0f);
                    break;

                // Unimplemented message types
                case ChannelPressure:
                    midiCounter = 0;
                    patchSettings.setValue(GEN_AFTERTOUCH, (float)midiEvent->AsChannelPressure().pressure / 127.0f);
                    break;

                case ControlChange:
                    cc = midiEvent->AsControlChange();
                    switch (cc.control_number) {
                        case 1:  // Mod Wheel
                            midiCounter = 0;
                            patchSettings.setValue(GEN_MOD_WHEEL, (float)cc.value / 127.0f);
                            break;

                        case 11: // Expression
                            midiCounter = 0;
                            patchSettings.setValue(GEN_EXPRESSION, (float)cc.value / 127.0f);
                            break;

                        case 64: // Sustain Pedal
                            midiCounter = 0;
                            if (cc.value < 64) {
                                patchSettings.setValue(GEN_SUSTAIN, 0.0f); // Off
                            } else {
                                patchSettings.setValue(GEN_SUSTAIN, 1.0f);  // On
                            }
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
        return patchSettings.getBoolValue(PatchSetting::GEN_SELECT_BUTTON);
    }

    void KiwiSynth::UpdateSettings()
    {
        balance = patchSettings.getFloatValue(PatchSetting::GEN_BALANCE);
        voiceBank.UpdateSettings();
        effectsEngine.UpdateSettings();
    }

    #ifdef __PATCH_SETTINGS__
    void KiwiSynth::TestOutput(DaisySeed* hw)
    {
        sprintf(buff, "----------------------");
        hw->PrintLine(buff);

        int val1 = patchSettings.getFloatValue(PatchSetting::VCO_1_PULSE_WIDTH) * 1000;
        int val2 = patchSettings.getFloatValue(PatchSetting::VCO_1_LEVEL) * 1000;
        int val3 = patchSettings.getFloatValue(PatchSetting::VCO_MASTER_TUNE) * 1000;
        int val4 = patchSettings.getFloatValue(PatchSetting::VCO_PORTAMENTO_SPEED) * 1000;
        int val5;
        int val6;
        sprintf(buff, "VCO 1 Pulse Width: %d   VCO 1 Level: %d   VCO Master Tune: %d   Port Speed: %d", val1, val2, val3, val4);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::VCO_2_PULSE_WIDTH) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::VCO_2_LEVEL) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::VCO_2_FINE_TUNE) * 1000;
        sprintf(buff, "VCO 2 Pulse Width: %d   VCO 2 Level: %d   VCO 2 Fine Tune: %d", val1, val2, val3);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::VCO_3_PULSE_WIDTH) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::VCO_3_LEVEL) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::VCO_3_FINE_TUNE) * 1000;
        sprintf(buff, "VCO 3 Pulse Width: %d   VCO 3 Level: %d   VCO 3 Fine Tune: %d", val1, val2, val3);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::VCO_NOISE_LEVEL) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::VCO_EXT_TRIGGER_GATE) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::VCO_EXT_LEVEL) * 1000;
        sprintf(buff, "VCO Noise Level: %d   VCO Trigger Gate: %d   VCO External Level: %d", val1, val2, val3);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::VCF_CUTOFF) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::VCF_RESONANCE) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::VCF_TRACKING) * 1000;
        val4 = patchSettings.getFloatValue(PatchSetting::VCF_ENV_1_DEPTH) * 1000;
        val5 = patchSettings.getFloatValue(PatchSetting::VCF_ENV_2_DEPTH) * 1000;
        sprintf(buff, "VCF Cutoff: %d   VCF Resonance: %d   VCF Tracking: %d   VCF Env 1 Depth: %d   VCF Env 2 Depth: %d", val1, val2, val3, val4, val5);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::VCA_LEVEL) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::VCA_ENV_1_DEPTH) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::SH_TO_VCF_CUTOFF) * 1000;
        val4 = patchSettings.getFloatValue(PatchSetting::SH_RATE) * 1000;
        val5 = patchSettings.getFloatValue(PatchSetting::GEN_BALANCE) * 1000;
        sprintf(buff, "VCA Level: %d   VCA Env 1 Depth: %d   S&H Level: %d   S&H Rate: %d   Balance: %d", val1, val2, val3, val4, val5);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::FX_1) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::FX_2) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::FX_3) * 1000;
        val4 = patchSettings.getFloatValue(PatchSetting::FX_4) * 1000;
        val5 = patchSettings.getFloatValue(PatchSetting::FX_5) * 1000;
        val6 = patchSettings.getFloatValue(PatchSetting::FX_REVERB) * 1000;
        sprintf(buff, "FX 1: %d   FX 2: %d   FX 3: %d   FX 4: %d   FX 5: %d   Reverb Level: %d", val1, val2, val3, val4, val5, val6);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::MOD_1_DEPTH) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::MOD_2_DEPTH) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::MOD_3_DEPTH) * 1000;
        val4 = patchSettings.getFloatValue(PatchSetting::MOD_4_DEPTH) * 1000;
        sprintf(buff, "Mod 1 Depth: %d   Mod 2 Depth: %d   Mod 3 Depth: %d   Mod 4 Depth: %d", val1, val2, val3, val4);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::LFO_1_PULSE_WIDTH) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::LFO_1_RATE) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::LFO_1_TRIGGER_PHASE) * 1000;
        val4 = patchSettings.getFloatValue(PatchSetting::LFO_1_TO_MASTER_TUNE) * 1000;
        sprintf(buff, "LFO 1 PW: %d   LFO 1 Rate: %d   LFO 1 Trigger Phase: %d   LFO 1 to Master Tune: %d", val1, val2, val3, val4);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::LFO_2_PULSE_WIDTH) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::LFO_2_RATE) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::LFO_2_TRIGGER_PHASE) * 1000;
        val4 = patchSettings.getFloatValue(PatchSetting::LFO_2_TO_VCF_CUTOFF) * 1000;
        sprintf(buff, "LFO 2 PW: %d   LFO 2 Rate: %d   LFO 2 Trigger Phase: %d   LFO 2 to Master Tune: %d", val1, val2, val3, val4);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::ENV_1_ATTACK) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::ENV_1_DECAY) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::ENV_1_SUSTAIN) * 1000;
        val4 = patchSettings.getFloatValue(PatchSetting::ENV_1_RELEASE) * 1000;
        sprintf(buff, "Env 1 Attack: %d   Env 1 Decay: %d   Env 1 Sustain: %d   Env 1 Release: %d", val1, val2, val3, val4);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::ENV_2_ATTACK) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::ENV_2_DECAY) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::ENV_2_SUSTAIN) * 1000;
        val4 = patchSettings.getFloatValue(PatchSetting::ENV_2_RELEASE) * 1000;
        sprintf(buff, "Env 2 Attack: %d   Env 2 Decay: %d   Env 2 Sustain: %d   Env 2 Release: %d", val1, val2, val3, val4);
        hw->PrintLine(buff);
        

        sprintf(buff, "----------------------");
        hw->PrintLine(buff);

        bool bool1 = patchSettings.getBoolValue(PatchSetting::VCO_2_ON);
        bool bool2 = patchSettings.getBoolValue(PatchSetting::VCO_3_ON);
        bool bool3 = patchSettings.getBoolValue(PatchSetting::VCO_NOISE_ON);
        bool bool4 = patchSettings.getBoolValue(PatchSetting::VCO_EXT_ON);
        bool bool5 = patchSettings.getBoolValue(PatchSetting::VCO_PORTAMENTO_ON);
        sprintf(buff, "VCO 2: %d   VCO 3: %d   Noise: %d   External: %d   Portamento: %d", bool1, bool2, bool3, bool4, bool5);
        hw->PrintLine(buff);

        bool1 = patchSettings.getBoolValue(PatchSetting::LFO_1_TRIGGER_RESET_ON);
    
        bool2 = patchSettings.getBoolValue(PatchSetting::LFO_2_TRIGGER_RESET_ON);
        bool3 = patchSettings.getBoolValue(PatchSetting::ENV_1_REVERSE_PHASE_ON);
        bool4 = patchSettings.getBoolValue(PatchSetting::ENV_2_REVERSE_PHASE_ON);
        bool5 = patchSettings.getBoolValue(PatchSetting::GEN_SELECT_BUTTON);
        sprintf(buff, "LFO 1 Rst: %d   LFO 2 Rst: %d   ENV 1 Phase: %d   ENV 2 Phase: %d   Select btn: %d", bool1, bool2, bool3, bool4, bool5);
        hw->PrintLine(buff);


        sprintf(buff, "----------------------");
        hw->PrintLine(buff);

        val1 = ge.getPinValues(0x20);
        val2 = ge.getPinValues(0x21);
        val3 = ge.getPinValues(0x22);
        val4 = ge.getPinValues(0x23);
        sprintf(buff, "0x20: %d   0x21: %d   0x22: %d   0x23: %d", val1, val2, val3, val4);
        hw->PrintLine(buff);

        val1 = patchSettings.getIntValue(PatchSetting::VCO_VOICES);
        val2 = patchSettings.getIntValue(PatchSetting::VCO_1_WAVEFORM);
        val3 = patchSettings.getIntValue(PatchSetting::VCO_2_WAVEFORM);
        val4 = patchSettings.getIntValue(PatchSetting::VCO_2_OCTAVE);
        val5 = patchSettings.getIntValue(PatchSetting::VCO_2_INTERVAL);
        val6 = patchSettings.getIntValue(PatchSetting::VCO_3_WAVEFORM);
        sprintf(buff, "VCO Vc: %d,  VCO 1 Wv: %d,  VCO 2 Wv: %d,  VCO 2 Oct: %d,  VCO 2 Int: %d,  VCO 3 Wv: %d", val1, val2, val3, val4, val5, val6);
        hw->PrintLine(buff);

        val1 = patchSettings.getIntValue(PatchSetting::VCO_3_OCTAVE);
        val2 = patchSettings.getIntValue(PatchSetting::VCO_3_INTERVAL);
        val3 = patchSettings.getIntValue(PatchSetting::VCO_NOISE_TYPE);
        val4 = patchSettings.getIntValue(PatchSetting::VCF_FILTER_TYPE);
        val5 = patchSettings.getIntValue(PatchSetting::LFO_1_WAVEFORM);
        val6 = patchSettings.getIntValue(PatchSetting::LFO_2_WAVEFORM);
        sprintf(buff, "VCO 3 Oct: %d,  VCO 3 Int: %d,  VCO Noise: %d,  VCF Type: %d,  LFO 1 Wv: %d,  LFO 2 Wv: %d", val1, val2, val3, val4, val5, val6);
        hw->PrintLine(buff);

        val1 = patchSettings.getIntValue(PatchSetting::MOD_1_SOURCE);
        val2 = patchSettings.getIntValue(PatchSetting::MOD_1_DESTINATION);
        val3 = patchSettings.getIntValue(PatchSetting::MOD_2_SOURCE);
        val4 = patchSettings.getIntValue(PatchSetting::MOD_2_DESTINATION);
        val5 = patchSettings.getIntValue(PatchSetting::MOD_3_SOURCE);
        val6 = patchSettings.getIntValue(PatchSetting::MOD_3_DESTINATION);
        sprintf(buff, "MOD 1 S: %d,  MOD 1 D: %d,  MOD 2 S: %d,  MOD 2 D: %d,  MOD 3 S: %d,  MOD 3 D: %d", val1, val2, val3, val4, val5, val6);
        hw->PrintLine(buff);

        val1 = patchSettings.getIntValue(PatchSetting::MOD_4_SOURCE);
        val2 = patchSettings.getIntValue(PatchSetting::MOD_4_DESTINATION);
        val3 = patchSettings.getIntValue(PatchSetting::GEN_SELECT);
        sprintf(buff, "MOD 4 S: %d,  MOD 4 D: %d,  GEN Sel: %d", val1, val2, val3);
        hw->PrintLine(buff);

        val1 = patchSettings.getFloatValue(PatchSetting::GEN_AFTERTOUCH) * 1000;
        val2 = patchSettings.getFloatValue(PatchSetting::GEN_MOD_WHEEL) * 1000;
        val3 = patchSettings.getFloatValue(PatchSetting::GEN_PITCH_BEND) * 1000;
        val4 = patchSettings.getFloatValue(PatchSetting::GEN_EXPRESSION) * 1000;
        val5 = patchSettings.getFloatValue(PatchSetting::GEN_SUSTAIN) * 1000;
        sprintf(buff, "ATouch: %d,  MWheel: %d,  PBend: %d,  Expr: %d,  Sus: %d", val1, val2, val3, val4, val5);
        hw->PrintLine(buff);

        val1 = patchSettings.lastChangedSetting;
        val2 = patchSettings.lastChangedValue * 1000;
        sprintf(buff, "Last Changed Setting: %d,  Last Changed Value: %d", val1, val2);
        hw->PrintLine(buff);
    }
    #endif // __PATCH_SETTINGS__
} // namespace kiwi_synth
