#include "VoiceBank.h"



using namespace kiwisynth;



void VoiceBank::Init(uint8_t maxVoices, uint8_t numOscillators, Patch* patch, float sampleRate) {
    modulations_ = new Modulation*[2];
    modulations_[0] = new Modulation[NUM_MODULATIONS];
    modulations_[1] = new Modulation[NUM_MODULATIONS];
    voiceMode_ = (VoiceMode)255; // Setting to an invalid number so that SetVoiceMode will trigger naturally
    InitModulations();
    for (int i = 0; i < maxVoices; i++) {
        voices_[i].numOscillators_ = voices_[i].maxOscillators_;
    }

    patch_ = patch;
    maxVoices_ = maxVoices;
    numVoices_ = maxVoices;
    for (uint8_t i = 0; i < maxVoices; i++)
    {
        Voice nextVoice;
        nextVoice.Init(numOscillators, sampleRate, i);
        voices_.push_back(nextVoice);
    }
    
    for (int i = 0; i < 6; i++) {
        paraOscNotes_[i] = -128;
        isParaOscPlaying_[i] = false;
        isParaOscOffRequested_[i] = false;
    }
    oscTriggerCount_ = -1;

    isSampleAndHoldInputAvailable_ = false;
}



void VoiceBank::UpdateSettings()
{
    VoiceMode newVoiceMode = (VoiceMode)patch_->voice1Settings_->getIntValue(PatchSetting::OSC_VOICES);
    if (newVoiceMode != voiceMode_) {
        AllNotesOff();
        voiceMode_ = newVoiceMode;

        if (voiceMode_ == VOICE_MODE_MONO) {
            numVoices_ = 1;
            voices_[0].ParaphonicMode(false);
            voices_[1].ParaphonicMode(false);
            voices_[2].ParaphonicMode(false);
            voices_[0].isHardSyncOn_ = false;
            voices_[1].isHardSyncOn_ = false;
            voices_[2].isHardSyncOn_ = false;
            voices_[0].pmMode_ = PM_MODE_OFF;
            voices_[1].pmMode_ = PM_MODE_OFF;
            voices_[2].pmMode_ = PM_MODE_OFF;
        } else if (voiceMode_ == VOICE_MODE_PARA) {
            numVoices_ = 2;
            voices_[0].ParaphonicMode(true);
            voices_[1].ParaphonicMode(true);
            voices_[2].ParaphonicMode(true);
            for (int i = 0; i < 6; i++) {
                isParaOscPlaying_[i] = false;
                paraOscNotes_[i] = -128;
                isParaOscOffRequested_[i] = false;
            }
            voices_[0].isHardSyncOn_ = false;
            voices_[1].isHardSyncOn_ = false;
            voices_[2].isHardSyncOn_ = false;
            voices_[0].pmMode_ = PM_MODE_OFF;
            voices_[1].pmMode_ = PM_MODE_OFF;
            voices_[2].pmMode_ = PM_MODE_OFF;
        } else if (voiceMode_ == VOICE_MODE_HSYNC) {
            numVoices_ = 3;
            voices_[0].ParaphonicMode(false);
            voices_[1].ParaphonicMode(false);
            voices_[2].ParaphonicMode(false);
            voices_[0].isHardSyncOn_ = true;
            voices_[1].isHardSyncOn_ = true;
            voices_[2].isHardSyncOn_ = true;
            voices_[0].pmMode_ = PM_MODE_OFF;
            voices_[1].pmMode_ = PM_MODE_OFF;
            voices_[2].pmMode_ = PM_MODE_OFF;
        } else if (voiceMode_ == VOICE_MODE_HSYNC_MONO) {
            numVoices_ = 1;
            voices_[0].ParaphonicMode(false);
            voices_[1].ParaphonicMode(false);
            voices_[2].ParaphonicMode(false);
            voices_[0].isHardSyncOn_ = true;
            voices_[1].isHardSyncOn_ = true;
            voices_[2].isHardSyncOn_ = true;
            voices_[0].pmMode_ = PM_MODE_OFF;
            voices_[1].pmMode_ = PM_MODE_OFF;
            voices_[2].pmMode_ = PM_MODE_OFF;
        } else if (voiceMode_ == VOICE_MODE_PM_PARA) {
            numVoices_ = 3;
            voices_[0].ParaphonicMode(false);
            voices_[1].ParaphonicMode(false);
            voices_[2].ParaphonicMode(false);
            voices_[0].isHardSyncOn_ = false;
            voices_[1].isHardSyncOn_ = false;
            voices_[2].isHardSyncOn_ = false;
            voices_[0].pmMode_ = PM_MODE_PARALLEL;
            voices_[1].pmMode_ = PM_MODE_PARALLEL;
            voices_[2].pmMode_ = PM_MODE_PARALLEL;
        } else if (voiceMode_ == VOICE_MODE_PM_PARA_MONO) {
            numVoices_ = 1;
            voices_[0].ParaphonicMode(false);
            voices_[1].ParaphonicMode(false);
            voices_[2].ParaphonicMode(false);
            voices_[0].isHardSyncOn_ = false;
            voices_[1].isHardSyncOn_ = false;
            voices_[2].isHardSyncOn_ = false;
            voices_[0].pmMode_ = PM_MODE_PARALLEL;
            voices_[1].pmMode_ = PM_MODE_PARALLEL;
            voices_[2].pmMode_ = PM_MODE_PARALLEL;
        } else if (voiceMode_ == VOICE_MODE_PM_SER) {
            numVoices_ = 3;
            voices_[0].ParaphonicMode(false);
            voices_[1].ParaphonicMode(false);
            voices_[2].ParaphonicMode(false);
            voices_[0].isHardSyncOn_ = false;
            voices_[1].isHardSyncOn_ = false;
            voices_[2].isHardSyncOn_ = false;
            voices_[0].pmMode_ = PM_MODE_SERIAL;
            voices_[1].pmMode_ = PM_MODE_SERIAL;
            voices_[2].pmMode_ = PM_MODE_SERIAL;
        } else if (voiceMode_ == VOICE_MODE_PM_SER_MONO) {
            numVoices_ = 1;
            voices_[0].ParaphonicMode(false);
            voices_[1].ParaphonicMode(false);
            voices_[2].ParaphonicMode(false);
            voices_[0].isHardSyncOn_ = false;
            voices_[1].isHardSyncOn_ = false;
            voices_[2].isHardSyncOn_ = false;
            voices_[0].pmMode_ = PM_MODE_SERIAL;
            voices_[1].pmMode_ = PM_MODE_SERIAL;
            voices_[2].pmMode_ = PM_MODE_SERIAL;
        } else { // VOICE_MODE_POLYPHONIC
            numVoices_ = 3;
            voices_[0].ParaphonicMode(false);
            voices_[1].ParaphonicMode(false);
            voices_[2].ParaphonicMode(false);
            voices_[0].isHardSyncOn_ = false;
            voices_[1].isHardSyncOn_ = false;
            voices_[2].isHardSyncOn_ = false;
            voices_[0].pmMode_ = PM_MODE_OFF;
            voices_[1].pmMode_ = PM_MODE_OFF;
            voices_[2].pmMode_ = PM_MODE_OFF;
        }

        patch_->SetVoiceMode((VoiceMode)voiceMode_);
    }
    voices_[0].UpdateSettings(patch_->voice1Settings_);
    voices_[1].UpdateSettings(patch_->voice2Settings_);
    voices_[2].UpdateSettings(patch_->voice1Settings_);

    UpdateModulations();
}



