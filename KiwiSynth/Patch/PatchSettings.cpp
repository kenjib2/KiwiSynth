#include "PatchSettings.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwisynth
{
    // -------------------- BEGIN INITIALIZE STATIC ARRAYS --------------------
    static int8_t* genMaxIntValues() {
        int8_t* tmp = new int8_t[NUM_PATCH_SETTINGS];

        // Set the max values for int variables
        memset(tmp, 0, NUM_PATCH_SETTINGS * sizeof(int8_t));
        tmp[OSC_VOICES] = VOICE_MODE_MAX - 1;
        tmp[OSC_1_WAVEFORM] = WAVEFORM_MAX - 1;
        tmp[OSC_2_WAVEFORM] = WAVEFORM_MAX - 1;
        tmp[OSC_2_OCTAVE] = 4;
        tmp[OSC_2_INTERVAL] = 22;
        tmp[OSC_3_WAVEFORM] = WAVEFORM_MAX - 1;
        tmp[OSC_3_OCTAVE] = 4;
        tmp[OSC_3_INTERVAL] = 22;
        tmp[OSC_NOISE_TYPE] = NOISE_TYPE_MAX - 1;
        tmp[FLT_FILTER_TYPE] = FILTER_TYPE_MAX - 1;
        tmp[LFO_1_WAVEFORM] = LFO_WAVEFORM_MAX - 1;
        tmp[LFO_2_WAVEFORM] = LFO_WAVEFORM_MAX - 1;
        tmp[MOD_1_SOURCE] = NUM_MOD_SOURCES - 1;
        tmp[MOD_1_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        tmp[MOD_2_SOURCE] = NUM_MOD_SOURCES - 1;
        tmp[MOD_2_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        tmp[MOD_3_SOURCE] = NUM_MOD_SOURCES - 1;
        tmp[MOD_3_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        tmp[MOD_4_SOURCE] = NUM_MOD_SOURCES - 1;
        tmp[MOD_4_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        tmp[MOD_5_SOURCE] = NUM_MOD_SOURCES - 1;
        tmp[MOD_5_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        tmp[MOD_6_SOURCE] = NUM_MOD_SOURCES - 1;
        tmp[MOD_6_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        tmp[MOD_7_SOURCE] = NUM_MOD_SOURCES - 1;
        tmp[MOD_7_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        tmp[MOD_8_SOURCE] = NUM_MOD_SOURCES - 1;
        tmp[MOD_8_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        tmp[GEN_SELECT] = 127;

        return tmp;
    }
    int8_t *PatchSettings::maxIntValues = genMaxIntValues();

    static float* genLMinLookup() {
        float* tmp = new float[NUM_PATCH_SETTINGS];

        memset(tmp, 0, NUM_PATCH_SETTINGS * sizeof(float));
        tmp[OSC_PORTAMENTO_SPEED] = logf(0.0001f < 0.0000001f ? 0.0000001f : 0.0001f);
        tmp[FX_1] = logf(MIN_DISTORTION_GAIN < 0.0000001f ? 0.0000001f : MIN_DISTORTION_GAIN);
        tmp[LFO_1_RATE] = logf(0.1f < 0.0000001f ? 0.0000001f : 0.1f);
        tmp[LFO_2_RATE] = logf(0.1f < 0.0000001f ? 0.0000001f : 0.1f);
        tmp[SH_RATE] = logf(floatSampleRate / 1000.f < 0.0000001f ? 0.0000001f : floatSampleRate / 1000.f);
        tmp[MOD_1_DEPTH] = logf(0.1f < 0.0000001f ? 0.0000001f : 0.045f);  // Using MOD_1_DEPTH as a hacky fill in for alternate FX_1 value

        return tmp;
    }
    float *PatchSettings::lMinLookup = genLMinLookup();

    static float* genLMaxLookup() {
        float* tmp = new float[NUM_PATCH_SETTINGS];

        memset(tmp, 0, NUM_PATCH_SETTINGS * sizeof(float));
        tmp[OSC_PORTAMENTO_SPEED] = logf(0.05f);
        tmp[FX_1] = logf(MAX_DISTORTION_GAIN);
        tmp[LFO_1_RATE] = logf(20.0f);
        tmp[LFO_2_RATE] = logf(20.0f);
        tmp[SH_RATE] = logf(floatSampleRate);
        tmp[MOD_1_DEPTH] = logf(12.0f);  // Using MOD_1_DEPTH as a hacky fill in for alternate FX_1 value

        return tmp;
    }
    float *PatchSettings::lMaxLookup = genLMaxLookup();
    // -------------------- END INITIALIZE STATIC ARRAYS --------------------

    void PatchSettings::Init(MultiPots* multiPots, GpioExpansion* ge)
    {
        this->multiPots = multiPots;
        this->ge = ge;

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 16; j++) {
                lastPinValues[i][j] = false;
            }
        }

        // Set default that does not change.
        setValue(OSC_1_ON, true);

        // Set initial encoder values.
        setValue(OSC_VOICES, (int8_t)0);
        setValue(OSC_1_WAVEFORM, (int8_t)0);
        setValue(OSC_2_WAVEFORM, (int8_t)0);
        setValue(OSC_2_OCTAVE, (int8_t)2);
        setValue(OSC_2_INTERVAL, (int8_t)11);
        setValue(OSC_3_WAVEFORM, (int8_t)0);
        setValue(OSC_3_OCTAVE, (int8_t)2);
        setValue(OSC_3_INTERVAL, (int8_t)11);
        setValue(OSC_NOISE_TYPE, (int8_t)0);
        setValue(FLT_FILTER_TYPE, (int8_t)0);
        setValue(LFO_1_WAVEFORM, (int8_t)0);
        setValue(LFO_2_WAVEFORM, (int8_t)0);
        setValue(MOD_1_SOURCE, (int8_t)0);
        setValue(MOD_1_DESTINATION, (int8_t)-1);
        setValue(MOD_2_SOURCE, (int8_t)0);
        setValue(MOD_2_DESTINATION, (int8_t)-1);
        setValue(MOD_3_SOURCE, (int8_t)0);
        setValue(MOD_3_DESTINATION, (int8_t)-1);
        setValue(MOD_4_SOURCE, (int8_t)0);
        setValue(MOD_4_DESTINATION, (int8_t)-1);
        setValue(MOD_5_SOURCE, (int8_t)0);
        setValue(MOD_5_DESTINATION, (int8_t)-1);
        setValue(MOD_6_SOURCE, (int8_t)0);
        setValue(MOD_6_DESTINATION, (int8_t)-1);
        setValue(MOD_7_SOURCE, (int8_t)0);
        setValue(MOD_7_DESTINATION, (int8_t)-1);
        setValue(MOD_8_SOURCE, (int8_t)0);
        setValue(MOD_8_DESTINATION, (int8_t)-1);
        setValue(GEN_SELECT, (int8_t)0);
        setValue(GEN_SELECT_BUTTON, false);

        // Set external controllers in case they are not in use
        setValue(GEN_PITCH_BEND, 0.0f);
        setValue(GEN_AFTERTOUCH, 0.0f);
        setValue(GEN_MOD_WHEEL, 0.0f);
        setValue(GEN_EXPRESSION, 0.0f);
        setValue(GEN_SUSTAIN, 0.0f);

        for (int i = 0; i < 8; i++) {
            modSigns[i] = 1.0f;
        }

        SetControlsLive(true);
        DefaultSettings();
    }

    void PatchSettings::Copy(SavedPatch* savedPatch, int voiceNumber)
    {
        if (voiceNumber == 0) {
            memcpy(boolValues, savedPatch->voice1BoolValues, sizeof(bool) * NUM_PATCH_SETTINGS_SAVED);
            memcpy(intValues, savedPatch->voice1IntValues, sizeof(int8_t) * NUM_PATCH_SETTINGS_SAVED);
            memcpy(floatValues, savedPatch->voice1FloatValues, sizeof(float) * NUM_PATCH_SETTINGS_SAVED);
        } else {
            memcpy(boolValues, savedPatch->voice2BoolValues, sizeof(bool) * NUM_PATCH_SETTINGS_SAVED);
            memcpy(intValues, savedPatch->voice2IntValues, sizeof(int8_t) * NUM_PATCH_SETTINGS_SAVED);
            memcpy(floatValues, savedPatch->voice2FloatValues, sizeof(float) * NUM_PATCH_SETTINGS_SAVED);
        }

        SetModSigns();
    }

    void PatchSettings::SetModSigns() {
        for (int i = 0; i < 8; i++) {
            if (this->floatValues[MOD_1_DEPTH + i] >= 0.0f) {
                modSigns[i] = 1.0f;
            } else {
                modSigns[i] = -1.0f;
            }
        }
    }

    void PatchSettings::Copy(PatchSettings* patchSettings)
    {
        memcpy(boolValues, patchSettings->boolValues, sizeof(bool) * NUM_PATCH_SETTINGS);
        memcpy(intValues, patchSettings->intValues, sizeof(int8_t) * NUM_PATCH_SETTINGS);
        memcpy(floatValues, patchSettings->floatValues, sizeof(float) * NUM_PATCH_SETTINGS);

        SetModSigns();
    }

    void PatchSettings::CopyVoiceSettings(PatchSettings* patchSettings)
    {
        memcpy(boolValues, patchSettings->boolValues, sizeof(bool) * MOD_1_SOURCE);
        memcpy(intValues, patchSettings->intValues, sizeof(int8_t) * MOD_1_SOURCE);
        memcpy(floatValues, patchSettings->floatValues, sizeof(float) * MOD_1_SOURCE);
    }

    void PatchSettings::Load(SavedPatch* savedPatch, int voiceNumber)
    {
        if (voiceNumber == 0) {
            memcpy(savedPatch->voice1BoolValues, boolValues, sizeof(bool) * NUM_PATCH_SETTINGS_SAVED);
            memcpy(savedPatch->voice1IntValues, intValues, sizeof(int8_t) * NUM_PATCH_SETTINGS_SAVED);
            memcpy(savedPatch->voice1FloatValues, floatValues, sizeof(float) * NUM_PATCH_SETTINGS_SAVED);
        } else if (voiceNumber == 1) {
            memcpy(savedPatch->voice2BoolValues, boolValues, sizeof(bool) * NUM_PATCH_SETTINGS_SAVED);
            memcpy(savedPatch->voice2IntValues, intValues, sizeof(int8_t) * NUM_PATCH_SETTINGS_SAVED);
            memcpy(savedPatch->voice2FloatValues, floatValues, sizeof(float) * NUM_PATCH_SETTINGS_SAVED);
        }
    }

    void PatchSettings::processEncoder(PatchSetting setting, int controlNumber, int pinA, int pinB)
    {
        int index = controlNumber - 0x20;
        bool lastAValue = lastPinValues[index][pinA - 1];
        bool pinAValue = ge->getPinValue(controlNumber, pinA);
        bool pinBValue = ge->getPinValue(controlNumber, pinB);

        if (lastAValue && !pinAValue) {
            int8_t currentValue = getIntValue(setting);

            if (pinBValue != pinAValue) {
                if (currentValue > 0) {
                    setValue(setting, --currentValue);
                } else if (currentValue == 0 && ((setting >= MOD_1_DESTINATION && setting <= MOD_8_DESTINATION) || setting == GEN_SELECT)) {
                    // Mod destination can go to -1
                    setValue(setting, --currentValue);
                } else if (setting == GEN_SELECT) {
                    // Gen select has no lower limit
                    setValue(setting, --currentValue);
                }
            } else {
                setValue(setting, ++currentValue);
            }
        }

        lastPinValues[index][pinA - 1] = pinAValue;
    }

    void PatchSettings::updatePotValues(int controlNumber)
    {
        if (!isLive) {
            UpdateFloatProtect(controlNumber);
        }

        switch(controlNumber)
        {
            case 0:
                setValue(FLT_ENV_2_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(OSC_3_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_2_DECAY, multiPots->GetMpValue(2, controlNumber));
                setValue(OSC_PORTAMENTO_SPEED, multiPots->GetDirectValue(0));
                break;
            case 1:
                setValue(SH_RATE, multiPots->GetMpValue(0, controlNumber));
                setValue(OSC_3_FINE_TUNE, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_2_SUSTAIN, multiPots->GetMpValue(2, controlNumber));
                break;
            case 2:
                setValue(MOD_3_DEPTH, multiPots->GetMpValue(0, controlNumber) * modSigns[2]);
                setValue(AMP_ENV_1_DEPTH, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_2_RELEASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 3:
                setValue(MOD_4_DEPTH, multiPots->GetMpValue(0, controlNumber) * modSigns[3]);
                setValue(OSC_2_FINE_TUNE, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_1_DECAY, multiPots->GetMpValue(2, controlNumber));
                break;
            case 4:
                setValue(MOD_2_DEPTH, multiPots->GetMpValue(0, controlNumber) * modSigns[1]);
                setValue(AMP_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_1_SUSTAIN, multiPots->GetMpValue(2, controlNumber));
                break;
            case 5:
                setValue(MOD_1_DEPTH, multiPots->GetMpValue(0, controlNumber) * modSigns[0]);
                setValue(OSC_2_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_1_RELEASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 6:
                setValue(FX_REVERB, multiPots->GetMpValue(0, controlNumber));
                setValue(OSC_MASTER_TUNE, multiPots->GetMpValue(1, controlNumber));
                setValue(FLT_ENV_1_DEPTH, multiPots->GetMpValue(2, controlNumber));
                break;
            case 7:
                setValue(FX_5, multiPots->GetMpValue(0, controlNumber));
                setValue(OSC_1_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(FLT_TRACKING, multiPots->GetMpValue(2, controlNumber));
                break;
            case 8:
                setValue(FX_4, multiPots->GetMpValue(0, controlNumber));
                setValue(OSC_1_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(OSC_INPUT_LEVEL, multiPots->GetMpValue(2, controlNumber));
                break;
            case 9:
                setValue(GEN_BALANCE, multiPots->GetMpValue(0, controlNumber));
                setValue(OSC_2_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(OSC_INPUT_THRESHOLD, multiPots->GetMpValue(2, controlNumber));
                break;
            case 10:
                setValue(FX_3, multiPots->GetMpValue(0, controlNumber));
                setValue(LFO_1_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(LFO_1_TRIGGER_PHASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 11:
                setValue(FX_2, multiPots->GetMpValue(0, controlNumber));
                setValue(LFO_2_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(LFO_2_TRIGGER_PHASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 12:
                setValue(FX_1, multiPots->GetMpValue(0, controlNumber));
                setValue(LFO_2_RATE, multiPots->GetMpValue(1, controlNumber));
                setValue(LFO_1_TO_MASTER_TUNE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 13:
                setValue(FLT_CUTOFF, multiPots->GetMpValue(0, controlNumber));
                setValue(LFO_1_RATE, multiPots->GetMpValue(1, controlNumber));
                setValue(LFO_2_TO_FLT_CUTOFF, multiPots->GetMpValue(2, controlNumber));
                break;
            case 14:
                setValue(FLT_RESONANCE, multiPots->GetMpValue(0, controlNumber));
                setValue(OSC_NOISE_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_2_ATTACK, multiPots->GetMpValue(2, controlNumber));
                break;
            case 15:
                setValue(SH_TO_FLT_CUTOFF, multiPots->GetMpValue(0, controlNumber));
                setValue(OSC_3_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_1_ATTACK, multiPots->GetMpValue(2, controlNumber));
                break;
        }
    }

    void PatchSettings::SetFloatProtect(PatchSetting patchSetting, float newValue) {
        float currentValue = getFloatValue(patchSetting);

        if (floatProtectedUpdateNeeded) { // Initial settings
            if (newValue < currentValue) {
                floatValuesProtected[patchSetting] = -1;
            } else {
                floatValuesProtected[patchSetting] = 1;
            }
        } else { // Updating settings
            if ((floatValuesProtected[patchSetting] == -1 && newValue > currentValue) ||
                    (floatValuesProtected[patchSetting] == 1 && newValue < currentValue) ||
                    (std::fabs(newValue - currentValue)) < 0.003f) { // Checking proximity because near 0.0f and 1.0f we might not be able to cross via the pot
                floatValuesProtected[patchSetting] = 0;
            }
        }
    }

    void PatchSettings::UpdateFloatProtect(int controlNumber)
    {
        switch(controlNumber)
        {
            case 0:
                SetFloatProtect(FLT_ENV_2_DEPTH, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(OSC_3_LEVEL, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(ENV_2_DECAY, multiPots->GetMpValue(2, controlNumber));
                SetFloatProtect(OSC_PORTAMENTO_SPEED, multiPots->GetDirectValue(0));
                break;
            case 1:
                SetFloatProtect(SH_RATE, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(OSC_3_FINE_TUNE, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(ENV_2_SUSTAIN, multiPots->GetMpValue(2, controlNumber));
                break;
            case 2:
                SetFloatProtect(MOD_3_DEPTH, multiPots->GetMpValue(0, controlNumber) * modSigns[2]);
                SetFloatProtect(AMP_ENV_1_DEPTH, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(ENV_2_RELEASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 3:
                SetFloatProtect(MOD_4_DEPTH, multiPots->GetMpValue(0, controlNumber) * modSigns[3]);
                SetFloatProtect(OSC_2_FINE_TUNE, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(ENV_1_DECAY, multiPots->GetMpValue(2, controlNumber));
                break;
            case 4:
                SetFloatProtect(MOD_2_DEPTH, multiPots->GetMpValue(0, controlNumber) * modSigns[1]);
                SetFloatProtect(AMP_LEVEL, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(ENV_1_SUSTAIN, multiPots->GetMpValue(2, controlNumber));
                break;
            case 5:
                SetFloatProtect(MOD_1_DEPTH, multiPots->GetMpValue(0, controlNumber) * modSigns[0]);
                SetFloatProtect(OSC_2_LEVEL, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(ENV_1_RELEASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 6:
                SetFloatProtect(FX_REVERB, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(OSC_MASTER_TUNE, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(FLT_ENV_1_DEPTH, multiPots->GetMpValue(2, controlNumber));
                break;
            case 7:
                SetFloatProtect(FX_5, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(OSC_1_LEVEL, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(FLT_TRACKING, multiPots->GetMpValue(2, controlNumber));
                break;
            case 8:
                SetFloatProtect(FX_4, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(OSC_1_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(OSC_INPUT_LEVEL, multiPots->GetMpValue(2, controlNumber));
                break;
            case 9:
                SetFloatProtect(GEN_BALANCE, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(OSC_2_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(OSC_INPUT_THRESHOLD, multiPots->GetMpValue(2, controlNumber));
                break;
            case 10:
                SetFloatProtect(FX_3, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(LFO_1_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(LFO_1_TRIGGER_PHASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 11:
                SetFloatProtect(FX_2, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(LFO_2_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(LFO_2_TRIGGER_PHASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 12:
                SetFloatProtect(FX_1, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(LFO_2_RATE, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(LFO_1_TO_MASTER_TUNE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 13:
                SetFloatProtect(FLT_CUTOFF, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(LFO_1_RATE, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(LFO_2_TO_FLT_CUTOFF, multiPots->GetMpValue(2, controlNumber));
                break;
            case 14:
                SetFloatProtect(FLT_RESONANCE, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(OSC_NOISE_LEVEL, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(ENV_2_ATTACK, multiPots->GetMpValue(2, controlNumber));
                break;
            case 15:
                SetFloatProtect(SH_TO_FLT_CUTOFF, multiPots->GetMpValue(0, controlNumber));
                SetFloatProtect(OSC_3_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                SetFloatProtect(ENV_1_ATTACK, multiPots->GetMpValue(2, controlNumber));
                break;
        }

        if (floatProtectedUpdateNeeded) {
            floatProtectedUpdateCounter++;
            if (floatProtectedUpdateCounter > 15) {
                floatProtectedUpdateNeeded = false;
            }
        }
    }

    void PatchSettings::updateGpioExpansionValues(int controlNumber)
    {
        if (!isLive) {
            UpdateBoolProtect(controlNumber);
        }

        switch(controlNumber)
        {
            case 0x20:
                setValue(OSC_PORTAMENTO_ON, ge->getPinValue(controlNumber, 7));
                boolValues[GEN_SELECT_BUTTON] = ge->getPinValue(controlNumber, 11); // Bypass setValue so that it always updates or else the UI will freeze after loading a patch.
                setValue(ENV_2_REVERSE_PHASE_ON, ge->getPinValue(controlNumber, 14));
                setValue(ENV_1_REVERSE_PHASE_ON, ge->getPinValue(controlNumber, 15));
                processEncoder(MOD_1_SOURCE, controlNumber, 1, 2);
                processEncoder(OSC_3_INTERVAL, controlNumber, 4, 3);
                processEncoder(OSC_2_INTERVAL, controlNumber, 6, 5);
                processEncoder(GEN_SELECT, controlNumber, 9, 10);
                processEncoder(FLT_FILTER_TYPE, controlNumber, 12, 13);
                break;
            case 0x21:
                setValue(OSC_2_ON, ge->getPinValue(controlNumber, 5));
                processEncoder(OSC_2_WAVEFORM, controlNumber, 1, 2);
                processEncoder(OSC_3_WAVEFORM, controlNumber, 3, 4);
                processEncoder(OSC_VOICES, controlNumber, 10, 6);
                processEncoder(OSC_1_WAVEFORM, controlNumber, 11, 12);
                processEncoder(OSC_2_OCTAVE, controlNumber, 13, 14);
                processEncoder(OSC_3_OCTAVE, controlNumber, 15, 9);
                break;
            case 0x22:
                processEncoder(MOD_4_SOURCE, controlNumber, 2, 1);
                processEncoder(MOD_3_SOURCE, controlNumber, 4, 3);
                processEncoder(MOD_2_SOURCE, controlNumber, 6, 5);
                processEncoder(MOD_1_DESTINATION, controlNumber, 9, 7);
                processEncoder(MOD_2_DESTINATION, controlNumber, 10, 11);
                processEncoder(MOD_3_DESTINATION, controlNumber, 12, 13);
                processEncoder(MOD_4_DESTINATION, controlNumber, 15, 14);
                break;
            case 0x23:
                setValue(OSC_NOISE_ON, ge->getPinValue(controlNumber, 5));
                setValue(OSC_3_ON, ge->getPinValue(controlNumber, 6));
                setValue(OSC_INPUT_ON, ge->getPinValue(controlNumber, 9));
                setValue(LFO_1_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 10));
                setValue(LFO_2_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 11));
                processEncoder(LFO_2_WAVEFORM, controlNumber, 2, 1);
                processEncoder(LFO_1_WAVEFORM, controlNumber, 4, 3);
                processEncoder(OSC_NOISE_TYPE, controlNumber, 13, 12);
                break;
        }
    }

    void PatchSettings::UpdateBoolProtect(int controlNumber)
    {
        switch(controlNumber)
        {
            case 0x20:
                SetBoolProtect(OSC_PORTAMENTO_ON, ge->getPinValue(controlNumber, 7));
                SetBoolProtect(ENV_2_REVERSE_PHASE_ON, ge->getPinValue(controlNumber, 14));
                SetBoolProtect(ENV_1_REVERSE_PHASE_ON, ge->getPinValue(controlNumber, 15));
                break;
            case 0x21:
                SetBoolProtect(OSC_2_ON, ge->getPinValue(controlNumber, 5));
                break;
            case 0x22:
                break;
            case 0x23:
                SetBoolProtect(OSC_NOISE_ON, ge->getPinValue(controlNumber, 5));
                SetBoolProtect(OSC_3_ON, ge->getPinValue(controlNumber, 6));
                SetBoolProtect(OSC_INPUT_ON, ge->getPinValue(controlNumber, 9));
                SetBoolProtect(LFO_1_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 10));
                SetBoolProtect(LFO_2_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 11));
                break;
        }
    }

    void PatchSettings::DefaultSettings()
    {
        setValue(OSC_VOICES, (int8_t)0);
        setValue(OSC_MASTER_TUNE, 0.5f);
        setValue(OSC_PORTAMENTO_ON, false);
        setValue(OSC_PORTAMENTO_SPEED, 0.5f);

        setValue(OSC_1_WAVEFORM, (int8_t)0);
        setValue(OSC_2_WAVEFORM, (int8_t)0);
        setValue(OSC_3_WAVEFORM, (int8_t)0);
        setValue(OSC_1_PULSE_WIDTH, 0.0f);
        setValue(OSC_2_PULSE_WIDTH, 0.0f);
        setValue(OSC_3_PULSE_WIDTH, 0.0f);
        setValue(OSC_1_LEVEL, 0.999f);
        setValue(OSC_2_LEVEL, 0.999f);
        setValue(OSC_3_LEVEL, 0.999f);
        setValue(OSC_1_ON, true);
        setValue(OSC_2_ON, false);
        setValue(OSC_3_ON, false);
        setValue(OSC_2_OCTAVE, (int8_t)2);
        setValue(OSC_3_OCTAVE, (int8_t)2);
        setValue(OSC_2_INTERVAL, (int8_t)11);
        setValue(OSC_3_INTERVAL, (int8_t)11);
        setValue(OSC_2_FINE_TUNE, 0.5f);
        setValue(OSC_3_FINE_TUNE, 0.5f);

        setValue(OSC_NOISE_ON, false);
        setValue(OSC_NOISE_TYPE, (int8_t)0);
        setValue(OSC_NOISE_LEVEL, 0.0f);

        setValue(OSC_INPUT_ON, false);
        setValue(OSC_INPUT_THRESHOLD, 0.5f);
        setValue(OSC_INPUT_LEVEL, 0.0f);

        setValue(FLT_FILTER_TYPE, (int8_t)0);
        setValue(FLT_CUTOFF, 0.999f);
        setValue(FLT_RESONANCE, 0.0f);
        setValue(FLT_TRACKING, 0.999f);
        setValue(FLT_ENV_1_DEPTH, 0.0f);
        setValue(FLT_ENV_2_DEPTH, 0.0f);

        setValue(AMP_LEVEL, 0.0f);
        setValue(AMP_ENV_1_DEPTH, 0.999f);

        setValue(ENV_1_ATTACK, 0.01f);
        setValue(ENV_2_ATTACK, 0.01f);
        setValue(ENV_1_DECAY, 0.01f);
        setValue(ENV_2_DECAY, 0.01f);
        setValue(ENV_1_SUSTAIN, 0.999f);
        setValue(ENV_2_SUSTAIN, 0.999f);
        setValue(ENV_1_RELEASE, 0.01f);
        setValue(ENV_2_RELEASE, 0.01f);
        setValue(ENV_1_REVERSE_PHASE_ON, false);
        setValue(ENV_2_REVERSE_PHASE_ON, false);

        setValue(LFO_1_WAVEFORM, (int8_t)0);
        setValue(LFO_2_WAVEFORM, (int8_t)0);
        setValue(LFO_1_PULSE_WIDTH, 0.0f);
        setValue(LFO_2_PULSE_WIDTH, 0.0f);
        setValue(LFO_1_RATE, 0.5f);
        setValue(LFO_2_RATE, 0.5f);
        setValue(LFO_1_TRIGGER_RESET_ON, false);
        setValue(LFO_2_TRIGGER_RESET_ON, false);
        setValue(LFO_1_TRIGGER_PHASE, 0.5f);
        setValue(LFO_2_TRIGGER_PHASE, 0.5f);
        setValue(LFO_1_TO_MASTER_TUNE, 0.0f);
        setValue(LFO_2_TO_FLT_CUTOFF, 0.0f);

        setValue(SH_TO_FLT_CUTOFF, 0.0f);
        setValue(SH_RATE, 0.5f);

        setValue(MOD_1_SOURCE, (int8_t)0);
        setValue(MOD_2_SOURCE, (int8_t)0);
        setValue(MOD_3_SOURCE, (int8_t)0);
        setValue(MOD_4_SOURCE, (int8_t)0);
        setValue(MOD_5_SOURCE, (int8_t)0);
        setValue(MOD_6_SOURCE, (int8_t)0);
        setValue(MOD_7_SOURCE, (int8_t)0);
        setValue(MOD_8_SOURCE, (int8_t)0);

        setValue(MOD_1_DESTINATION, (int8_t)-1);
        setValue(MOD_2_DESTINATION, (int8_t)-1);
        setValue(MOD_3_DESTINATION, (int8_t)-1);
        setValue(MOD_4_DESTINATION, (int8_t)-1);
        setValue(MOD_5_DESTINATION, (int8_t)-1);
        setValue(MOD_6_DESTINATION, (int8_t)-1);
        setValue(MOD_7_DESTINATION, (int8_t)-1);
        setValue(MOD_8_DESTINATION, (int8_t)-1);

        setValue(MOD_1_DEPTH, 0.0f);
        setValue(MOD_2_DEPTH, 0.0f);
        setValue(MOD_3_DEPTH, 0.0f);
        setValue(MOD_4_DEPTH, 0.0f);
        setValue(MOD_5_DEPTH, 0.0f);
        setValue(MOD_6_DEPTH, 0.0f);
        setValue(MOD_7_DEPTH, 0.0f);
        setValue(MOD_8_DEPTH, 0.0f);

        setValue(FX_1, 0.0f);
        setValue(FX_2, 0.0f);
        setValue(FX_3, 0.0f);
        setValue(FX_4, 0.0f);
        setValue(FX_5, 0.0f);
        setValue(FX_REVERB, 0.0f);

        setValue(GEN_BALANCE, 0.5f);
    }

    void PatchSettings::SetControlsLive(bool isLive) {
        this->isLive = isLive;
        if (isLive) {
            memset(boolValuesProtected, 0, sizeof(int8_t) * NUM_PATCH_SETTINGS);
            memset(floatValuesProtected, 0, sizeof(int8_t) * NUM_PATCH_SETTINGS);
            floatProtectedUpdateNeeded = false;
        } else {
            memset(boolValuesProtected, 1, sizeof(int8_t) * NUM_PATCH_SETTINGS);
            memset(floatValuesProtected, 1, sizeof(int8_t) * NUM_PATCH_SETTINGS);
            floatProtectedUpdateNeeded = true;
            floatProtectedUpdateCounter = 0;
        }
    }

} // namespace kiwisynth
