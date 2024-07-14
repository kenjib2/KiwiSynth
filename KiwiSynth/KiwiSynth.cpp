#include "KiwiSynth.h"

namespace kiwi_synth
{
    char temp[256];

    void KiwiSynth::Init(DaisySeed* hw, float sampleRate)
    {
        sprintf(temp, "Initializing");
        this->hw = hw;

        numVoices = DEFAULT_NUM_VOICES;
        SetMidiChannel(0);

        ConfigureMultiPots();
        ConfigureGpioExpansion();

        patchSettings.Init(&multiPots, &ge);
        multiPots.RegisterControlListener(&patchSettings, ControlId::MULTIPOTS);
        ge.RegisterControlListener(&patchSettings, ControlId::GPIO_EXPANSION);

        voiceBank.Init(numVoices, NUM_VCOS, &patchSettings, sampleRate);

        InitMidi();
    }

    void KiwiSynth::ConfigureMultiPots()
    {
        int numMps = 3;
        int numChannels = 16;
        int numDirectPots = 1;

        std::vector<Pin> mpPins;
        mpPins.push_back(A0);
        mpPins.push_back(A1);
        mpPins.push_back(A2);

        std::vector<Pin> directPins;
        directPins.push_back(A4);

        MultiPotsConfig multiPotsConfig;
        multiPotsConfig.numMps = numMps;
        multiPotsConfig.numChannels = numChannels;
        multiPotsConfig.numDirectPots = numDirectPots;
        multiPotsConfig.pinA0 = D7;
        multiPotsConfig.pinA1 = D8;
        multiPotsConfig.pinA2 = D9;
        multiPotsConfig.pinA3 = D10;
        multiPotsConfig.pinsSignal = mpPins;
        multiPotsConfig.pinsDirect = directPins;
        //multiPotsConfig.useTimer = true;
        //multiPotsConfig.refreshRate = 50; // One half of a ms, which means 8ms for all pots to refresh. Shorter can cause timing problems.
        // Look into refresh rate. Faster should be possible. Could be either pins left floating or not using the E pin.

        multiPots.Init(hw, &multiPotsConfig);
        //multiPots.StartTimer();
    }

    void KiwiSynth::ConfigureGpioExpansion()
    {
    	ge.Init();
        //ge.StartTimer();
    }

    void KiwiSynth::InitMidi()
    {
        MidiUartHandler::Config midi_config;
        midi_config.transport_config.tx = D13;
        midi_config.transport_config.rx = D14;
        midi.Init(midi_config);
        midi.StartReceive();
        sprintf(temp, "MIDI Started");
    }

    void KiwiSynth::SetMidiChannel(int midiChannel)
    {
        this->midiChannel = midiChannel;
    }

    void KiwiSynth::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
    {
        ProcessMidi();
        voiceBank.Process(out, size);
    }

    void KiwiSynth::ProcessMidi()
    {
        midi.Listen();
        while(midi.HasEvents())
        {
            sprintf(temp, "MIDI Event");
            MidiEvent event = midi.PopEvent();
            HandleMidiMessage(&event);
        }
    }

    void KiwiSynth::ProcessInputs()
    {
        multiPots.Process();
        ge.Process();
    }

    void KiwiSynth::HandleMidiMessage(MidiEvent* midiEvent)
    {
        sprintf(temp, "Handling MIDI");
        if (midiChannel == midiEvent->channel) {
            NoteOnEvent on;
            NoteOffEvent off;
            //ControlChangeEvent controlChange;
            switch(midiEvent->type)
            {
                case NoteOn:
                    on = midiEvent->AsNoteOn();
                    if(midiEvent->data[1] != 0) // This is to avoid Max/MSP Note outs for now..
                    {
                        sprintf(temp, "NoteOn");
                        voiceBank.NoteOn(on.note, on.velocity);
                        break;
                    }
                    break;

                case NoteOff:
                    off = midiEvent->AsNoteOff();
                    sprintf(temp, "NoteOff");
                    voiceBank.NoteOff(off.note, off.velocity);
                    /*for (size_t i = 0; i < voices.size(); i++) {
                        Voice* voice = voices->FindVoice(); //if (voices[i]->noteTriggered && voices[i]->currentMidiNote == off.note) {
                        if (voice) {
                            voice->NoteOff(off.note, off.velocity);
                            voiceBank->RemovePlayingVoice(voice);
                            break;
                        }
                    }*/
                    break;

                // Unimplemented message types
                case ChannelPressure:
                case PolyphonicKeyPressure:
                case ProgramChange:
                case PitchBend:
                case SystemCommon:
                case SystemRealTime:
                case ChannelMode:
                case MessageLast:
                    break;

                case ControlChange:
                    /*controlChange = midiEvent->AsControlChange();
                    intVals[0] = controlChange.control_number;
                    intVals[1] = controlChange.value;
                    midiListeners[i]->Trigger(TriggerMidiControlChange, intVals, NULL);*/
                    break;

                default:
                    break;
            }
        }
    }