void VoiceBank::Process(float* sample)
{
    // Triggering paraphonic oscillators to turn off after a delay to give time to detect multiple simultaneous key releases
    if (__builtin_expect(oscTriggerCount_ == 0, 0)) {

        for (int i = 0; i < 6; i++) {
            if (isParaOscOffRequested_[i]) {
                isParaOscOffRequested_[i] = false;
                isParaOscPlaying_[i] = false;
                paraOscNotes_[i] = -128;
                bool noteOff = false;
                int voice = i/3;
                int oscBase = voice * 3;
                if (
                    (isParaOscOffRequested_[oscBase] || !isParaOscPlaying_[oscBase])
                    && (isParaOscOffRequested_[oscBase + 1] || !isParaOscPlaying_[oscBase + 1])
                    && (isParaOscOffRequested_[oscBase + 2] || !isParaOscPlaying_[oscBase + 2])
                ) {
                    noteOff = true;
                }
                voices_[voice].ParaNoteOff(i%3, noteOff);
            }
        }
    }
    oscTriggerCount_--;

    sample[0] = 0.0f;
    sample[1] = 0.0f;

    float nextVoice[2];

    voices_[0].isSampleAndHoldAvailable_ = isSampleAndHoldInputAvailable_;
    voices_[0].Process(nextVoice, patch_->voice1Settings_, modulations_[0], systemModulations_, numVoices_);
    sample[0] += nextVoice[0];
    sample[1] += nextVoice[1];

    if (__builtin_expect(
            voiceMode_ != VOICE_MODE_MONO &&
            voiceMode_ != VOICE_MODE_HSYNC_MONO &&
            voiceMode_ != VOICE_MODE_PM_PARA_MONO &&
            voiceMode_ != VOICE_MODE_PM_SER_MONO,
            1)) {
        voices_[1].isSampleAndHoldAvailable_ = isSampleAndHoldInputAvailable_;
        voices_[1].Process(nextVoice, patch_->voice2Settings_, modulations_[1], systemModulations_, numVoices_);
        sample[0] += nextVoice[0];
        sample[1] += nextVoice[1];
    }

    if (
            voiceMode_ == VOICE_MODE_HSYNC ||
            voiceMode_ == VOICE_MODE_PM_PARA ||
            voiceMode_ == VOICE_MODE_PM_SER ||
            voiceMode_ == VOICE_MODE_POLY
        ) {
        voices_[2].isSampleAndHoldAvailable_ = isSampleAndHoldInputAvailable_;
        voices_[2].Process(nextVoice, patch_->voice1Settings_, modulations_[0], systemModulations_, numVoices_);
        sample[0] += nextVoice[0];
        sample[1] += nextVoice[1];
    }
}



