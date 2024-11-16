#include "VoiceBank.h"

namespace kiwi_synth
{
    void VoiceBank::Init(uint8_t maxVoices, uint8_t numOscillators, Patch* patch, float sampleRate) {
        modulations = new Modulation*[2];
        modulations[0] = new Modulation[NUM_MODULATIONS];
        modulations[1] = new Modulation[NUM_MODULATIONS];
        voiceMode = (VoiceMode)255; // Setting to an invalid number so that SetVoiceMode will trigger naturally
        InitModulations();
        for (int i = 0; i < maxVoices; i++) {
            voices[i].numOscillators = voices[i].maxOscillators;
        }

        this->patch = patch;
        this->maxVoices = maxVoices;
        this->numVoices = maxVoices;
        for (uint8_t i = 0; i < maxVoices; i++)
        {
            Voice nextVoice;
            nextVoice.Init(numOscillators, sampleRate, i);
            voices.push_back(nextVoice);
        }
        
        for (int i = 0; i < 6; i++) {
            paraOscNotes[i] = -128;
            paraOscPlaying[i] = false;
            paraOscOffRequested[i] = false;
        }
        oscTriggerCount = -1;

        sampleAndHoldInputAvailable = false;
    }

    
    void VoiceBank::UpdateSettings()
    {
        VoiceMode newVoiceMode = (VoiceMode)patch->voice1Settings->getIntValue(PatchSetting::OSC_VOICES);
        if (newVoiceMode != voiceMode) {
            AllNotesOff();
            voiceMode = newVoiceMode;

            if (voiceMode == VOICE_MODE_MONO) {
                numVoices = 1;
                voices[0].ParaphonicMode(false);
                voices[1].ParaphonicMode(false);
                voices[2].ParaphonicMode(false);
                voices[0].hardSync = false;
                voices[1].hardSync = false;
                voices[2].hardSync = false;
                voices[0].pmMode = PM_MODE_OFF;
                voices[1].pmMode = PM_MODE_OFF;
                voices[2].pmMode = PM_MODE_OFF;
            } else if (voiceMode == VOICE_MODE_PARA) {
                numVoices = 2;
                voices[0].ParaphonicMode(true);
                voices[1].ParaphonicMode(true);
                voices[2].ParaphonicMode(true);
                for (int i = 0; i < 6; i++) {
                    paraOscPlaying[i] = false;
                    paraOscNotes[i] = -128;
                    paraOscOffRequested[i] = false;
                }
                voices[0].hardSync = false;
                voices[1].hardSync = false;
                voices[2].hardSync = false;
                voices[0].pmMode = PM_MODE_OFF;
                voices[1].pmMode = PM_MODE_OFF;
                voices[2].pmMode = PM_MODE_OFF;
            } else if (voiceMode == VOICE_MODE_HSYNC) {
                numVoices = 3;
                voices[0].ParaphonicMode(false);
                voices[1].ParaphonicMode(false);
                voices[2].ParaphonicMode(false);
                voices[0].hardSync = true;
                voices[1].hardSync = true;
                voices[2].hardSync = true;
                voices[0].pmMode = PM_MODE_OFF;
                voices[1].pmMode = PM_MODE_OFF;
                voices[2].pmMode = PM_MODE_OFF;
            } else if (voiceMode == VOICE_MODE_HSYNC_MONO) {
                numVoices = 1;
                voices[0].ParaphonicMode(false);
                voices[1].ParaphonicMode(false);
                voices[2].ParaphonicMode(false);
                voices[0].hardSync = true;
                voices[1].hardSync = true;
                voices[2].hardSync = true;
                voices[0].pmMode = PM_MODE_OFF;
                voices[1].pmMode = PM_MODE_OFF;
                voices[2].pmMode = PM_MODE_OFF;
            } else if (voiceMode == VOICE_MODE_PM_PARA) {
                numVoices = 3;
                voices[0].ParaphonicMode(false);
                voices[1].ParaphonicMode(false);
                voices[2].ParaphonicMode(false);
                voices[0].hardSync = false;
                voices[1].hardSync = false;
                voices[2].hardSync = false;
                voices[0].pmMode = PM_MODE_PARALLEL;
                voices[1].pmMode = PM_MODE_PARALLEL;
                voices[2].pmMode = PM_MODE_PARALLEL;
            } else if (voiceMode == VOICE_MODE_PM_PARA_MONO) {
                numVoices = 1;
                voices[0].ParaphonicMode(false);
                voices[1].ParaphonicMode(false);
                voices[2].ParaphonicMode(false);
                voices[0].hardSync = false;
                voices[1].hardSync = false;
                voices[2].hardSync = false;
                voices[0].pmMode = PM_MODE_PARALLEL;
                voices[1].pmMode = PM_MODE_PARALLEL;
                voices[2].pmMode = PM_MODE_PARALLEL;
            } else if (voiceMode == VOICE_MODE_PM_SER) {
                numVoices = 3;
                voices[0].ParaphonicMode(false);
                voices[1].ParaphonicMode(false);
                voices[2].ParaphonicMode(false);
                voices[0].hardSync = false;
                voices[1].hardSync = false;
                voices[2].hardSync = false;
                voices[0].pmMode = PM_MODE_SERIAL;
                voices[1].pmMode = PM_MODE_SERIAL;
                voices[2].pmMode = PM_MODE_SERIAL;
            } else if (voiceMode == VOICE_MODE_PM_SER_MONO) {
                numVoices = 1;
                voices[0].ParaphonicMode(false);
                voices[1].ParaphonicMode(false);
                voices[2].ParaphonicMode(false);
                voices[0].hardSync = false;
                voices[1].hardSync = false;
                voices[2].hardSync = false;
                voices[0].pmMode = PM_MODE_SERIAL;
                voices[1].pmMode = PM_MODE_SERIAL;
                voices[2].pmMode = PM_MODE_SERIAL;
            } else { // VOICE_MODE_POLYPHONIC
                numVoices = 3;
                voices[0].ParaphonicMode(false);
                voices[1].ParaphonicMode(false);
                voices[2].ParaphonicMode(false);
                voices[0].hardSync = false;
                voices[1].hardSync = false;
                voices[2].hardSync = false;
                voices[0].pmMode = PM_MODE_OFF;
                voices[1].pmMode = PM_MODE_OFF;
                voices[2].pmMode = PM_MODE_OFF;
            }

            patch->SetVoiceMode((VoiceMode)voiceMode);
        }
        voices[0].UpdateSettings(patch->voice1Settings);
        voices[1].UpdateSettings(patch->voice2Settings);
        voices[2].UpdateSettings(patch->voice1Settings);

        UpdateModulations();
    }

