#include "KiwiSynth.h"

namespace kiwi_synth
{
    KiwiSynth::KiwiSynth(DaisySeed* hw, float sampleRate) : hw(hw)
    {
        numVoices = DEFAULT_NUM_VOICES;
        SetMidiChannel(0);

        ConfigureMultiPots();
        patchSettings = new PatchSettings(multiPots);
        multiPots->RegisterControlListener(patchSettings, ControlId::MULTIPOTS);

        voiceBank = new VoiceBank(numVoices, patchSettings, sampleRate);
    }

    KiwiSynth::~KiwiSynth()
    {
        delete voiceBank;
        delete patchSettings;
        delete multiPots;
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
        multiPotsConfig.useTimer = true;
        multiPotsConfig.refreshRate = 50; // One half of a ms, which means 8ms for all pots to refresh. Shorter can cause timing problems.

        multiPots = new MultiPots(hw, &multiPotsConfig);
        multiPots->StartTimer();
    }

    void KiwiSynth::InitMidi()
    {
        MidiUartHandler::Config midi_config;
        midi_config.transport_config.tx = D13;
        midi_config.transport_config.rx = D14;
        midi.Init(midi_config);
        midi.StartReceive();
    }

    void KiwiSynth::SetMidiChannel(int midiChannel)
    {
        this->midiChannel = midiChannel;
    }

    void KiwiSynth::Process(AudioHandle::OutputBuffer out, size_t size)
    {
        ProcessMidi();
        voiceBank->Process(out, size);
    }

    void KiwiSynth::ProcessMidi()
    {
        midi.Listen();
        while(midi.HasEvents())
        {
            MidiEvent event = midi.PopEvent();
            HandleMidiMessage(&event);
        }
    }

    void KiwiSynth::HandleMidiMessage(MidiEvent* midiEvent)
    {
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
                        voiceBank->NoteOn(on.note, on.velocity);
                        break;
                    }
                    break;

                case NoteOff:
                    off = midiEvent->AsNoteOff();
                    voiceBank->NoteOff(off.note, off.velocity);
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

		float val0 = patchSettings->getFloatValue(PatchSetting::VCO_MASTER_TUNE);
		sprintf(buff, "Pot 0-0 value: %.3f", val0);
		hw->PrintLine(buff);

		float val1 = patchSettings->getFloatValue(PatchSetting::VCO_PORTAMENTO_SPEED);
		sprintf(buff, "Pot 0-1 value: %.3f", val1);
		hw->PrintLine(buff);

		float valD0 = patchSettings->getFloatValue(PatchSetting::GEN_BALANCE);
		sprintf(buff, "Pot D0 value: %.3f", valD0);
		hw->PrintLine(buff);
    }
} // namespace kiwi_synth