void VoiceBank::InitModulations() {
    for (int i = 0; i < 2; i++) {
        modulations_[i][MODS_MOD_MATRIX_1].source_ = SRC_NONE;
        modulations_[i][MODS_MOD_MATRIX_1].destination_ = DST_NONE;
        modulations_[i][MODS_MOD_MATRIX_1].depth_ = 0.0f;
        modulations_[i][MODS_MOD_MATRIX_2].source_ = SRC_NONE;
        modulations_[i][MODS_MOD_MATRIX_2].destination_ = DST_NONE;
        modulations_[i][MODS_MOD_MATRIX_2].depth_ = 0.0f;
        modulations_[i][MODS_MOD_MATRIX_3].source_ = SRC_NONE;
        modulations_[i][MODS_MOD_MATRIX_3].destination_ = DST_NONE;
        modulations_[i][MODS_MOD_MATRIX_3].depth_ = 0.0f;
        modulations_[i][MODS_MOD_MATRIX_4].source_ = SRC_NONE;
        modulations_[i][MODS_MOD_MATRIX_4].destination_ = DST_NONE;
        modulations_[i][MODS_MOD_MATRIX_4].depth_ = 0.0f;
        modulations_[i][MODS_MOD_MATRIX_5].source_ = SRC_NONE;
        modulations_[i][MODS_MOD_MATRIX_5].destination_ = DST_NONE;
        modulations_[i][MODS_MOD_MATRIX_5].depth_ = 0.0f;
        modulations_[i][MODS_MOD_MATRIX_6].source_ = SRC_NONE;
        modulations_[i][MODS_MOD_MATRIX_6].destination_ = DST_NONE;
        modulations_[i][MODS_MOD_MATRIX_6].depth_ = 0.0f;
        modulations_[i][MODS_MOD_MATRIX_7].source_ = SRC_NONE;
        modulations_[i][MODS_MOD_MATRIX_7].destination_ = DST_NONE;
        modulations_[i][MODS_MOD_MATRIX_7].depth_ = 0.0f;
        modulations_[i][MODS_MOD_MATRIX_8].source_ = SRC_NONE;
        modulations_[i][MODS_MOD_MATRIX_8].destination_ = DST_NONE;
        modulations_[i][MODS_MOD_MATRIX_8].depth_ = 0.0f;

        modulations_[i][MODS_LFO_1_TO_OSCS].source_ = SRC_LFO_1;
        modulations_[i][MODS_LFO_1_TO_OSCS].destination_ = DST_OSCS_FREQ;
        modulations_[i][MODS_LFO_1_TO_OSCS].depth_ = 0.0f;
        modulations_[i][MODS_ENV_1_TO_AMP].source_ = SRC_ENV_1;
        modulations_[i][MODS_ENV_1_TO_AMP].destination_ = DST_AMP_LEVEL;
        modulations_[i][MODS_ENV_1_TO_AMP].depth_ = 0.0f;
        modulations_[i][MODS_NOTE_TO_FLT_FREQ].source_ = SRC_NOTE;
        modulations_[i][MODS_NOTE_TO_FLT_FREQ].destination_ = DST_FLT_CUTOFF;
        modulations_[i][MODS_NOTE_TO_FLT_FREQ].depth_ = 0.0f;
        modulations_[i][MODS_ENV_1_TO_FLT_FREQ].source_ = SRC_ENV_1;
        modulations_[i][MODS_ENV_1_TO_FLT_FREQ].destination_ = DST_FLT_CUTOFF;
        modulations_[i][MODS_ENV_1_TO_FLT_FREQ].depth_ = 0.0f;
        modulations_[i][MODS_ENV_2_TO_FLT_FREQ].source_ = SRC_ENV_2;
        modulations_[i][MODS_ENV_2_TO_FLT_FREQ].destination_ = DST_FLT_CUTOFF;
        modulations_[i][MODS_ENV_2_TO_FLT_FREQ].depth_ = 0.0f;
        modulations_[i][MODS_LFO_2_TO_FLT_FREQ].source_ = SRC_LFO_2;
        modulations_[i][MODS_LFO_2_TO_FLT_FREQ].destination_ = DST_FLT_CUTOFF;
        modulations_[i][MODS_LFO_2_TO_FLT_FREQ].depth_ = 0.0f;
        modulations_[i][MODS_SH_TO_FLT_FREQ].source_ = SRC_SH;
        modulations_[i][MODS_SH_TO_FLT_FREQ].destination_ = DST_FLT_CUTOFF;
        modulations_[i][MODS_SH_TO_FLT_FREQ].depth_ = 0.0f;
    }
}