    void VoiceBank::Process(float* sample)
    {
        // Triggering paraphonic oscillators to turn off after a delay to give time to detect multiple simultaneous key releases
        if (__builtin_expect(oscTriggerCount == 0, 0)) {

            for (int i = 0; i < 6; i++) {
                if (paraOscOffRequested[i]) {
                    paraOscOffRequested[i] = false;
                    paraOscPlaying[i] = false;
                    paraOscNotes[i] = -128;
                    bool noteOff = false;
                    int voice = i/3;
                    int oscBase = voice * 3;
                    if (
                        (paraOscOffRequested[oscBase] || !paraOscPlaying[oscBase])
                        && (paraOscOffRequested[oscBase + 1] || !paraOscPlaying[oscBase + 1])
                        && (paraOscOffRequested[oscBase + 2] || !paraOscPlaying[oscBase + 2])
                    ) {
                        noteOff = true;
                    }
                    voices[voice].ParaNoteOff(i%3, noteOff);
                }
            }
        }
        oscTriggerCount--;

        sample[0] = 0.0f;
        sample[1] = 0.0f;

        float nextVoice[2];

        voices[0].sampleAndHoldAvailable = sampleAndHoldInputAvailable;
        voices[0].Process(nextVoice, patch->voice1Settings, modulations[0], systemModulations, numVoices);
        sample[0] += nextVoice[0];
        sample[1] += nextVoice[1];

        if (__builtin_expect(
                voiceMode != VOICE_MODE_MONO &&
                voiceMode != VOICE_MODE_HSYNC_MONO &&
                voiceMode != VOICE_MODE_PM_PARA_MONO &&
                voiceMode != VOICE_MODE_PM_SER_MONO,
                1)) {
            voices[1].sampleAndHoldAvailable = sampleAndHoldInputAvailable;
            voices[1].Process(nextVoice, patch->voice2Settings, modulations[1], systemModulations, numVoices);
            sample[0] += nextVoice[0];
            sample[1] += nextVoice[1];
        }

        if (
                voiceMode == VOICE_MODE_HSYNC ||
                voiceMode == VOICE_MODE_PM_PARA ||
                voiceMode == VOICE_MODE_PM_SER ||
                voiceMode == VOICE_MODE_POLY
            ) {
            voices[2].sampleAndHoldAvailable = sampleAndHoldInputAvailable;
            voices[2].Process(nextVoice, patch->voice1Settings, modulations[0], systemModulations, numVoices);
            sample[0] += nextVoice[0];
            sample[1] += nextVoice[1];
        }
    }

