#include "VoiceBank.h"

namespace kiwi_synth
{
    void VoiceBank::Init(uint8_t maxVoices, uint8_t numVcos, Patch* patch, float sampleRate) {
        #ifdef __FUNCTIONALITY_OPTION__
        modulations = new Modulation*[3];
        modulations[2] = new Modulation[NUM_MODULATIONS];
        #else
        modulations = new Modulation*[2];
        #endif // __FUNCTIONALITY_OPTION__
        modulations[0] = new Modulation[NUM_MODULATIONS];
        modulations[1] = new Modulation[NUM_MODULATIONS];
        voiceMode = 255;
        InitModulations();
        for (int i = 0; i < maxVoices; i++) {
            voices[i].numVcos = voices[i].maxVcos;
        }

        this->patch = patch;
        this->maxVoices = maxVoices;
        this->numVoices = maxVoices;
        for (uint8_t i = 0; i < maxVoices; i++)
        {
            Voice nextVoice;
            nextVoice.Init(numVcos, patch->getActiveSettings(), sampleRate, i);
            voices.push_back(nextVoice);
        }
    }

    
    void VoiceBank::UpdateSettings()
    {
        int8_t newVoiceMode = patch->getActiveSettings()->getIntValue(PatchSetting::VCO_VOICES);
        if (newVoiceMode != voiceMode) {
            AllNotesOff();
            voiceMode = newVoiceMode;

            switch (voiceMode)
            {
                case VOICE_MODE_POLY:
                default:
                    #ifdef __FUNCTIONALITY_OPTION__
                    for (int i = 0; i < maxVoices; i++) {
                        voices[i].fullFunctionality = true;
                        voices[i].numVcos = voices[i].maxVcos;
                    }
                    #endif // __FUNCTIONALITY_OPTION__
                    patch->SetActivePatchSettings(0);
                    numVoices = 2;
                    break;
                case VOICE_MODE_MONO:
                    #ifdef __FUNCTIONALITY_OPTION__
                    for (int i = 0; i < maxVoices; i++) {
                        voices[i].fullFunctionality = true;
                        voices[i].numVcos = voices[i].maxVcos;
                    }
                    #endif // __FUNCTIONALITY_OPTION__
                    patch->SetActivePatchSettings(0);
                    numVoices = 1;
                    break;
                case VOICE_MODE_MULTI:
                    #ifdef __FUNCTIONALITY_OPTION__
                    for (int i = 0; i < maxVoices; i++) {
                        voices[i].fullFunctionality = true;
                        voices[i].numVcos = voices[i].maxVcos;
                    }
                    #endif // __FUNCTIONALITY_OPTION__
                    patch->SetActivePatchSettings(1);
                    numVoices = 2;
                    break;
                #ifdef __FUNCTIONALITY_OPTION__
                case VOICE_MODE_3V:
                        for (int i = 0; i < maxVoices; i++) {
                            voices[i].fullFunctionality = false;
                            voices[i].numVcos = 2;
                        }
                        patch->SetActivePatchSettings(0);
                        numVoices = maxVoices;
                    break;
                #endif // __FUNCTIONALITY_OPTION__
            }
        }

        UpdateModulations();

        switch (voiceMode)
        {
            case VOICE_MODE_POLY:
            case VOICE_MODE_MONO:
            default:
                voices[0].UpdateSettings(patch->getActiveSettings());
                voices[1].UpdateSettings(patch->getActiveSettings());
                break;
            #ifdef __FUNCTIONALITY_OPTION__
            case VOICE_MODE_3V:
                voices[0].UpdateSettings(patch->getActiveSettings());
                voices[1].UpdateSettings(patch->getActiveSettings());
                voices[2].UpdateSettings(patch->getActiveSettings());
                break;
            #endif // __FUNCTIONALITY_OPTION__
            case VOICE_MODE_MULTI:
                //voices[0].UpdateSettings(patch->getActiveSettings());
                voices[1].UpdateSettings(patch->getActiveSettings());
                break;
        }
    }