void VoiceBank::UpdateModulations() {
    modulations_[0][MODS_MOD_MATRIX_1].source_ = (ModulationSource)patch_->voice1Settings_->getIntValue(MOD_1_SOURCE);
    modulations_[0][MODS_MOD_MATRIX_1].destination_ = (ModulationDestination)patch_->voice1Settings_->getIntValue(MOD_1_DESTINATION);
    modulations_[0][MODS_MOD_MATRIX_1].depth_ = patch_->voice1Settings_->getFloatValue(MOD_1_DEPTH);
    modulations_[0][MODS_MOD_MATRIX_2].source_ = (ModulationSource)patch_->voice1Settings_->getIntValue(MOD_2_SOURCE);
    modulations_[0][MODS_MOD_MATRIX_2].destination_ = (ModulationDestination)patch_->voice1Settings_->getIntValue(MOD_2_DESTINATION);
    modulations_[0][MODS_MOD_MATRIX_2].depth_ = patch_->voice1Settings_->getFloatValue(MOD_2_DEPTH);
    modulations_[0][MODS_MOD_MATRIX_3].source_ = (ModulationSource)patch_->voice1Settings_->getIntValue(MOD_3_SOURCE);
    modulations_[0][MODS_MOD_MATRIX_3].destination_ = (ModulationDestination)patch_->voice1Settings_->getIntValue(MOD_3_DESTINATION);
    modulations_[0][MODS_MOD_MATRIX_3].depth_ = patch_->voice1Settings_->getFloatValue(MOD_3_DEPTH);
    modulations_[0][MODS_MOD_MATRIX_4].source_ = (ModulationSource)patch_->voice1Settings_->getIntValue(MOD_4_SOURCE);
    modulations_[0][MODS_MOD_MATRIX_4].destination_ = (ModulationDestination)patch_->voice1Settings_->getIntValue(MOD_4_DESTINATION);
    modulations_[0][MODS_MOD_MATRIX_4].depth_ = patch_->voice1Settings_->getFloatValue(MOD_4_DEPTH);
    /*modulations_[0][MODS_MOD_MATRIX_5].source_ = (ModulationSource)patch->voice1Settings->getIntValue(MOD_5_SOURCE);
    modulations_[0][MODS_MOD_MATRIX_5].destination_ = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_5_DESTINATION);
    modulations_[0][MODS_MOD_MATRIX_5].depth_ = patch->voice1Settings->getFloatValue(MOD_5_DEPTH);
    modulations_[0][MODS_MOD_MATRIX_6].source_ = (ModulationSource)patch->voice1Settings->getIntValue(MOD_6_SOURCE);
    modulations_[0][MODS_MOD_MATRIX_6].destination_ = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_6_DESTINATION);
    modulations_[0][MODS_MOD_MATRIX_6].depth_ = patch->voice1Settings->getFloatValue(MOD_6_DEPTH);
    modulations_[0][MODS_MOD_MATRIX_7].source_ = (ModulationSource)patch->voice1Settings->getIntValue(MOD_7_SOURCE);
    modulations_[0][MODS_MOD_MATRIX_7].destination_ = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_7_DESTINATION);
    modulations_[0][MODS_MOD_MATRIX_7].depth_ = patch->voice1Settings->getFloatValue(MOD_7_DEPTH);
    modulations_[0][MODS_MOD_MATRIX_8].source_ = (ModulationSource)patch->voice1Settings->getIntValue(MOD_8_SOURCE);
    modulations_[0][MODS_MOD_MATRIX_8].destination_ = (ModulationDestination)patch->voice1Settings->getIntValue(MOD_8_DESTINATION);
    modulations_[0][MODS_MOD_MATRIX_8].depth_ = patch->voice1Settings->getFloatValue(MOD_8_DEPTH);*/

    memcpy(modulations_[1], modulations_[0], sizeof(Modulation) * 8);
    
    systemModulations_[0].source_ = SRC_PITCH_BEND;
    systemModulations_[0].depth_ = 0.16666666667f;
    if (modulations_[0][MODS_MOD_MATRIX_1].source_ != SRC_PITCH_BEND &&
            modulations_[0][MODS_MOD_MATRIX_2].source_ != SRC_PITCH_BEND &&
            modulations_[0][MODS_MOD_MATRIX_3].source_ != SRC_PITCH_BEND &&
            modulations_[0][MODS_MOD_MATRIX_4].source_ != SRC_PITCH_BEND /*&&
            modulations_[0][MODS_MOD_MATRIX_5].source_ != SRC_PITCH_BEND &&
            modulations_[0][MODS_MOD_MATRIX_6].source_ != SRC_PITCH_BEND &&
            modulations_[0][MODS_MOD_MATRIX_7].source_ != SRC_PITCH_BEND &&
            modulations_[0][MODS_MOD_MATRIX_8].source_ != SRC_PITCH_BEND*/) {
        systemModulations_[0].destination_ = DST_OSCS_FREQ;
    } else {
        systemModulations_[0].destination_ = DST_NONE;
    }

    systemModulations_[1].source_ = SRC_MOD_WHEEL;
    systemModulations_[1].depth_ = 0.08333333333f;
    if (modulations_[0][MODS_MOD_MATRIX_1].source_ != SRC_MOD_WHEEL &&
            modulations_[0][MODS_MOD_MATRIX_2].source_ != SRC_MOD_WHEEL &&
            modulations_[0][MODS_MOD_MATRIX_3].source_ != SRC_MOD_WHEEL &&
            modulations_[0][MODS_MOD_MATRIX_4].source_ != SRC_MOD_WHEEL /*&&
            modulations_[0][MODS_MOD_MATRIX_5].source_ != SRC_MOD_WHEEL &&
            modulations_[0][MODS_MOD_MATRIX_6].source_ != SRC_MOD_WHEEL &&
            modulations_[0][MODS_MOD_MATRIX_7].source_ != SRC_MOD_WHEEL &&
            modulations_[0][MODS_MOD_MATRIX_8].source_ != SRC_MOD_WHEEL*/) {
        systemModulations_[1].destination_ = DST_LFO_1_TO_MASTER_TUNE;
    } else {
        systemModulations_[1].destination_ = DST_NONE;
    }

    systemModulations_[2].source_ = SRC_VELOCITY;
    systemModulations_[2].depth_ = 1.0f;
    if (modulations_[0][MODS_MOD_MATRIX_1].source_ != SRC_VELOCITY &&
            modulations_[0][MODS_MOD_MATRIX_2].source_ != SRC_VELOCITY &&
            modulations_[0][MODS_MOD_MATRIX_3].source_ != SRC_VELOCITY &&
            modulations_[0][MODS_MOD_MATRIX_4].source_ != SRC_VELOCITY /*&&
            modulations_[0][MODS_MOD_MATRIX_5].source_ != SRC_VELOCITY &&
            modulations_[0][MODS_MOD_MATRIX_6].source_ != SRC_VELOCITY &&
            modulations_[0][MODS_MOD_MATRIX_7].source_ != SRC_VELOCITY &&
            modulations_[0][MODS_MOD_MATRIX_8].source_ != SRC_VELOCITY*/) {
        systemModulations_[2].destination_ = DST_ENV_1_TO_AMP;
    } else {
        systemModulations_[2].destination_ = DST_NONE;
    }

    systemModulations_[3].source_ = SRC_AFTERTOUCH;
    systemModulations_[3].depth_ = 0.2f;
    if (modulations_[0][MODS_MOD_MATRIX_1].source_ != SRC_AFTERTOUCH &&
            modulations_[0][MODS_MOD_MATRIX_2].source_ != SRC_AFTERTOUCH &&
            modulations_[0][MODS_MOD_MATRIX_3].source_ != SRC_AFTERTOUCH &&
            modulations_[0][MODS_MOD_MATRIX_4].source_ != SRC_AFTERTOUCH /*&&
            modulations_[0][MODS_MOD_MATRIX_5].source_ != SRC_AFTERTOUCH &&
            modulations_[0][MODS_MOD_MATRIX_6].source_ != SRC_AFTERTOUCH &&
            modulations_[0][MODS_MOD_MATRIX_7].source_ != SRC_AFTERTOUCH &&
            modulations_[0][MODS_MOD_MATRIX_8].source_ != SRC_AFTERTOUCH*/) {
        systemModulations_[3].destination_ = DST_FLT_CUTOFF;
    } else {
        systemModulations_[3].destination_ = DST_NONE;
    }

    systemModulations_[4].source_ = SRC_SUSTAIN;
    systemModulations_[4].depth_ = 0.5f;
    if (modulations_[0][MODS_MOD_MATRIX_1].source_ != SRC_SUSTAIN &&
            modulations_[0][MODS_MOD_MATRIX_2].source_ != SRC_SUSTAIN &&
            modulations_[0][MODS_MOD_MATRIX_3].source_ != SRC_SUSTAIN &&
            modulations_[0][MODS_MOD_MATRIX_4].source_ != SRC_SUSTAIN /*&&
            modulations_[0][MODS_MOD_MATRIX_5].source_ != SRC_SUSTAIN &&
            modulations_[0][MODS_MOD_MATRIX_6].source_ != SRC_SUSTAIN &&
            modulations_[0][MODS_MOD_MATRIX_7].source_ != SRC_SUSTAIN &&
            modulations_[0][MODS_MOD_MATRIX_8].source_ != SRC_SUSTAIN*/) {
        systemModulations_[4].destination_ = DST_ENV_1_RELEASE;
    } else {
        systemModulations_[4].destination_ = DST_NONE;
    }

/*    systemModulations_[5].source_ = SRC_EXPRESSION;
    systemModulations_[5].depth_ = 0.5f;
    if (modulations_[0][MODS_MOD_MATRIX_1].source_ != SRC_EXPRESSION &&
            modulations_[0][MODS_MOD_MATRIX_2].source_ != SRC_EXPRESSION &&
            modulations_[0][MODS_MOD_MATRIX_3].source_ != SRC_EXPRESSION &&
            modulations_[0][MODS_MOD_MATRIX_4].source_ != SRC_EXPRESSION *//*&&
            modulations_[0][MODS_MOD_MATRIX_5].source_ != SRC_EXPRESSION &&
            modulations_[0][MODS_MOD_MATRIX_6].source_ != SRC_EXPRESSION &&
            modulations_[0][MODS_MOD_MATRIX_7].source_ != SRC_EXPRESSION &&
            modulations_[0][MODS_MOD_MATRIX_8].source_ != SRC_EXPRESSION*//*) {
        systemModulations_[5].destination_ = DST_FLT_CUTOFF;
    } else {
        systemModulations_[5].destination_ = DST_NONE;
    }*/

    if (modulations_[0][MODS_MOD_MATRIX_1].destination_ == DST_SH_IN ||
            modulations_[0][MODS_MOD_MATRIX_2].destination_ == DST_SH_IN ||
            modulations_[0][MODS_MOD_MATRIX_3].destination_ == DST_SH_IN ||
            modulations_[0][MODS_MOD_MATRIX_4].destination_ == DST_SH_IN /*||
            modulations_[0][MODS_MOD_MATRIX_5].destination_ == DST_SH_IN ||
            modulations_[0][MODS_MOD_MATRIX_6].destination_ == DST_SH_IN ||
            modulations_[0][MODS_MOD_MATRIX_7].destination_ == DST_SH_IN ||
            modulations_[0][MODS_MOD_MATRIX_8].destination_ == DST_SH_IN*/) {
        isSampleAndHoldInputAvailable_ = true;
    } else {
        isSampleAndHoldInputAvailable_ = false;
    }

    modulations_[0][MODS_LFO_1_TO_OSCS].depth_ = patch_->voice1Settings_->getFloatValueExponential(LFO_1_TO_MASTER_TUNE);
    modulations_[0][MODS_ENV_1_TO_AMP].depth_ = patch_->voice1Settings_->getFloatValue(AMP_ENV_1_DEPTH);
    modulations_[0][MODS_NOTE_TO_FLT_FREQ].depth_ = patch_->voice1Settings_->getFloatValue(FLT_TRACKING);
    modulations_[0][MODS_ENV_1_TO_FLT_FREQ].depth_ = patch_->voice1Settings_->getFloatValueExponential(FLT_ENV_1_DEPTH);
    modulations_[0][MODS_ENV_2_TO_FLT_FREQ].depth_ = patch_->voice1Settings_->getFloatValueExponential(FLT_ENV_2_DEPTH);
    modulations_[0][MODS_LFO_2_TO_FLT_FREQ].depth_ = patch_->voice1Settings_->getFloatValueExponential(LFO_2_TO_FLT_CUTOFF);
    modulations_[0][MODS_SH_TO_FLT_FREQ].depth_ = patch_->voice1Settings_->getFloatValue(SH_TO_FLT_CUTOFF);

    modulations_[1][MODS_LFO_1_TO_OSCS].depth_ = patch_->voice2Settings_->getFloatValueExponential(LFO_1_TO_MASTER_TUNE);
    modulations_[1][MODS_ENV_1_TO_AMP].depth_ = patch_->voice2Settings_->getFloatValue(AMP_ENV_1_DEPTH);
    modulations_[1][MODS_NOTE_TO_FLT_FREQ].depth_ = patch_->voice2Settings_->getFloatValue(FLT_TRACKING);
    modulations_[1][MODS_ENV_1_TO_FLT_FREQ].depth_ = patch_->voice2Settings_->getFloatValueExponential(FLT_ENV_1_DEPTH);
    modulations_[1][MODS_ENV_2_TO_FLT_FREQ].depth_ = patch_->voice2Settings_->getFloatValueExponential(FLT_ENV_2_DEPTH);
    modulations_[1][MODS_LFO_2_TO_FLT_FREQ].depth_ = patch_->voice2Settings_->getFloatValueExponential(LFO_2_TO_FLT_CUTOFF);
    modulations_[1][MODS_SH_TO_FLT_FREQ].depth_ = patch_->voice2Settings_->getFloatValue(SH_TO_FLT_CUTOFF);
}