    void VoiceBank::InitModulations() {
        for (int i = 0; i < 2; i++) {
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

            modulations[i][MODS_LFO_1_TO_OSCS].source = SRC_LFO_1;
            modulations[i][MODS_LFO_1_TO_OSCS].destination = DST_OSCS_FREQ;
            modulations[i][MODS_LFO_1_TO_OSCS].depth = 0.0f;
            modulations[i][MODS_ENV_1_TO_AMP].source = SRC_ENV_1;
            modulations[i][MODS_ENV_1_TO_AMP].destination = DST_AMP_LEVEL;
            modulations[i][MODS_ENV_1_TO_AMP].depth = 0.0f;
            modulations[i][MODS_NOTE_TO_FLT_FREQ].source = SRC_NOTE;
            modulations[i][MODS_NOTE_TO_FLT_FREQ].destination = DST_FLT_CUTOFF;
            modulations[i][MODS_NOTE_TO_FLT_FREQ].depth = 0.0f;
            modulations[i][MODS_ENV_1_TO_FLT_FREQ].source = SRC_ENV_1;
            modulations[i][MODS_ENV_1_TO_FLT_FREQ].destination = DST_FLT_CUTOFF;
            modulations[i][MODS_ENV_1_TO_FLT_FREQ].depth = 0.0f;
            modulations[i][MODS_ENV_2_TO_FLT_FREQ].source = SRC_ENV_2;
            modulations[i][MODS_ENV_2_TO_FLT_FREQ].destination = DST_FLT_CUTOFF;
            modulations[i][MODS_ENV_2_TO_FLT_FREQ].depth = 0.0f;
            modulations[i][MODS_LFO_2_TO_FLT_FREQ].source = SRC_LFO_2;
            modulations[i][MODS_LFO_2_TO_FLT_FREQ].destination = DST_FLT_CUTOFF;
            modulations[i][MODS_LFO_2_TO_FLT_FREQ].depth = 0.0f;
            modulations[i][MODS_SH_TO_FLT_FREQ].source = SRC_SH;
            modulations[i][MODS_SH_TO_FLT_FREQ].destination = DST_FLT_CUTOFF;
            modulations[i][MODS_SH_TO_FLT_FREQ].depth = 0.0f;
        }
    }