    void KiwiSynth::TestOutput()
    {
    	char buff[256];
        sprintf(buff, "----------------------");
		hw->PrintLine(buff);

		hw->PrintLine(temp);
        /*
		float val1 = patchSettings.getFloatValue(PatchSetting::VCO_1_PULSE_WIDTH);
		float val2 = patchSettings.getFloatValue(PatchSetting::VCO_1_LEVEL);
		float val3 = patchSettings.getFloatValue(PatchSetting::VCO_MASTER_TUNE);
		float val4 = patchSettings.getFloatValue(PatchSetting::VCO_PORTAMENTO_SPEED);
        float val5;
        float val6;
		sprintf(buff, "VCO 1 Pulse Width: %.3f   VCO 1 Level: %.3f   VCO Master Tune: %.3f   Port Speed: %.3f", val1, val2, val3, val4);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::VCO_2_PULSE_WIDTH);
		val2 = patchSettings.getFloatValue(PatchSetting::VCO_2_LEVEL);
		val3 = patchSettings.getFloatValue(PatchSetting::VCO_2_FINE_TUNE);
		sprintf(buff, "VCO 2 Pulse Width: %.3f   VCO 2 Level: %.3f   VCO 2 Fine Tune: %.3f", val1, val2, val3);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::VCO_3_PULSE_WIDTH);
		val2 = patchSettings.getFloatValue(PatchSetting::VCO_3_LEVEL);
		val3 = patchSettings.getFloatValue(PatchSetting::VCO_3_FINE_TUNE);
		sprintf(buff, "VCO 3 Pulse Width: %.3f   VCO 3 Level: %.3f   VCO 3 Fine Tune: %.3f", val1, val2, val3);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::VCO_NOISE_LEVEL);
		val2 = patchSettings.getFloatValue(PatchSetting::VCO_EXT_TRIGGER_GATE);
		val3 = patchSettings.getFloatValue(PatchSetting::VCO_EXT_LEVEL);
		sprintf(buff, "VCO Noise Level: %.3f   VCO Trigger Gate: %.3f   VCO External Level: %.3f", val1, val2, val3);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::VCF_CUTOFF);
		val2 = patchSettings.getFloatValue(PatchSetting::VCF_RESONANCE);
		val3 = patchSettings.getFloatValue(PatchSetting::VCF_TRACKING);
		val4 = patchSettings.getFloatValue(PatchSetting::VCF_ENV_1_DEPTH);
		val5 = patchSettings.getFloatValue(PatchSetting::VCF_ENV_2_DEPTH);
		sprintf(buff, "VCF Cutoff: %.3f   VCF Resonance: %.3f   VCF Tracking: %.3f   VCF Env 1 Depth: %.3f   VCF Env 2 Depth: %.3f", val1, val2, val3, val4, val5);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::VCA_LEVEL);
		val2 = patchSettings.getFloatValue(PatchSetting::VCA_ENV_1_DEPTH);
		val3 = patchSettings.getFloatValue(PatchSetting::SH_LEVEL);
		val4 = patchSettings.getFloatValue(PatchSetting::SH_RATE);
		val5 = patchSettings.getFloatValue(PatchSetting::GEN_BALANCE);
		sprintf(buff, "VCA Level: %.3f   VCA Env 1 Depth: %.3f   S&H Level: %.3f   S&H Rate: %.3f   Balance: %.3f", val1, val2, val3, val4, val5);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::FX_1);
		val2 = patchSettings.getFloatValue(PatchSetting::FX_2);
		val3 = patchSettings.getFloatValue(PatchSetting::FX_3);
		val4 = patchSettings.getFloatValue(PatchSetting::FX_4);
		val5 = patchSettings.getFloatValue(PatchSetting::FX_5);
		val6 = patchSettings.getFloatValue(PatchSetting::FX_REVERB);
		sprintf(buff, "FX 1: %.3f   FX 2: %.3f   FX 3: %.3f   FX 4: %.3f   FX 5: %.3f   Reverb Level: %.3f", val1, val2, val3, val4, val5, val6);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::MOD_1_DEPTH);
		val2 = patchSettings.getFloatValue(PatchSetting::MOD_2_DEPTH);
		val3 = patchSettings.getFloatValue(PatchSetting::MOD_3_DEPTH);
		val4 = patchSettings.getFloatValue(PatchSetting::MOD_4_DEPTH);
		sprintf(buff, "Mod 1 Depth: %.3f   Mod 2 Depth: %.3f   Mod 3 Depth: %.3f   Mod 4 Depth: %.3f", val1, val2, val3, val4);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::LFO_1_PULSE_WIDTH);
		val2 = patchSettings.getFloatValue(PatchSetting::LFO_1_RATE);
		val3 = patchSettings.getFloatValue(PatchSetting::LFO_1_TRIGGER_PHASE);
		val4 = patchSettings.getFloatValue(PatchSetting::LFO_1_TO_MASTER_TUNE);
		sprintf(buff, "LFO 1 PW: %.3f   LFO 1 Rate: %.3f   LFO 1 Trigger Phase: %.3f   LFO 1 to Master Tune: %.3f", val1, val2, val3, val4);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::LFO_2_PULSE_WIDTH);
		val2 = patchSettings.getFloatValue(PatchSetting::LFO_2_RATE);
		val3 = patchSettings.getFloatValue(PatchSetting::LFO_2_TRIGGER_PHASE);
		val4 = patchSettings.getFloatValue(PatchSetting::LFO_2_TO_VCF_CUTOFF);
		sprintf(buff, "LFO 2 PW: %.3f   LFO 2 Rate: %.3f   LFO 2 Trigger Phase: %.3f   LFO 2 to Master Tune: %.3f", val1, val2, val3, val4);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::ENV_1_ATTACK);
		val2 = patchSettings.getFloatValue(PatchSetting::ENV_1_DECAY);
		val3 = patchSettings.getFloatValue(PatchSetting::ENV_1_SUSTAIN);
		val4 = patchSettings.getFloatValue(PatchSetting::ENV_1_RELEASE);
		sprintf(buff, "Env 1 Attack: %.3f   Env 1 Decay: %.3f   Env 1 Sustain: %.3f   Env 1 Release: %.3f", val1, val2, val3, val4);
		hw->PrintLine(buff);

		val1 = patchSettings.getFloatValue(PatchSetting::ENV_2_ATTACK);
		val2 = patchSettings.getFloatValue(PatchSetting::ENV_2_DECAY);
		val3 = patchSettings.getFloatValue(PatchSetting::ENV_2_SUSTAIN);
		val4 = patchSettings.getFloatValue(PatchSetting::ENV_2_RELEASE);
		sprintf(buff, "Env 2 Attack: %.3f   Env 2 Decay: %.3f   Env 2 Sustain: %.3f   Env 2 Release: %.3f", val1, val2, val3, val4);
		hw->PrintLine(buff);
        

        sprintf(buff, "----------------------");
		hw->PrintLine(buff);

        sprintf(buff, "1: %d   2: %d   3: %d   4: %d", ge.getPinValues(0x20), ge.getPinValues(0x21), ge.getPinValues(0x22), ge.getPinValues(0x23));
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
		hw->PrintLine(buff);*/
    }
} // namespace kiwi_synth