void VoiceBank::NoteOn(uint8_t note, uint8_t velocity)
{
    int oscillator;
    switch (voiceMode_) {
        case VOICE_MODE_MONO:
        case VOICE_MODE_HSYNC_MONO:
        case VOICE_MODE_PM_PARA_MONO:
        case VOICE_MODE_PM_SER_MONO:
            voices_[1].NoteOff(note, velocity); // Kill any voice 2 notes just in case
            playingNotes_.push_back(note);
            if (voices_[0].isNoteTriggered_) {
                voices_[0].NoteOn(note, velocity, false);
            } else {
                voices_[0].NoteOn(note, velocity);
            }
            break;
        case VOICE_MODE_MULTI:
            playingNotes_.push_back(note);
            if (voices_[0].isNoteTriggered_) {
                voices_[0].NoteOn(note, velocity, false);
                voices_[1].NoteOn(note, velocity, false);
            } else {
                voices_[0].NoteOn(note, velocity);
                voices_[1].NoteOn(note, velocity);
            }
            break;
        case VOICE_MODE_SPLIT:
            if (note < patch_->GetSplitNote()) {
                if (voices_[1].isNoteTriggered_) {
                    voices_[1].NoteOn(note, velocity, false);
                } else {
                    voices_[1].NoteOn(note, velocity);
                }
            } else {
                playingNotes_.push_back(note);
                if (voices_[0].isNoteTriggered_) {
                    voices_[0].NoteOn(note, velocity, false);
                } else {
                    voices_[0].NoteOn(note, velocity);
                }
            }
            break;
        case VOICE_MODE_PARA:
            oscillator = RequestOscillator(note);
            isParaOscOffRequested_[oscillator] = false;
            paraOscNotes_[oscillator] = note;
            isParaOscPlaying_[oscillator] = true;
            voices_[oscillator / 3].ParaNoteOn(oscillator % 3, note, velocity);
            break;
        default:
            Voice* voice = RequestVoice(note);
            if (voice != NULL) {
                playingNotes_.push_back(note);
                voice->NoteOn(note, velocity);
            }
            break;
    }
}



