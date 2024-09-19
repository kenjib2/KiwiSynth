#include "PatchSettings.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    // -------------------- BEGIN INITIALIZE STATIC ARRAYS --------------------
    static int8_t* genMaxIntValues() {
        int8_t* tmp = new int8_t[NUM_PATCH_SETTINGS];

        // Set the max values for int variables
        memset(tmp, 0, NUM_PATCH_SETTINGS * sizeof(int8_t));
        tmp[VCO_VOICES] = 2; // 0 is duo-polyphonic, 1 is monophonic, 2 is duo-timbral monophonic
        tmp[VCO_1_WAVEFORM] = 4;
        tmp[VCO_2_WAVEFORM] = 4;
        tmp[VCO_2_OCTAVE] = 4;
        tmp[VCO_2_INTERVAL] = 22;
        tmp[VCO_3_WAVEFORM] = 4;
        tmp[VCO_3_OCTAVE] = 4;
        tmp[VCO_3_INTERVAL] = 22;
        tmp[VCO_NOISE_TYPE] = 1;
        tmp[VCF_FILTER_TYPE] = 7;
        tmp[LFO_1_WAVEFORM] = 3;
        tmp[LFO_2_WAVEFORM] = 3;
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
        tmp[GEN_SELECT] = 50;
        tmp[GEN_FX_SELECT] = 127;
        tmp[GEN_REVERB_SELECT] = 2;

        return tmp;
    }
    int8_t *PatchSettings::maxIntValues = genMaxIntValues();

    static float* genLMinLookup() {
        float* tmp = new float[NUM_PATCH_SETTINGS];

        memset(tmp, 0, NUM_PATCH_SETTINGS * sizeof(float));
        tmp[VCO_PORTAMENTO_SPEED] = logf(0.0001f < 0.0000001f ? 0.0000001f : 0.0001f);
        tmp[FX_1] = logf(MIN_DISTORTION_GAIN < 0.0000001f ? 0.0000001f : MIN_DISTORTION_GAIN);
        tmp[LFO_1_RATE] = logf(0.1f < 0.0000001f ? 0.0000001f : 0.1f);
        tmp[LFO_2_RATE] = logf(0.1f < 0.0000001f ? 0.0000001f : 0.1f);
        tmp[SH_RATE] = logf(48.0f < 0.0000001f ? 0.0000001f : 48.0f);

        return tmp;
    }
    float *PatchSettings::lMinLookup = genLMinLookup();

    static float* genLMaxLookup() {
        float* tmp = new float[NUM_PATCH_SETTINGS];

        memset(tmp, 0, NUM_PATCH_SETTINGS * sizeof(float));
        tmp[VCO_PORTAMENTO_SPEED] = logf(0.05f);
        tmp[FX_1] = logf(MAX_DISTORTION_GAIN);
        tmp[LFO_1_RATE] = logf(20.0f);
        tmp[LFO_2_RATE] = logf(20.0f);
        tmp[SH_RATE] = logf(48000.0f);

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
        setValue(VCO_1_ON, true);

        // Set initial encoder values.
        setValue(VCO_VOICES, (int8_t)0);
        setValue(VCO_1_WAVEFORM, (int8_t)0);
        setValue(VCO_2_WAVEFORM, (int8_t)0);
        setValue(VCO_2_OCTAVE, (int8_t)2);
        setValue(VCO_2_INTERVAL, (int8_t)11);
        setValue(VCO_3_WAVEFORM, (int8_t)0);
        setValue(VCO_3_OCTAVE, (int8_t)2);
        setValue(VCO_3_INTERVAL, (int8_t)11);
        setValue(VCO_NOISE_TYPE, (int8_t)0);
        setValue(VCF_FILTER_TYPE, (int8_t)0);
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
        setValue(GEN_FX_SELECT, (int8_t)0);
        setValue(GEN_REVERB_SELECT, (int8_t)0);

        // Set external controllers in case they are not in use
        setValue(GEN_PITCH_BEND, 0.0f);
        setValue(GEN_AFTERTOUCH, 0.0f);
        setValue(GEN_MOD_WHEEL, 0.0f);
        setValue(GEN_EXPRESSION, 0.0f);
        setValue(GEN_SUSTAIN, 0.0f);

        DefaultSettings();
    }

    void PatchSettings::Copy(PatchSettings* patchSettings)
    {
        memcpy(boolValues, patchSettings->boolValues, sizeof(bool) * NUM_PATCH_SETTINGS);
        memcpy(intValues, patchSettings->intValues, sizeof(int8_t) * NUM_PATCH_SETTINGS);
        memcpy(floatValues, patchSettings->floatValues, sizeof(float) * NUM_PATCH_SETTINGS);
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
                    // Mod destination and gen select can go to -1
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
        switch(controlNumber)
        {
            case 0:
                setValue(VCF_ENV_2_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_3_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_2_DECAY, multiPots->GetMpValue(2, controlNumber));
                setValue(VCO_PORTAMENTO_SPEED, multiPots->GetDirectValue(0));
                break;
            case 1:
                setValue(SH_RATE, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_3_FINE_TUNE, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_2_SUSTAIN, multiPots->GetMpValue(2, controlNumber));
                break;
            case 2:
                setValue(MOD_3_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(VCA_ENV_1_DEPTH, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_2_RELEASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 3:
                setValue(MOD_4_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_2_FINE_TUNE, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_1_DECAY, multiPots->GetMpValue(2, controlNumber));
                break;
            case 4:
                setValue(MOD_2_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(VCA_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_1_SUSTAIN, multiPots->GetMpValue(2, controlNumber));
                break;
            case 5:
                setValue(MOD_1_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_2_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_1_RELEASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 6:
                setValue(FX_REVERB, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_MASTER_TUNE, multiPots->GetMpValue(1, controlNumber));
                setValue(VCF_ENV_1_DEPTH, multiPots->GetMpValue(2, controlNumber));
                break;
            case 7:
                setValue(FX_5, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_1_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(VCF_TRACKING, multiPots->GetMpValue(2, controlNumber));
                break;
            case 8:
                setValue(FX_4, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_1_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(VCO_EXT_LEVEL, multiPots->GetMpValue(2, controlNumber));
                break;
            case 9:
                setValue(GEN_BALANCE, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_2_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(VCO_EXT_TRIGGER_GATE, multiPots->GetMpValue(2, controlNumber));
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
                setValue(VCF_CUTOFF, multiPots->GetMpValue(0, controlNumber));
                setValue(LFO_1_RATE, multiPots->GetMpValue(1, controlNumber));
                setValue(LFO_2_TO_VCF_CUTOFF, multiPots->GetMpValue(2, controlNumber));
                break;
            case 14:
                setValue(VCF_RESONANCE, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_NOISE_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_2_ATTACK, multiPots->GetMpValue(2, controlNumber));
                break;
            case 15:
                setValue(SH_TO_VCF_CUTOFF, multiPots->GetMpValue(0, controlNumber));
                setValue(VCO_3_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(ENV_1_ATTACK, multiPots->GetMpValue(2, controlNumber));
                break;
        }
    }

    void PatchSettings::updateGpioExpansionValues(int controlNumber)
    {
        switch(controlNumber)
        {
            case 0x20:
                setValue(VCO_PORTAMENTO_ON, ge->getPinValue(controlNumber, 7));
                setValue(GEN_SELECT_BUTTON, ge->getPinValue(controlNumber, 11));
                setValue(ENV_2_REVERSE_PHASE_ON, ge->getPinValue(controlNumber, 14));
                setValue(ENV_1_REVERSE_PHASE_ON, ge->getPinValue(controlNumber, 15));
                processEncoder(MOD_1_SOURCE, controlNumber, 1, 2);
                processEncoder(VCO_3_INTERVAL, controlNumber, 4, 3);
                processEncoder(VCO_2_INTERVAL, controlNumber, 6, 5);
                processEncoder(GEN_SELECT, controlNumber, 9, 10);
                processEncoder(VCF_FILTER_TYPE, controlNumber, 12, 13);
                break;
            case 0x21:
                setValue(VCO_2_ON, ge->getPinValue(controlNumber, 5));
                processEncoder(VCO_2_WAVEFORM, controlNumber, 1, 2);
                processEncoder(VCO_3_WAVEFORM, controlNumber, 3, 4);
                processEncoder(VCO_VOICES, controlNumber, 10, 6);
                processEncoder(VCO_1_WAVEFORM, controlNumber, 11, 12);
                processEncoder(VCO_2_OCTAVE, controlNumber, 13, 14);
                processEncoder(VCO_3_OCTAVE, controlNumber, 15, 9);
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
                setValue(VCO_NOISE_ON, ge->getPinValue(controlNumber, 5));
                setValue(VCO_3_ON, ge->getPinValue(controlNumber, 6));
                setValue(VCO_EXT_ON, ge->getPinValue(controlNumber, 9));
                setValue(LFO_1_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 10));
                setValue(LFO_2_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 11));
                processEncoder(LFO_2_WAVEFORM, controlNumber, 2, 1);
                processEncoder(LFO_1_WAVEFORM, controlNumber, 4, 3);
                processEncoder(VCO_NOISE_TYPE, controlNumber, 13, 12);
                break;
        }
    }

    void PatchSettings::DefaultSettings()
    {
        setValue(VCO_VOICES, (int8_t)0);
        setValue(VCO_MASTER_TUNE, 0.5f);
        setValue(VCO_PORTAMENTO_ON, false);
        setValue(VCO_PORTAMENTO_SPEED, 0.5f);

        setValue(VCO_1_WAVEFORM, (int8_t)0);
        setValue(VCO_2_WAVEFORM, (int8_t)0);
        setValue(VCO_3_WAVEFORM, (int8_t)0);
        setValue(VCO_1_PULSE_WIDTH, 0.0f);
        setValue(VCO_2_PULSE_WIDTH, 0.0f);
        setValue(VCO_3_PULSE_WIDTH, 0.0f);
        setValue(VCO_1_LEVEL, 0.999f);
        setValue(VCO_2_LEVEL, 0.999f);
        setValue(VCO_3_LEVEL, 0.999f);
        setValue(VCO_1_ON, true);
        setValue(VCO_2_ON, false);
        setValue(VCO_3_ON, false);
        setValue(VCO_2_OCTAVE, (int8_t)2);
        setValue(VCO_3_OCTAVE, (int8_t)2);
        setValue(VCO_2_INTERVAL, (int8_t)11);
        setValue(VCO_3_INTERVAL, (int8_t)11);
        setValue(VCO_2_FINE_TUNE, 0.5f);
        setValue(VCO_3_FINE_TUNE, 0.5f);

        setValue(VCO_NOISE_ON, false);
        setValue(VCO_NOISE_TYPE, (int8_t)0);
        setValue(VCO_NOISE_LEVEL, 0.0f);

        setValue(VCO_EXT_ON, false);
        setValue(VCO_EXT_TRIGGER_GATE, 0.5f);
        setValue(VCO_EXT_LEVEL, 0.0f);

        setValue(VCF_FILTER_TYPE, (int8_t)0);
        setValue(VCF_CUTOFF, 0.999f);
        setValue(VCF_RESONANCE, 0.0f);
        setValue(VCF_TRACKING, 0.999f);
        setValue(VCF_ENV_1_DEPTH, 0.0f);
        setValue(VCF_ENV_2_DEPTH, 0.0f);

        setValue(VCA_LEVEL, 0.0f);
        setValue(VCA_ENV_1_DEPTH, 0.999f);

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
        setValue(LFO_2_TO_VCF_CUTOFF, 0.0f);

        setValue(SH_TO_VCF_CUTOFF, 0.0f);
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
        setValue(GEN_SELECT, (int8_t)0);
        setValue(GEN_SELECT_BUTTON, false);
        setValue(GEN_FX_SELECT, (int8_t)0);
        setValue(GEN_REVERB_SELECT, (int8_t)0);
        setValue(GEN_REVERB_DECAY, 0.5f);
        char buffer[4];
        strcpy(buffer, "New");
        setValue(GEN_NAME, buffer);
        setValue(GEN_AFTERTOUCH, 0.0f);
        setValue(GEN_MOD_WHEEL, 0.0f);
        setValue(GEN_PITCH_BEND, 0.0f);
        setValue(GEN_EXPRESSION, 0.0f);
        setValue(GEN_SUSTAIN, 0.0f);
    }

} // namespace kiwi_synth
