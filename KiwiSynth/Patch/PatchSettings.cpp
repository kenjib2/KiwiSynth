#include "PatchSettings.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
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

        // Set the max values for int variables
        memset(&maxIntValues, 0, NUM_PATCH_SETTINGS * sizeof(int8_t));
        maxIntValues[VCO_VOICES] = 2;
        maxIntValues[VCO_1_WAVEFORM] = 2;
        maxIntValues[VCO_2_WAVEFORM] = 2;
        maxIntValues[VCO_2_OCTAVE] = 4;
        maxIntValues[VCO_2_INTERVAL] = 22;
        maxIntValues[VCO_3_WAVEFORM] = 2;
        maxIntValues[VCO_3_OCTAVE] = 4;
        maxIntValues[VCO_3_INTERVAL] = 22;
        maxIntValues[VCO_NOISE_TYPE] = 1;
        maxIntValues[VCF_FILTER_TYPE] = 7;
        maxIntValues[LFO_1_WAVEFORM] = 3;
        maxIntValues[LFO_2_WAVEFORM] = 3;
        maxIntValues[MOD_1_SOURCE] = NUM_MOD_SOURCES - 1;
        maxIntValues[MOD_1_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        maxIntValues[MOD_2_SOURCE] = NUM_MOD_SOURCES - 1;
        maxIntValues[MOD_2_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        maxIntValues[MOD_3_SOURCE] = NUM_MOD_SOURCES - 1;
        maxIntValues[MOD_3_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        maxIntValues[MOD_4_SOURCE] = NUM_MOD_SOURCES - 1;
        maxIntValues[MOD_4_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        maxIntValues[MOD_5_SOURCE] = NUM_MOD_SOURCES - 1;
        maxIntValues[MOD_5_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        maxIntValues[MOD_6_SOURCE] = NUM_MOD_SOURCES - 1;
        maxIntValues[MOD_6_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        maxIntValues[MOD_7_SOURCE] = NUM_MOD_SOURCES - 1;
        maxIntValues[MOD_7_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        maxIntValues[MOD_8_SOURCE] = NUM_MOD_SOURCES - 1;
        maxIntValues[MOD_8_DESTINATION] = NUM_MOD_DESTINATIONS - 1;
        maxIntValues[GEN_SELECT] = 50;
        maxIntValues[GEN_FX_SELECT] = 8;
        maxIntValues[GEN_REVERB_SELECT] = 2;

        // Set initial encoder values.
        setValue(PatchSetting::VCO_VOICES, (int8_t)0);
        setValue(PatchSetting::VCO_1_WAVEFORM, (int8_t)0);
        setValue(PatchSetting::VCO_2_WAVEFORM, (int8_t)0);
        setValue(PatchSetting::VCO_2_OCTAVE, (int8_t)2);
        setValue(PatchSetting::VCO_2_INTERVAL, (int8_t)11);
        setValue(PatchSetting::VCO_3_WAVEFORM, (int8_t)0);
        setValue(PatchSetting::VCO_3_OCTAVE, (int8_t)2);
        setValue(PatchSetting::VCO_3_INTERVAL, (int8_t)11);
        setValue(PatchSetting::VCO_NOISE_TYPE, (int8_t)0);
        setValue(PatchSetting::VCF_FILTER_TYPE, (int8_t)0);
        setValue(PatchSetting::LFO_1_WAVEFORM, (int8_t)0);
        setValue(PatchSetting::LFO_2_WAVEFORM, (int8_t)0);
        setValue(PatchSetting::MOD_1_SOURCE, (int8_t)0);
        setValue(PatchSetting::MOD_1_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_2_SOURCE, (int8_t)0);
        setValue(PatchSetting::MOD_2_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_3_SOURCE, (int8_t)0);
        setValue(PatchSetting::MOD_3_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_4_SOURCE, (int8_t)0);
        setValue(PatchSetting::MOD_4_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_5_SOURCE, (int8_t)0);
        setValue(PatchSetting::MOD_5_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_6_SOURCE, (int8_t)0);
        setValue(PatchSetting::MOD_6_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_7_SOURCE, (int8_t)0);
        setValue(PatchSetting::MOD_7_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_8_SOURCE, (int8_t)0);
        setValue(PatchSetting::MOD_8_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::GEN_SELECT, (int8_t)0);
        setValue(PatchSetting::GEN_FX_SELECT, (int8_t)0);
        setValue(PatchSetting::GEN_REVERB_SELECT, (int8_t)0);
        setValue(PatchSetting::GEN_PITCH_BEND, 0.0f);
        setValue(PatchSetting::GEN_AFTERTOUCH, 0.0f);
        setValue(PatchSetting::GEN_MOD_WHEEL, 0.0f);
        setValue(PatchSetting::GEN_EXPRESSION, 0.0f);
        setValue(PatchSetting::GEN_SUSTAIN, 0.0f);
    }

    void PatchSettings::setValue(PatchSetting setting, int8_t value)
    {
        if (value != intValues[setting]) {
            lastChangedSetting = setting;
            lastChangedValue = value;
        }
        intValues[setting] = std::min(value, maxIntValues[setting]);
    }


    void PatchSettings::setValue(PatchSetting setting, float value)
    {
        floatValues[setting] = value;
    }

    void PatchSettings::setValue(PatchSetting setting, bool value)
    {
        boolValues[setting] = value;
    }

    void PatchSettings::setValue(PatchSetting setting, std::string value)
    {
        switch(setting) {
            case GEN_NAME:
                if (value.length() > MAX_PATCH_NAME_LENGTH) {
                    value = value.substr(0, MAX_PATCH_NAME_LENGTH);
                }
                strcpy(name, value.c_str());
                break;
            default:
                break;
        }
    }

    int8_t PatchSettings::getIntValue(PatchSetting setting)
    {
        return intValues[setting];
    }

    float PatchSettings::getFloatValue(PatchSetting setting) {
        return floatValues[setting];
    }

    float PatchSettings::getFloatValue(PatchSetting setting, Scale scale)
    {
        float value = getFloatValue(setting);

        switch(scale)
        {
            case EXPONENTIAL:
                value = value * value;
                break;
            case OCTAVE:
                {
                    // For plus or minus one octave we want 2^(2x-1)
                    // More generally it's 2^(rangex - range/2)
                    // This only works if max + min = 0
                    value = pow(2, value);

                    break;
                }
            case LOGARHITHMIC:
                value = expf(value);
                break;
            case LINEAR:
            default:
                value = value;
                break;
        }

        return value;
    }

    float PatchSettings::getFloatValue(PatchSetting setting, float min, float max, Scale scale)
    {
        float value = getFloatValue(setting);

        float lmin;
        float lmax;
        switch(scale)
        {
            case EXPONENTIAL:
                value = value * value * (max - min) + min;
                break;
            case OCTAVE:
                {
                    float range = max - min;
                    // For plus or minus one octave we want 2^(2x-1)
                    // More generally it's 2^(rangex - range/2)
                    // This only works if max + min = 0
                    value = pow(2, range * value - range / 2);

                    break;
                }
            case LOGARHITHMIC:
                lmin = logf(min < 0.0000001f ? 0.0000001f : min);
                lmax = logf(max);
                value = expf((value * (lmax - lmin)) + lmin);
                break;
            case LINEAR:
            default:
                value = value * (max - min) + min;
                break;
        }

        return value;
    }

    bool PatchSettings::getBoolValue(PatchSetting setting)
    {
        return boolValues[setting];
    }

    std::string PatchSettings::getStringValue(PatchSetting setting)
    {
        switch(setting) {
            case GEN_NAME:
                return std::string(name);
            default:
                return "";
        }
    }

    void PatchSettings::controlUpdate(int controlNumber, int controlId)
    {
        switch (controlId)
        {
            case ControlId::MULTIPOTS:
                updatePotValues(controlNumber);
                break;
            case ControlId::GPIO_EXPANSION:
                updateGpioExpansionValues(controlNumber);
            default:
                break;
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
                } else if (currentValue == 0 && setting >= MOD_1_DESTINATION && setting <= MOD_8_DESTINATION) {
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
                setValue(PatchSetting::VCF_ENV_2_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_3_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::ENV_2_DECAY, multiPots->GetMpValue(2, controlNumber));
                setValue(PatchSetting::VCO_PORTAMENTO_SPEED, multiPots->GetDirectValue(0));
                break;
            case 1:
                setValue(PatchSetting::SH_RATE, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_3_FINE_TUNE, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::ENV_2_SUSTAIN, multiPots->GetMpValue(2, controlNumber));
                break;
            case 2:
                setValue(PatchSetting::MOD_3_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCA_ENV_1_DEPTH, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::ENV_2_RELEASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 3:
                setValue(PatchSetting::MOD_4_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_2_FINE_TUNE, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::ENV_1_DECAY, multiPots->GetMpValue(2, controlNumber));
                break;
            case 4:
                setValue(PatchSetting::MOD_2_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCA_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::ENV_1_SUSTAIN, multiPots->GetMpValue(2, controlNumber));
                break;
            case 5:
                setValue(PatchSetting::MOD_1_DEPTH, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_2_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::ENV_1_RELEASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 6:
                setValue(PatchSetting::FX_REVERB, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_MASTER_TUNE, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::VCF_ENV_1_DEPTH, multiPots->GetMpValue(2, controlNumber));
                break;
            case 7:
                setValue(PatchSetting::FX_5, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_1_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::VCF_TRACKING, multiPots->GetMpValue(2, controlNumber));
                break;
            case 8:
                setValue(PatchSetting::FX_4, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_1_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::VCO_EXT_LEVEL, multiPots->GetMpValue(2, controlNumber));
                break;
            case 9:
                setValue(PatchSetting::GEN_BALANCE, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_2_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::VCO_EXT_TRIGGER_GATE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 10:
                setValue(PatchSetting::FX_3, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::LFO_1_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::LFO_1_TRIGGER_PHASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 11:
                setValue(PatchSetting::FX_2, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::LFO_2_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::LFO_2_TRIGGER_PHASE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 12:
                setValue(PatchSetting::FX_1, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::LFO_2_RATE, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::LFO_1_TO_MASTER_TUNE, multiPots->GetMpValue(2, controlNumber));
                break;
            case 13:
                setValue(PatchSetting::VCF_CUTOFF, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::LFO_1_RATE, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::LFO_2_TO_VCF_CUTOFF, multiPots->GetMpValue(2, controlNumber));
                break;
            case 14:
                setValue(PatchSetting::VCF_RESONANCE, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_NOISE_LEVEL, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::ENV_2_ATTACK, multiPots->GetMpValue(2, controlNumber));
                break;
            case 15:
                setValue(PatchSetting::SH_TO_VCF_CUTOFF, multiPots->GetMpValue(0, controlNumber));
                setValue(PatchSetting::VCO_3_PULSE_WIDTH, multiPots->GetMpValue(1, controlNumber));
                setValue(PatchSetting::ENV_1_ATTACK, multiPots->GetMpValue(2, controlNumber));
                break;
        }
    }

    void PatchSettings::updateGpioExpansionValues(int controlNumber)
    {
        switch(controlNumber)
        {
            case 0x20:
                setValue(PatchSetting::VCO_PORTAMENTO_ON, ge->getPinValue(controlNumber, 7));
                setValue(PatchSetting::GEN_SELECT_BUTTON, ge->getPinValue(controlNumber, 11));
                setValue(PatchSetting::ENV_2_REVERSE_PHASE_ON, ge->getPinValue(controlNumber, 14));
                setValue(PatchSetting::ENV_1_REVERSE_PHASE_ON, ge->getPinValue(controlNumber, 15));
                processEncoder(PatchSetting::MOD_1_SOURCE, controlNumber, 1, 2);
                processEncoder(PatchSetting::VCO_3_INTERVAL, controlNumber, 4, 3);
                processEncoder(PatchSetting::VCO_2_INTERVAL, controlNumber, 6, 5);
                processEncoder(PatchSetting::GEN_SELECT, controlNumber, 9, 10);
                processEncoder(PatchSetting::VCF_FILTER_TYPE, controlNumber, 12, 13);
                break;
            case 0x21:
                setValue(PatchSetting::VCO_2_ON, ge->getPinValue(controlNumber, 5));
                processEncoder(PatchSetting::VCO_2_WAVEFORM, controlNumber, 1, 2);
                processEncoder(PatchSetting::VCO_3_WAVEFORM, controlNumber, 3, 4);
                processEncoder(PatchSetting::VCO_VOICES, controlNumber, 10, 6);
                processEncoder(PatchSetting::VCO_1_WAVEFORM, controlNumber, 11, 12);
                processEncoder(PatchSetting::VCO_2_OCTAVE, controlNumber, 13, 14);
                processEncoder(PatchSetting::VCO_3_OCTAVE, controlNumber, 15, 9);
                break;
            case 0x22:
                processEncoder(PatchSetting::MOD_4_SOURCE, controlNumber, 2, 1);
                processEncoder(PatchSetting::MOD_3_SOURCE, controlNumber, 4, 3);
                processEncoder(PatchSetting::MOD_2_SOURCE, controlNumber, 6, 5);
                processEncoder(PatchSetting::MOD_1_DESTINATION, controlNumber, 9, 7);
                processEncoder(PatchSetting::MOD_2_DESTINATION, controlNumber, 10, 11);
                processEncoder(PatchSetting::MOD_3_DESTINATION, controlNumber, 12, 13);
                processEncoder(PatchSetting::MOD_4_DESTINATION, controlNumber, 15, 14);
                break;
            case 0x23:
                setValue(PatchSetting::VCO_NOISE_ON, ge->getPinValue(controlNumber, 5));
                setValue(PatchSetting::VCO_3_ON, ge->getPinValue(controlNumber, 6));
                setValue(PatchSetting::VCO_EXT_ON, ge->getPinValue(controlNumber, 9));
                setValue(PatchSetting::LFO_1_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 10));
                setValue(PatchSetting::LFO_2_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 11));
                processEncoder(PatchSetting::LFO_2_WAVEFORM, controlNumber, 2, 1);
                processEncoder(PatchSetting::LFO_1_WAVEFORM, controlNumber, 4, 3);
                processEncoder(PatchSetting::VCO_NOISE_TYPE, controlNumber, 13, 12);
                break;
        }
    }

} // namespace kiwi_synth