void VoiceBank::NoteOff(uint8_t note, uint8_t velocity)
{
    switch (voiceMode_) {
        case VOICE_MODE_MONO:
        case VOICE_MODE_HSYNC_MONO:
        case VOICE_MODE_PM_PARA_MONO:
        case VOICE_MODE_PM_SER_MONO:
            voices_[1].NoteOff(note, velocity); // Kill any voice 2 notes just in case
            if (note == playingNotes_.back()) {
                playingNotes_.pop_back();
                if (playingNotes_.size() > 0) {
                    int prevNote = playingNotes_.back();
                    voices_[0].NoteOn(prevNote, velocity, false);
                } else {
                    voices_[0].NoteOff(note, velocity);
                }
            } else {
                for(unsigned int i = 0; i < playingNotes_.size(); i++) {
                    if (playingNotes_[i] == note) {
                        playingNotes_.erase(playingNotes_.begin() + i);
                        break;
                    }
                }
            }
            break;
        case VOICE_MODE_MULTI:
            if (note == playingNotes_.back()) {
                playingNotes_.pop_back();
                if (playingNotes_.size() > 0) {
                    int prevNote = playingNotes_.back();
                    voices_[0].NoteOn(prevNote, velocity, false);
                    voices_[1].NoteOn(prevNote, velocity, false);
                } else {
                    voices_[0].NoteOff(note, velocity);
                    voices_[1].NoteOff(note, velocity);
                }
            } else {
                for(unsigned int i = 0; i < playingNotes_.size(); i++) {
                    if (playingNotes_[i] == note) {
                        playingNotes_.erase(playingNotes_.begin() + i);
                        break;
                    }
                }
            }
            break;
        case VOICE_MODE_SPLIT:
            if (note < patch_->GetSplitNote()) {
                if (voices_[1].isNoteTriggered_ && voices_[1].currentMidiNote_ == note) {
                    voices_[1].NoteOff(note, velocity);
                    break;
                }
            } else {
                if (note == playingNotes_.back()) {
                    playingNotes_.pop_back();
                    if (playingNotes_.size() > 0) {
                        int prevNote = playingNotes_.back();
                        voices_[0].NoteOn(prevNote, velocity, false);
                    } else {
                        voices_[0].NoteOff(note, velocity);
                    }
                } else {
                    for(unsigned int i = 0; i < playingNotes_.size(); i++) {
                        if (playingNotes_[i] == note) {
                            playingNotes_.erase(playingNotes_.begin() + i);
                            break;
                        }
                    }
                }
            }
            break;
        case VOICE_MODE_PARA:
            for (int i = 0; i < 6; i++) {
                if (paraOscNotes_[i] == note) {
                    isParaOscOffRequested_[i] = true;
                    oscTriggerCount_ = OSC_TRIGGER_SAMPLES;
                }
            }
        default:
            int numVoicesPlaying = 0;
            int voiceFound = -1;

            // Look for a voice playing the note
            for (size_t i = 0; i < voices_.size(); i++) {
                if (!voices_[i].IsAvailable()) {
                    numVoicesPlaying++;
                    if (voices_[i].triggerNote_ == note) {
                        voiceFound = i;
                    }
                }
            }

            if (voiceFound == -1) {
                // We didn't find the voice. There is no note to stop.
            } else if (numVoicesPlaying == maxVoices_ && playingNotes_.size() > maxVoices_) {
                std::vector<int> otherNotes;
                for (int i = 0; i < numVoices_; i++) {
                    if (i != voiceFound) {
                        otherNotes.push_back(voices_[i].triggerNote_);
                    }
                }

                // Go backward through the vector to find the last note that isn't playing
                for (size_t i = 0; i < playingNotes_.size(); i++) {
                    if (playingNotes_[i] != note && count(otherNotes.begin(), otherNotes.end(), playingNotes_[i]) == 0) {
                        // We have found it. Switch to that note
                        voices_[voiceFound].NoteOn(playingNotes_[i], velocity, false);
                    }
                }

            } else {
                // There are no extra notes on the stack. Just stop the note normally.
                voices_[voiceFound] .NoteOff(note, velocity);
            }

            // Remove the note from the stack if it exists
            for(unsigned int j = 0; j < playingNotes_.size(); j++) {
                if (playingNotes_[j] == note) {
                    playingNotes_.erase(playingNotes_.begin() + j);
                    break;
                }
            }
            break;
    }
}