    void VoiceBank::UpdateModulations() {
        modulations[0][MODS_MOD_MATRIX_1].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_1_SOURCE);
        modulations[0][MODS_MOD_MATRIX_1].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_1_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_1].depth = patch->voice1Settings->getFloatValue(MOD_1_DEPTH);
        modulations[0][MODS_MOD_MATRIX_2].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_2_SOURCE);
        modulations[0][MODS_MOD_MATRIX_2].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_2_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_2].depth = patch->voice1Settings->getFloatValue(MOD_2_DEPTH);
        modulations[0][MODS_MOD_MATRIX_3].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_3_SOURCE);
        modulations[0][MODS_MOD_MATRIX_3].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_3_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_3].depth = patch->voice1Settings->getFloatValue(MOD_3_DEPTH);
        modulations[0][MODS_MOD_MATRIX_4].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_4_SOURCE);
        modulations[0][MODS_MOD_MATRIX_4].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_4_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_4].depth = patch->voice1Settings->getFloatValue(MOD_4_DEPTH);
        /*modulations[0][MODS_MOD_MATRIX_5].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_5_SOURCE);
        modulations[0][MODS_MOD_MATRIX_5].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_5_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_5].depth = patch->voice1Settings->getFloatValue(MOD_5_DEPTH);
        modulations[0][MODS_MOD_MATRIX_6].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_6_SOURCE);
        modulations[0][MODS_MOD_MATRIX_6].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_6_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_6].depth = patch->voice1Settings->getFloatValue(MOD_6_DEPTH);
        modulations[0][MODS_MOD_MATRIX_7].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_7_SOURCE);
        modulations[0][MODS_MOD_MATRIX_7].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_7_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_7].depth = patch->voice1Settings->getFloatValue(MOD_7_DEPTH);
        modulations[0][MODS_MOD_MATRIX_8].source = (ModulationSource)patch->voice1Settings->getIntValue(MOD_8_SOURCE);
        modulations[0][MODS_MOD_MATRIX_8].destination = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_8_DESTINATION);
        modulations[0][MODS_MOD_MATRIX_8].depth = patch->voice1Settings->getFloatValue(MOD_8_DEPTH);*/
 
        memcpy(modulations[1], modulations[0], sizeof(Modulation) * 8);
        
        systemModulations[0].source = SRC_PITCH_BEND;
        systemModulations[0].depth = 0.16666666667f;
        if (modulations[0][MODS_MOD_MATRIX_1].source != SRC_PITCH_BEND &&
                modulations[0][MODS_MOD_MATRIX_2].source != SRC_PITCH_BEND &&
                modulations[0][MODS_MOD_MATRIX_3].source != SRC_PITCH_BEND &&
                modulations[0][MODS_MOD_MATRIX_4].source != SRC_PITCH_BEND /*&&
                modulations[0][MODS_MOD_MATRIX_5].source != SRC_PITCH_BEND &&
                modulations[0][MODS_MOD_MATRIX_6].source != SRC_PITCH_BEND &&
                modulations[0][MODS_MOD_MATRIX_7].source != SRC_PITCH_BEND &&
                modulations[0][MODS_MOD_MATRIX_8].source != SRC_PITCH_BEND*/) {
            systemModulations[0].destination = DST_OSCS_FREQ;
        } else {
            systemModulations[0].destination = DST_NONE;
        }

        systemModulations[1].source = SRC_MOD_WHEEL;
        systemModulations[1].depth = 0.08333333333f;
        if (modulations[0][MODS_MOD_MATRIX_1].source != SRC_MOD_WHEEL &&
                modulations[0][MODS_MOD_MATRIX_2].source != SRC_MOD_WHEEL &&
                modulations[0][MODS_MOD_MATRIX_3].source != SRC_MOD_WHEEL &&
                modulations[0][MODS_MOD_MATRIX_4].source != SRC_MOD_WHEEL /*&&
                modulations[0][MODS_MOD_MATRIX_5].source != SRC_MOD_WHEEL &&
                modulations[0][MODS_MOD_MATRIX_6].source != SRC_MOD_WHEEL &&
                modulations[0][MODS_MOD_MATRIX_7].source != SRC_MOD_WHEEL &&
                modulations[0][MODS_MOD_MATRIX_8].source != SRC_MOD_WHEEL*/) {
            systemModulations[1].destination = DST_LFO_1_TO_MASTER_TUNE;
        } else {
            systemModulations[1].destination = DST_NONE;
        }

        systemModulations[2].source = SRC_VELOCITY;
        systemModulations[2].depth = 1.0f;
        if (modulations[0][MODS_MOD_MATRIX_1].source != SRC_VELOCITY &&
                modulations[0][MODS_MOD_MATRIX_2].source != SRC_VELOCITY &&
                modulations[0][MODS_MOD_MATRIX_3].source != SRC_VELOCITY &&
                modulations[0][MODS_MOD_MATRIX_4].source != SRC_VELOCITY /*&&
                modulations[0][MODS_MOD_MATRIX_5].source != SRC_VELOCITY &&
                modulations[0][MODS_MOD_MATRIX_6].source != SRC_VELOCITY &&
                modulations[0][MODS_MOD_MATRIX_7].source != SRC_VELOCITY &&
                modulations[0][MODS_MOD_MATRIX_8].source != SRC_VELOCITY*/) {
            systemModulations[2].destination = DST_ENV_1_TO_AMP;
        } else {
            systemModulations[2].destination = DST_NONE;
        }

        systemModulations[3].source = SRC_AFTERTOUCH;
        systemModulations[3].depth = 0.2f;
        if (modulations[0][MODS_MOD_MATRIX_1].source != SRC_AFTERTOUCH &&
                modulations[0][MODS_MOD_MATRIX_2].source != SRC_AFTERTOUCH &&
                modulations[0][MODS_MOD_MATRIX_3].source != SRC_AFTERTOUCH &&
                modulations[0][MODS_MOD_MATRIX_4].source != SRC_AFTERTOUCH /*&&
                modulations[0][MODS_MOD_MATRIX_5].source != SRC_AFTERTOUCH &&
                modulations[0][MODS_MOD_MATRIX_6].source != SRC_AFTERTOUCH &&
                modulations[0][MODS_MOD_MATRIX_7].source != SRC_AFTERTOUCH &&
                modulations[0][MODS_MOD_MATRIX_8].source != SRC_AFTERTOUCH*/) {
            systemModulations[3].destination = DST_FLT_CUTOFF;
        } else {
            systemModulations[3].destination = DST_NONE;
        }

        systemModulations[4].source = SRC_SUSTAIN;
        systemModulations[4].depth = 0.5f;
        if (modulations[0][MODS_MOD_MATRIX_1].source != SRC_SUSTAIN &&
                modulations[0][MODS_MOD_MATRIX_2].source != SRC_SUSTAIN &&
                modulations[0][MODS_MOD_MATRIX_3].source != SRC_SUSTAIN &&
                modulations[0][MODS_MOD_MATRIX_4].source != SRC_SUSTAIN /*&&
                modulations[0][MODS_MOD_MATRIX_5].source != SRC_SUSTAIN &&
                modulations[0][MODS_MOD_MATRIX_6].source != SRC_SUSTAIN &&
                modulations[0][MODS_MOD_MATRIX_7].source != SRC_SUSTAIN &&
                modulations[0][MODS_MOD_MATRIX_8].source != SRC_SUSTAIN*/) {
            systemModulations[4].destination = DST_ENV_1_RELEASE;
        } else {
            systemModulations[4].destination = DST_NONE;
        }

        systemModulations[5].source = SRC_EXPRESSION;
        systemModulations[5].depth = 0.5f;
        if (modulations[0][MODS_MOD_MATRIX_1].source != SRC_EXPRESSION &&
                modulations[0][MODS_MOD_MATRIX_2].source != SRC_EXPRESSION &&
                modulations[0][MODS_MOD_MATRIX_3].source != SRC_EXPRESSION &&
                modulations[0][MODS_MOD_MATRIX_4].source != SRC_EXPRESSION /*&&
                modulations[0][MODS_MOD_MATRIX_5].source != SRC_EXPRESSION &&
                modulations[0][MODS_MOD_MATRIX_6].source != SRC_EXPRESSION &&
                modulations[0][MODS_MOD_MATRIX_7].source != SRC_EXPRESSION &&
                modulations[0][MODS_MOD_MATRIX_8].source != SRC_EXPRESSION*/) {
            systemModulations[5].destination = DST_FLT_CUTOFF;
        } else {
            systemModulations[5].destination = DST_NONE;
        }

        if (modulations[0][MODS_MOD_MATRIX_1].destination == DST_SH_IN ||
                modulations[0][MODS_MOD_MATRIX_2].destination == DST_SH_IN ||
                modulations[0][MODS_MOD_MATRIX_3].destination == DST_SH_IN ||
                modulations[0][MODS_MOD_MATRIX_4].destination == DST_SH_IN /*||
                modulations[0][MODS_MOD_MATRIX_5].destination == DST_SH_IN ||
                modulations[0][MODS_MOD_MATRIX_6].destination == DST_SH_IN ||
                modulations[0][MODS_MOD_MATRIX_7].destination == DST_SH_IN ||
                modulations[0][MODS_MOD_MATRIX_8].destination == DST_SH_IN*/) {
            sampleAndHoldInputAvailable = true;
        } else {
            sampleAndHoldInputAvailable = false;
        }

        modulations[0][MODS_LFO_1_TO_OSCS].depth = patch->voice1Settings->getFloatValueExponential(LFO_1_TO_MASTER_TUNE);
        modulations[0][MODS_ENV_1_TO_AMP].depth = patch->voice1Settings->getFloatValue(AMP_ENV_1_DEPTH);
        modulations[0][MODS_NOTE_TO_FLT_FREQ].depth = patch->voice1Settings->getFloatValue(FLT_TRACKING);
        modulations[0][MODS_ENV_1_TO_FLT_FREQ].depth = patch->voice1Settings->getFloatValueExponential(FLT_ENV_1_DEPTH);
        modulations[0][MODS_ENV_2_TO_FLT_FREQ].depth = patch->voice1Settings->getFloatValueExponential(FLT_ENV_2_DEPTH);
        modulations[0][MODS_LFO_2_TO_FLT_FREQ].depth = patch->voice1Settings->getFloatValueExponential(LFO_2_TO_FLT_CUTOFF);
        modulations[0][MODS_SH_TO_FLT_FREQ].depth = patch->voice1Settings->getFloatValue(SH_TO_FLT_CUTOFF);

        modulations[1][MODS_LFO_1_TO_OSCS].depth = patch->voice2Settings->getFloatValueExponential(LFO_1_TO_MASTER_TUNE);
        modulations[1][MODS_ENV_1_TO_AMP].depth = patch->voice2Settings->getFloatValue(AMP_ENV_1_DEPTH);
        modulations[1][MODS_NOTE_TO_FLT_FREQ].depth = patch->voice2Settings->getFloatValue(FLT_TRACKING);
        modulations[1][MODS_ENV_1_TO_FLT_FREQ].depth = patch->voice2Settings->getFloatValueExponential(FLT_ENV_1_DEPTH);
        modulations[1][MODS_ENV_2_TO_FLT_FREQ].depth = patch->voice2Settings->getFloatValueExponential(FLT_ENV_2_DEPTH);
        modulations[1][MODS_LFO_2_TO_FLT_FREQ].depth = patch->voice2Settings->getFloatValueExponential(LFO_2_TO_FLT_CUTOFF);
        modulations[1][MODS_SH_TO_FLT_FREQ].depth = patch->voice2Settings->getFloatValue(SH_TO_FLT_CUTOFF);
    }

    void VoiceBank::NoteOn(uint8_t note, uint8_t velocity)
    {
        int oscillator;
        switch (voiceMode) {
            case VOICE_MODE_MONO:
            case VOICE_MODE_HSYNC_MONO:
            case VOICE_MODE_PM_PARA_MONO:
            case VOICE_MODE_PM_SER_MONO:
                voices[1].NoteOff(note, velocity); // Kill any voice 2 notes just in case
                playingNotes.push_back(note);
                if (voices[0].noteTriggered) {
                    voices[0].NoteOn(note, velocity, false);
                } else {
                    voices[0].NoteOn(note, velocity);
                }
                break;
            case VOICE_MODE_MULTI:
                playingNotes.push_back(note);
                if (voices[0].noteTriggered) {
                    voices[0].NoteOn(note, velocity, false);
                    voices[1].NoteOn(note, velocity, false);
                } else {
                    voices[0].NoteOn(note, velocity);
                    voices[1].NoteOn(note, velocity);
                }
                break;
            case VOICE_MODE_SPLIT:
                if (note < patch->GetSplitNote()) {
                    if (voices[1].noteTriggered) {
                        voices[1].NoteOn(note, velocity, false);
                    } else {
                        voices[1].NoteOn(note, velocity);
                    }
                } else {
                    playingNotes.push_back(note);
                    if (voices[0].noteTriggered) {
                        voices[0].NoteOn(note, velocity, false);
                    } else {
                        voices[0].NoteOn(note, velocity);
                    }
                }
                break;
            case VOICE_MODE_PARA:
                oscillator = RequestOscillator(note);
                paraOscOffRequested[oscillator] = false;
                paraOscNotes[oscillator] = note;
                paraOscPlaying[oscillator] = true;
                voices[oscillator / 3].ParaNoteOn(oscillator % 3, note, velocity);
                break;
            default:
                Voice* voice = RequestVoice(note);
                if (voice != NULL) {
                    playingNotes.push_back(note);
                    voice->NoteOn(note, velocity);
                }
                break;
        }
    }

    void VoiceBank::NoteOff(uint8_t note, uint8_t velocity)
    {
        switch (voiceMode) {
            case VOICE_MODE_MONO:
            case VOICE_MODE_HSYNC_MONO:
            case VOICE_MODE_PM_PARA_MONO:
            case VOICE_MODE_PM_SER_MONO:
                voices[1].NoteOff(note, velocity); // Kill any voice 2 notes just in case
                if (note == playingNotes.back()) {
                    playingNotes.pop_back();
                    if (playingNotes.size() > 0) {
                        int prevNote = playingNotes.back();
                        voices[0].NoteOn(prevNote, velocity, false);
                    } else {
                        voices[0].NoteOff(note, velocity);
                    }
                } else {
                    for(unsigned int i = 0; i < playingNotes.size(); i++) {
                        if (playingNotes[i] == note) {
                            playingNotes.erase(playingNotes.begin() + i);
                            break;
                        }
                    }
                }
                break;
            case VOICE_MODE_MULTI:
                if (note == playingNotes.back()) {
                    playingNotes.pop_back();
                    if (playingNotes.size() > 0) {
                        int prevNote = playingNotes.back();
                        voices[0].NoteOn(prevNote, velocity, false);
                        voices[1].NoteOn(prevNote, velocity, false);
                    } else {
                        voices[0].NoteOff(note, velocity);
                        voices[1].NoteOff(note, velocity);
                    }
                } else {
                    for(unsigned int i = 0; i < playingNotes.size(); i++) {
                        if (playingNotes[i] == note) {
                            playingNotes.erase(playingNotes.begin() + i);
                            break;
                        }
                    }
                }
                break;
            case VOICE_MODE_SPLIT:
                if (note < patch->GetSplitNote()) {
                    if (voices[1].noteTriggered && voices[1].currentMidiNote == note) {
                        voices[1].NoteOff(note, velocity);
                        break;
                    }
                } else {
                    if (note == playingNotes.back()) {
                        playingNotes.pop_back();
                        if (playingNotes.size() > 0) {
                            int prevNote = playingNotes.back();
                            voices[0].NoteOn(prevNote, velocity, false);
                        } else {
                            voices[0].NoteOff(note, velocity);
                        }
                    } else {
                        for(unsigned int i = 0; i < playingNotes.size(); i++) {
                            if (playingNotes[i] == note) {
                                playingNotes.erase(playingNotes.begin() + i);
                                break;
                            }
                        }
                    }
                }
                break;
            case VOICE_MODE_PARA:
                for (int i = 0; i < 6; i++) {
                    if (paraOscNotes[i] == note) {
                        paraOscOffRequested[i] = true;
                        oscTriggerCount = OSC_TRIGGER_SAMPLES;
                    }
                }
            default:
                int numVoicesPlaying = 0;
                int voiceFound = -1;

                // Look for a voice playing the note
                for (size_t i = 0; i < voices.size(); i++) {
                    if (!voices[i].IsAvailable()) {
                        numVoicesPlaying++;
                        if (voices[i].noteTriggered && voices[i].triggerNote == note) {
                            voiceFound = i;
                        }
                    }
                }

                if (voiceFound == -1) {
                    // We didn't find the voice. There is no note to stop.
                } else if (numVoicesPlaying == maxVoices && playingNotes.size() > maxVoices) {
                    int otherNote = voices[voiceFound ? 0 : 1].triggerNote;

                    // Go backward through the vector to find the last note that isn't playing
                    for (size_t i = 0; i < playingNotes.size(); i++) {
                        if (playingNotes[i] != note && playingNotes[i] != otherNote) {
                            // We have found it. Switch to that note
                            voices[voiceFound].NoteOn(playingNotes[i], velocity, false);
                        }
                    }
                } else {
                    // There are no extra notes on the stack. Just stop the note normally.
                    voices[voiceFound] .NoteOff(note, velocity);
                }

                // Remove the note from the stack if it exists
                for(unsigned int j = 0; j < playingNotes.size(); j++) {
                    if (playingNotes[j] == note) {
                        playingNotes.erase(playingNotes.begin() + j);
                        break;
                    }
                }
                break;
        }
    }

    void VoiceBank::AllNotesOff()
    {
        for (int i = 0; i < maxVoices; i++) {
            voices[i].NoteOff(0, 0);
        }
        playingNotes.clear();

        if (patch->GetVoiceMode() == VOICE_MODE_PARA) {
            for (int i = 0; i < 6; i++) {
                paraOscNotes[i] = -128;
                paraOscPlaying[i] = false;
                paraOscOffRequested[i] = false;
            }
            voices[0].ParaAllNotesOff();
            voices[1].ParaAllNotesOff();
        }
    }

    Voice* VoiceBank::RequestVoice(uint8_t midiNote)
    {
        uint8_t highestIndex = 0;
        uint8_t highestNote = 0;

        // First if that note is already playing, retrigger it to preserve envelope/lfo position.
        for (uint8_t i = 0; i < numVoices; i++) {
            if (!voices[i].IsAvailable() && voices[i].currentMidiNote == midiNote) {
                return &voices[i];
            }
        }

        // Else play an available note if it was previously the same note to preserve portamento.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsAvailable() && voices[i].currentMidiNote == midiNote) {
                return &voices[i];
            }
        }

        // Else if both available, take the voice that was previously closest in pitch to help with portamento.
        if (voices[0].IsAvailable() && voices[1].IsAvailable() && voices[2].IsAvailable()) {
            int closestVoice = 0;
            for (int i = 1; i < numVoices; i++) {
                if (std::abs(midiNote - voices[i].currentMidiNote) < std::abs(midiNote - voices[closestVoice].currentMidiNote)) {
                    closestVoice = i;
                }
            }
            return &voices[closestVoice];
        }
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsAvailable() && voices[i].currentMidiNote == midiNote) {
                return &voices[i];
            }
        }

        // Else return first available voice.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsAvailable()) {
                return &voices[i];
            }
        }

        // Else return first releasing voice that is not triggered.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsReleasing() && !voices[i].noteTriggered) {
                return &voices[i];
            }
        }

        // Else return first releasing voice.
        for (size_t i = 0; i < numVoices; i++) {
            if (voices[i].IsReleasing()) {
                return &voices[i];
            } else {
                // Find the highest playing note to make sure that we return that note in the next step.
                // The lowest note will be the most audibly conspicuous note to steal so we don't want that one.
                if (voices[i].currentMidiNote > highestNote) {
                    highestIndex = i;
                    highestNote = voices[i].currentMidiNote;
                }
            }
        }

        // Return the highest note.
        return &(voices[highestIndex]); 
    }

    int VoiceBank::RequestOscillator(uint8_t note) {
        // First use the first oscillators of each voice.
        if (OscillatorIsAvailable(0)) {
            return 0;
        }
        if (OscillatorIsAvailable(3)) {
            return 3;
        }

        // Find whether the note is closer in pitch to the first or second voice and try to get
        // a voice from that oscillator if possible, but get a voice from the other one if necessary.
        // This will help to cluster three note chords played on one hand together in one voice
        // so that they can release together.
        int delta0 = std::abs(paraOscNotes[0] - note);
        int delta3 = std::abs(paraOscNotes[3] - note);

        if (delta0 < delta3) {
            for (int i = 1; i < 6; i++) {
                if (OscillatorIsAvailable(i)) {
                    return i;
                }
            }
        } else {
            for (int i = 5; i > 0; i--) {
                if (OscillatorIsAvailable(i)) {
                    return i;
                }
            }
        }

        // All voices are used, so steal a note that is neither the highest nor lowest
        uint8_t highestIndex = 0;
        uint8_t highestNote = 0;
        uint8_t lowestIndex = 0;
        uint8_t lowestNote = 127;
        for (int i = 0; i < 6; i++) {
            // Find the highest
            if (paraOscNotes[i] > highestNote) {
                highestNote = paraOscNotes[i];
                highestIndex = i;
            }
            // Find the lowest
            if (paraOscNotes[i] < lowestNote) {
                lowestNote = paraOscNotes[i];
                lowestIndex = i;
            }
        }
        // Find a note to return/replace
        if (delta0 < delta3) {
            for (int i = 0; i < 6; i++) {
                if (i != highestIndex && i != lowestIndex) {
                    return i;
                }
            }
        } else {
            for (int i = 5; i >= 0; i--) {
                if (i != highestIndex && i != lowestIndex) {
                    return i;
                }
            }
        }

        // We should never reach this point since there are six oscillators and only 2 highest/lowest
        return 0;
    }

}