    void VoiceBank::Process(float* sample)
    {
        sample[0] = 0.0f;
        sample[1] = 0.0f;

        float nextVoice[2];

        switch (voiceMode)
        {
            case VOICE_MODE_POLY:
            #ifdef __FUNCTIONALITY_OPTION__
            case VOICE_MODE_3V:
            #endif // __FUNCTIONALITY_OPTION__
            default:
                voices[0].Process(nextVoice, patch->getActiveSettings(), modulations[0], numVoices);
                sample[0] += nextVoice[0];
                sample[1] += nextVoice[1];
    
                voices[1].Process(nextVoice, patch->getActiveSettings(), modulations[0], numVoices);
                sample[0] += nextVoice[0];
                sample[1] += nextVoice[1];
    
                #ifdef __FUNCTIONALITY_OPTION__
                voices[2].Process(nextVoice, patch->getActiveSettings(), modulations[0], numVoices);
                sample[0] += nextVoice[0];
                sample[1] += nextVoice[1];
                #endif // __FUNCTIONALITY_OPTION__
                
                break;
            case VOICE_MODE_MONO:
                voices[0].Process(nextVoice, patch->getActiveSettings(), modulations[0], numVoices);
                sample[0] += nextVoice[0];
                sample[1] += nextVoice[1];
    
                break;
            case VOICE_MODE_MULTI:
                voices[0].Process(nextVoice, patch->getVoice1Settings(), modulations[0], numVoices);
                sample[0] += nextVoice[0];
                sample[1] += nextVoice[1];
    
                voices[1].Process(nextVoice, patch->getActiveSettings(), modulations[1], numVoices);
                sample[0] += nextVoice[0];
                sample[1] += nextVoice[1];

                break;
        }
    }