void VoiceBank::AllNotesOff()
{
    for (int i = 0; i < maxVoices_; i++) {
        voices_[i].NoteOff(0, 0);
    }
    playingNotes_.clear();

    if (patch_->GetVoiceMode() == VOICE_MODE_PARA) {
        for (int i = 0; i < 6; i++) {
            paraOscNotes_[i] = -128;
            isParaOscPlaying_[i] = false;
            isParaOscOffRequested_[i] = false;
        }
        voices_[0].ParaAllNotesOff();
        voices_[1].ParaAllNotesOff();
    }
}



Voice* VoiceBank::RequestVoice(uint8_t midiNote)
{
    // First if that note is already playing, retrigger it to preserve envelope/lfo position.
    for (uint8_t i = 0; i < numVoices_; i++) {
        if (!voices_[i].IsAvailable() && voices_[i].currentMidiNote_ == midiNote) {
            return &voices_[i];
        }
    }

    // Else take the available voice that was previously closest in pitch to help with portamento.
    int closestVoice = -1;
    int closestDistance = 128;
    for (int i = 0; i < numVoices_; i++) {
        int nextDistance = std::abs(midiNote - voices_[i].currentMidiNote_);
        if (nextDistance < closestDistance && voices_[i].IsAvailable() && !voices_[i].IsReleasing()) {
            closestVoice = i;
            closestDistance = nextDistance;
        }
    }
    if (closestVoice > -1) {
        return &voices_[closestVoice];
    }

    // Else return first available voice.
    for (size_t i = 0; i < numVoices_; i++) {
        if (voices_[i].IsAvailable()) {
            return &voices_[i];
        }
    }

    // Else return first releasing voice that is not triggered.
    for (size_t i = 0; i < numVoices_; i++) {
        if (voices_[i].IsReleasing() && !voices_[i].isNoteTriggered_) {
            return &voices_[i];
        }
    }

    uint8_t highestIndex = 0;
    uint8_t highestNote = 0;

    // Else return first releasing voice.
    for (size_t i = 0; i < numVoices_; i++) {
        if (voices_[i].IsReleasing()) {
            return &voices_[i];
        } else {
            // Find the highest playing note to make sure that we return that note in the next step.
            // The lowest note will be the most audibly conspicuous note to steal so we don't want that one.
            if (voices_[i].currentMidiNote_ > highestNote) {
                highestIndex = i;
                highestNote = voices_[i].currentMidiNote_;
            }
        }
    }

    // Return the highest note.
    return &(voices_[highestIndex]); 

    /*
    // All voices are used, so steal a note that is neither the highest nor lowest
    uint8_t highestIndex = 0;
    uint8_t highestNote = 0;
    uint8_t lowestIndex = 0;
    uint8_t lowestNote = 127;
    for (int i = 0; i < 6; i++) {
        // Find the highest
        if (paraOscNotes_[i] > highestNote) {
            highestNote = paraOscNotes_[i];
            highestIndex = i;
        }
        // Find the lowest
        if (paraOscNotes_[i] < lowestNote) {
            lowestNote = paraOscNotes_[i];
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
    */
}



int VoiceBank::RequestOscillator(uint8_t note) {
    // First use the first oscillators of each voice.
    if (IsOscillatorAvailable(0)) {
        return 0;
    }
    if (IsOscillatorAvailable(3)) {
        return 3;
    }

    // Find whether the note is closer in pitch to the first or second voice and try to get
    // a voice from that oscillator if possible, but get a voice from the other one if necessary.
    // This will help to cluster three note chords played on one hand together in one voice
    // so that they can release together.
    int delta0 = std::abs(paraOscNotes_[0] - note);
    int delta3 = std::abs(paraOscNotes_[3] - note);

    if (delta0 < delta3) {
        for (int i = 1; i < 6; i++) {
            if (IsOscillatorAvailable(i)) {
                return i;
            }
        }
    } else {
        for (int i = 5; i > 0; i--) {
            if (IsOscillatorAvailable(i)) {
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
        if (paraOscNotes_[i] > highestNote) {
            highestNote = paraOscNotes_[i];
            highestIndex = i;
        }
        // Find the lowest
        if (paraOscNotes_[i] < lowestNote) {
            lowestNote = paraOscNotes_[i];
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