    void VoiceBank::InitModulations() {
        #ifdef __FUNCTIONALITY_OPTION__
        for (int i = 0; i < 3; i++) {
        #else
        for (int i = 0; i < 2; i++) {
        #endif // __FUNCTIONALITY_OPTION__
            modulations[i][MODS_MOD_MATRIX_1].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_1].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_1].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_2].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_2].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_2].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_3].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_3].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_3].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_4].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_4].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_4].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_5].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_5].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_5].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_6].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_6].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_6].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_7].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_7].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_7].depth = 0.0f;
            modulations[i][MODS_MOD_MATRIX_8].source = SRC_NONE;
            modulations[i][MODS_MOD_MATRIX_8].destination = DST_NONE;
            modulations[i][MODS_MOD_MATRIX_8].depth = 0.0f;

            modulations[i][MODS_LFO_1_TO_VCOS].source = SRC_LFO_1;
            modulations[i][MODS_LFO_1_TO_VCOS].destination = DST_VCOS_FREQ;
            modulations[i][MODS_LFO_1_TO_VCOS].depth = 0.0f;
            modulations[i][MODS_ENV_1_TO_VCA].source = SRC_ENV_1;
            modulations[i][MODS_ENV_1_TO_VCA].destination = DST_VCA_LEVEL;
            modulations[i][MODS_ENV_1_TO_VCA].depth = 0.0f;
            modulations[i][MODS_NOTE_TO_VCF_FREQ].source = SRC_NOTE;
            modulations[i][MODS_NOTE_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_NOTE_TO_VCF_FREQ].depth = 0.0f;
            modulations[i][MODS_ENV_1_TO_VCF_FREQ].source = SRC_ENV_1;
            modulations[i][MODS_ENV_1_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_ENV_1_TO_VCF_FREQ].depth = 0.0f;
            modulations[i][MODS_ENV_2_TO_VCF_FREQ].source = SRC_ENV_2;
            modulations[i][MODS_ENV_2_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_ENV_2_TO_VCF_FREQ].depth = 0.0f;
            modulations[i][MODS_LFO_2_TO_VCF_FREQ].source = SRC_LFO_2;
            modulations[i][MODS_LFO_2_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_LFO_2_TO_VCF_FREQ].depth = 0.0f;
            modulations[i][MODS_SH_TO_VCF_FREQ].source = SRC_SH;
            modulations[i][MODS_SH_TO_VCF_FREQ].destination = DST_VCF_CUTOFF;
            modulations[i][MODS_SH_TO_VCF_FREQ].depth = 0.0f;
        }
    }

    void VoiceBank::UpdateModulations() {
        modulations[0][MODS_MOD_MATRIX_1].source = (ModulationSource)patch->getActiveSettings()->getIntValue(MOD_1_SOURCE);
        modulations[0][MODS_MOD_MATRIX_1].destination = (ModulationDestination)patch->getActiveSettings()->getIntValue(MOD_1_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_1].depth = patch->getActiveSettings()->getFloatValue(MOD_1_DEPTH);
        modulations[0][MODS_MOD_MATRIX_2].source = (ModulationSource)patch->getActiveSettings()->getIntValue(MOD_2_SOURCE);
        modulations[0][MODS_MOD_MATRIX_2].destination = (ModulationDestination)patch->getActiveSettings()->getIntValue(MOD_2_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_2].depth = patch->getActiveSettings()->getFloatValue(MOD_2_DEPTH);
        modulations[0][MODS_MOD_MATRIX_3].source = (ModulationSource)patch->getActiveSettings()->getIntValue(MOD_3_SOURCE);
        modulations[0][MODS_MOD_MATRIX_3].destination = (ModulationDestination)patch->getActiveSettings()->getIntValue(MOD_3_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_3].depth = patch->getActiveSettings()->getFloatValue(MOD_3_DEPTH);
        modulations[0][MODS_MOD_MATRIX_4].source = (ModulationSource)patch->getActiveSettings()->getIntValue(MOD_4_SOURCE);
        modulations[0][MODS_MOD_MATRIX_4].destination = (ModulationDestination)patch->getActiveSettings()->getIntValue(MOD_4_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_4].depth = patch->getActiveSettings()->getFloatValue(MOD_4_DEPTH);
        modulations[0][MODS_MOD_MATRIX_5].source = (ModulationSource)patch->getActiveSettings()->getIntValue(MOD_5_SOURCE);
        modulations[0][MODS_MOD_MATRIX_5].destination = (ModulationDestination)patch->getActiveSettings()->getIntValue(MOD_5_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_5].depth = patch->getActiveSettings()->getFloatValue(MOD_5_DEPTH);
        modulations[0][MODS_MOD_MATRIX_6].source = (ModulationSource)patch->getActiveSettings()->getIntValue(MOD_6_SOURCE);
        modulations[0][MODS_MOD_MATRIX_6].destination = (ModulationDestination)patch->getActiveSettings()->getIntValue(MOD_6_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_6].depth = patch->getActiveSettings()->getFloatValue(MOD_6_DEPTH);
        modulations[0][MODS_MOD_MATRIX_7].source = (ModulationSource)patch->getActiveSettings()->getIntValue(MOD_7_SOURCE);
        modulations[0][MODS_MOD_MATRIX_7].destination = (ModulationDestination)patch->getActiveSettings()->getIntValue(MOD_7_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_7].depth = patch->getActiveSettings()->getFloatValue(MOD_7_DEPTH);
        modulations[0][MODS_MOD_MATRIX_8].source = (ModulationSource)patch->getActiveSettings()->getIntValue(MOD_8_SOURCE);
        modulations[0][MODS_MOD_MATRIX_8].destination = (ModulationDestination)patch->getActiveSettings()->getIntValue(MOD_8_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_8].depth = patch->getActiveSettings()->getFloatValue(MOD_8_DEPTH);
        memcpy(modulations[1], modulations[0], sizeof(Modulations) * 8);
        #ifdef __FUNCTIONALITY_OPTION__
        memcpy(modulations[2], modulations[0], sizeof(Modulations) * 8);
        #endif // __FUNCTIONALITY_OPTION__

        modulations[0][MODS_LFO_1_TO_VCOS].depth = patch->getVoice1Settings()->getFloatValueExponential(LFO_1_TO_MASTER_TUNE);
        modulations[0][MODS_ENV_1_TO_VCA].depth = patch->getVoice1Settings()->getFloatValue(VCA_ENV_1_DEPTH);
        modulations[0][MODS_NOTE_TO_VCF_FREQ].depth = patch->getVoice1Settings()->getFloatValue(VCF_TRACKING);
        modulations[0][MODS_ENV_1_TO_VCF_FREQ].depth = patch->getVoice1Settings()->getFloatValueExponential(VCF_ENV_1_DEPTH);
        modulations[0][MODS_ENV_2_TO_VCF_FREQ].depth = patch->getVoice1Settings()->getFloatValueExponential(VCF_ENV_2_DEPTH);
        modulations[0][MODS_LFO_2_TO_VCF_FREQ].depth = patch->getVoice1Settings()->getFloatValueExponential(LFO_2_TO_VCF_CUTOFF);
        modulations[0][MODS_SH_TO_VCF_FREQ].depth = patch->getVoice1Settings()->getFloatValue(SH_TO_VCF_CUTOFF);

        modulations[1][MODS_LFO_1_TO_VCOS].depth = patch->getVoice2Settings()->getFloatValueExponential(LFO_1_TO_MASTER_TUNE);
        modulations[1][MODS_ENV_1_TO_VCA].depth = patch->getVoice2Settings()->getFloatValue(VCA_ENV_1_DEPTH);
        modulations[1][MODS_NOTE_TO_VCF_FREQ].depth = patch->getVoice2Settings()->getFloatValue(VCF_TRACKING);
        modulations[1][MODS_ENV_1_TO_VCF_FREQ].depth = patch->getVoice2Settings()->getFloatValueExponential(VCF_ENV_1_DEPTH);
        modulations[1][MODS_ENV_2_TO_VCF_FREQ].depth = patch->getVoice2Settings()->getFloatValueExponential(VCF_ENV_2_DEPTH);
        modulations[1][MODS_LFO_2_TO_VCF_FREQ].depth = patch->getVoice2Settings()->getFloatValueExponential(LFO_2_TO_VCF_CUTOFF);
        modulations[1][MODS_SH_TO_VCF_FREQ].depth = patch->getVoice2Settings()->getFloatValue(SH_TO_VCF_CUTOFF);

        #ifdef __FUNCTIONALITY_OPTION__
        modulations[2][MODS_LFO_1_TO_VCOS].depth = patch->getVoice3Settings()->getFloatValueExponential(LFO_1_TO_MASTER_TUNE);
        modulations[2][MODS_ENV_1_TO_VCA].depth = patch->getVoice3Settings()->getFloatValue(VCA_ENV_1_DEPTH);
        modulations[2][MODS_NOTE_TO_VCF_FREQ].depth = patch->getVoice3Settings()->getFloatValue(VCF_TRACKING);
        modulations[2][MODS_ENV_1_TO_VCF_FREQ].depth = patch->getVoice3Settings()->getFloatValueExponential(VCF_ENV_1_DEPTH);
        modulations[2][MODS_ENV_2_TO_VCF_FREQ].depth = patch->getVoice3Settings()->getFloatValueExponential(VCF_ENV_2_DEPTH);
        modulations[2][MODS_LFO_2_TO_VCF_FREQ].depth = patch->getVoice3Settings()->getFloatValueExponential(LFO_2_TO_VCF_CUTOFF);
        modulations[2][MODS_SH_TO_VCF_FREQ].depth = patch->getVoice3Settings()->getFloatValue(SH_TO_VCF_CUTOFF);
        #endif // __FUNCTIONALITY_OPTION__
    }

    void VoiceBank::NoteOn(uint8_t note, uint8_t velocity)
    {
        switch (voiceMode) {
            case VOICE_MODE_MONO:
                voices[1].NoteOff(note, velocity); // Kill any voice 2 notes just in case
                monoNotes.push_back(note);
                if (voices[0].noteTriggered) {
                    voices[0].NoteOn(note, velocity, false);
                } else {
                    voices[0].NoteOn(note, velocity);
                }
                break;
            case VOICE_MODE_MULTI:
                monoNotes.push_back(note);
                if (voices[0].noteTriggered) {
                    voices[0].NoteOn(note, velocity, false);
                    voices[1].NoteOn(note, velocity, false);
                } else {
                    voices[0].NoteOn(note, velocity);
                    voices[1].NoteOn(note, velocity);
                }
                break;
            default:
                Voice* voice = RequestVoice(note);
                if (voice != NULL) {
                    voice->NoteOn(note, velocity);
                }
                break;
        }
    }

    void VoiceBank::NoteOff(uint8_t note, uint8_t velocity)
    {
        switch (voiceMode) {
            case VOICE_MODE_MONO:
                voices[1].NoteOff(note, velocity); // Kill any voice 2 notes just in case
                if (note == monoNotes.back()) {
                    monoNotes.pop_back();
                    if (monoNotes.size() > 0) {
                        int prevNote = monoNotes.back();
                        voices[0].NoteOn(prevNote, velocity, false);
                    } else {
                        voices[0].NoteOff(note, velocity);
                    }
                } else {
                    for(unsigned int i = 0; i < monoNotes.size(); i++) {
                        if (monoNotes[i] == note) {
                            monoNotes.erase(monoNotes.begin() + i);
                            break;
                        }
                    }
                }
                break;
            case VOICE_MODE_MULTI:
                if (note == monoNotes.back()) {
                    monoNotes.pop_back();
                    if (monoNotes.size() > 0) {
                        int prevNote = monoNotes.back();
                        voices[0].NoteOn(prevNote, velocity, false);
                        voices[1].NoteOn(prevNote, velocity, false);
                    } else {
                        voices[0].NoteOff(note, velocity);
                        voices[1].NoteOff(note, velocity);
                    }
                } else {
                    for(unsigned int i = 0; i < monoNotes.size(); i++) {
                        if (monoNotes[i] == note) {
                            monoNotes.erase(monoNotes.begin() + i);
                            break;
                        }
                    }
                }
                break;
            default:
                for (size_t i = 0; i < voices.size(); i++) {
                    if (voices[i].noteTriggered && voices[i].currentMidiNote == note) {
                        voices[i].NoteOff(note, velocity);
                        RemovePlayingVoice(i);
                        break;
                    }
                }
                break;
        }
    }

    void VoiceBank::AllNotesOff()
    {
        for (int i = 0; i < maxVoices; i++) {
            if (!voices[i].IsAvailable()) {
                voices[i].NoteOff(0, 0);
            }
        }
        monoNotes.clear();
    }

    Voice* VoiceBank::RequestVoice(uint8_t midiNote)
    {
        uint8_t lowestIndex = 0;
        uint8_t lowestNote = 128;
        uint8_t highestIndex = 0;
        uint8_t highestNote = 0;

        // First if that note is already playing, retrigger it to preserve envelope/lfo position.
        for (uint8_t i = 0; i < numVoices; i++) {
            if (!voices[i].IsAvailable() && voices[i].currentMidiNote == midiNote) {
                RemovePlayingVoice(i);
                AddPlayingVoice(i, midiNote);
                return &voices[i];
            }
        }

        // Else return first available voice.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsAvailable()) {
                AddPlayingVoice(i, midiNote);
                return &voices[i];
            }
        }

        // Else return first releasing voice.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsReleasing()) {
                RemovePlayingVoice(i);
                AddPlayingVoice(i, midiNote);
                return &voices[i];
            } else {
                // Find the lowest and highest playing notes to make sure that we don't return them in the next step.
                // These will be the most audibly conspicuous notes to steal.
                if (voices[i].currentMidiNote < lowestNote) {
                    lowestIndex = i;
                    lowestNote = voices[i].currentMidiNote;
                }
                if (voices[i].currentMidiNote > highestNote) {
                    highestIndex = i;
                    highestNote = voices[i].currentMidiNote;
                }
            }
        }

        // If no voices are available, return the oldest note.
        // We have to replace a playing note that is neither lowest nor highest.
        for (uint8_t i = 0; i < numVoices; i++) {
            uint8_t playingIndex = playingIndices[i];
            if (playingIndex != lowestIndex && playingIndex != highestIndex) {
                RemovePlayingVoice(playingIndex);
                AddPlayingVoice(playingIndex, midiNote);
                return &(voices[playingIndex]);
            }
        }

        // If we get here it means we have 1 or 2 voice polyphony. Return the highest note.
        RemovePlayingVoice(highestIndex);
        AddPlayingVoice(highestIndex, midiNote);
        return &(voices[highestIndex]); 
    }


    void VoiceBank::AddPlayingVoice(uint8_t index, uint8_t midiNote)
    {
        playingIndices.push_back(index);
        playingNotes.push_back(midiNote);
    }


    void VoiceBank::RemovePlayingVoice(uint8_t index)
    {
        for (uint8_t i = 0; i < playingIndices.size(); i++) {
            if (playingIndices[i] == index) {
                playingIndices.erase(playingIndices.begin()+i);
                playingNotes.erase(playingNotes.begin()+i);
                break;
            }
        }
    }

}