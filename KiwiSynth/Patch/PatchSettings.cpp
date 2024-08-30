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
        setValue(PatchSetting::MOD_1_SOURCE, (int8_t)-1);
        setValue(PatchSetting::MOD_1_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_2_SOURCE, (int8_t)-1);
        setValue(PatchSetting::MOD_2_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_3_SOURCE, (int8_t)-1);
        setValue(PatchSetting::MOD_3_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_4_SOURCE, (int8_t)-1);
        setValue(PatchSetting::MOD_4_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_5_SOURCE, (int8_t)-1);
        setValue(PatchSetting::MOD_5_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_6_SOURCE, (int8_t)-1);
        setValue(PatchSetting::MOD_6_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_7_SOURCE, (int8_t)-1);
        setValue(PatchSetting::MOD_7_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::MOD_8_SOURCE, (int8_t)-1);
        setValue(PatchSetting::MOD_8_DESTINATION, (int8_t)-1);
        setValue(PatchSetting::GEN_SELECT, (int8_t)0);
        setValue(PatchSetting::GEN_FX_SELECT, (int8_t)0);
        setValue(PatchSetting::GEN_REVERB_SELECT, (int8_t)0);
    }

    void PatchSettings::setValue(PatchSetting setting, int8_t value)
    {
        switch(setting) {
            case VCO_VOICES:
                intValues[0] = std::min((int)value, 2); // 2 Voice, 3 Voice (No Noise/SH), Mono, MultiTimbral Layered, MT Split Mono/Poly, MT Split Poly/Mono
                break;
            case VCO_1_WAVEFORM:
                intValues[1] = std::min((int)value, 2); // Blep Sq, Blep Saw, Blep Tri
                break;
            case VCO_2_WAVEFORM:
                intValues[2] = std::min((int)value, 2);
                break;
            case VCO_2_OCTAVE:
                intValues[3] = std::min((int)value, 4); // -2 to +2 Octaves
                break;
            case VCO_2_INTERVAL:
                intValues[4] = std::min((int)value, 22); // -11 to +11 Semitones
                break;
            case VCO_3_WAVEFORM:
                intValues[5] = std::min((int)value, 2);
                break;
            case VCO_3_OCTAVE:
                intValues[6] = std::min((int)value, 4);
                break;
            case VCO_3_INTERVAL:
                intValues[7] = std::min((int)value, 22);
                break;
            case VCO_NOISE_TYPE:
                intValues[8] = std::min((int)value, 1); // White, Dust
                break;
            case VCF_FILTER_TYPE:
                intValues[9] = std::min((int)value, 7); // Ladder Lowpass, 1 Pole Lowpass, 1 Pole Highpass, SVF Lowpass, SVF Highpass, SVF Bandpass, SVF Notch, SVF Peak
                break;
            case LFO_1_WAVEFORM:
                intValues[10] = std::min((int)value, 3); // Tri, Sq, Ramp, Saw
                break;
            case LFO_2_WAVEFORM:
                intValues[11] = std::min((int)value, 3); // Tri, Sq, Ramp, Saw
                break;
            case MOD_1_SOURCE:
                intValues[12] = std::min((int)value, NUM_MOD_SOURCES - 1);
                break;
            case MOD_1_DESTINATION:
                intValues[13] = std::min((int)value, NUM_MOD_DESTINATIONS - 1);
                break;
            case MOD_2_SOURCE:
                intValues[14] = std::min((int)value, NUM_MOD_SOURCES - 1);
                break;
            case MOD_2_DESTINATION:
                intValues[15] = std::min((int)value, NUM_MOD_DESTINATIONS - 1);
                break;
            case MOD_3_SOURCE:
                intValues[16] = std::min((int)value, NUM_MOD_SOURCES - 1);
                break;
            case MOD_3_DESTINATION:
                intValues[17] = std::min((int)value, NUM_MOD_DESTINATIONS - 1);
                break;
            case MOD_4_SOURCE:
                intValues[18] = std::min((int)value, NUM_MOD_SOURCES - 1);
                break;
            case MOD_4_DESTINATION:
                intValues[19] = std::min((int)value, NUM_MOD_DESTINATIONS - 1);
                break;
            case MOD_5_SOURCE:
                intValues[20] = std::min((int)value, NUM_MOD_SOURCES - 1);
                break;
            case MOD_5_DESTINATION:
                intValues[21] = std::min((int)value, NUM_MOD_DESTINATIONS - 1);
                break;
            case MOD_6_SOURCE:
                intValues[22] = std::min((int)value, NUM_MOD_SOURCES - 1);
                break;
            case MOD_6_DESTINATION:
                intValues[23] = std::min((int)value, NUM_MOD_DESTINATIONS - 1);
                break;
            case MOD_7_SOURCE:
                intValues[24] = std::min((int)value, NUM_MOD_SOURCES - 1);
                break;
            case MOD_7_DESTINATION:
                intValues[25] = std::min((int)value, NUM_MOD_DESTINATIONS - 1);
                break;
            case MOD_8_SOURCE:
                intValues[26] = std::min((int)value, NUM_MOD_SOURCES - 1);
                break;
            case MOD_8_DESTINATION:
                intValues[27] = std::min((int)value, NUM_MOD_DESTINATIONS - 1);
                break;
            case GEN_SELECT:
                intValues[28] = value;
                break;
            case GEN_FX_SELECT:
                intValues[29] = std::min((int)value, 8);
                break;
            case GEN_REVERB_SELECT:
                intValues[30] = std::min((int)value, 2); // Room, Hall, Cathedral
                break;
            default:
                break;
        }
    }


    void PatchSettings::setValue(PatchSetting setting, float value)
    {
        switch(setting) {
            case VCO_MASTER_TUNE:
                floatValues[0] = value;
                break;
            case VCO_PORTAMENTO_SPEED:
                floatValues[1] = value;
                break;
            case VCO_1_PULSE_WIDTH:
                floatValues[2] = value;
                break;
            case VCO_1_LEVEL:
                floatValues[3] = value;
                break;
            case VCO_2_PULSE_WIDTH:
                floatValues[4] = value;
                break;
            case VCO_2_LEVEL:
                floatValues[5] = value;
                break;
            case VCO_2_FINE_TUNE:
                floatValues[6] = value;
                break;
            case VCO_3_PULSE_WIDTH:
                floatValues[7] = value;
                break;
            case VCO_3_LEVEL:
                floatValues[8] = value;
                break;
            case VCO_3_FINE_TUNE:
                floatValues[9] = value;
                break;
            case VCO_NOISE_LEVEL:
                floatValues[10] = value;
                break;
            case VCO_EXT_TRIGGER_GATE:
                floatValues[11] = value;
                break;
            case VCO_EXT_LEVEL:
                floatValues[12] = value;
                break;
            case VCF_CUTOFF:
                floatValues[13] = value;
                break;
            case VCF_RESONANCE:
                floatValues[14] = value;
                break;
            case VCF_TRACKING:
                floatValues[15] = value;
                break;
            case VCF_ENV_1_DEPTH:
                floatValues[16] = value;
                break;
            case VCF_ENV_2_DEPTH:
                floatValues[17] = value;
                break;
            case VCA_LEVEL:
                floatValues[18] = value;
                break;
            case VCA_ENV_1_DEPTH:
                floatValues[19] = value;
                break;
            case ENV_1_ATTACK:
                floatValues[20] = value;
                break;
            case ENV_1_DECAY:
                floatValues[21] = value;
                break;
            case ENV_1_SUSTAIN:
                floatValues[22] = value;
                break;
            case ENV_1_RELEASE:
                floatValues[23] = value;
                break;
            case ENV_2_ATTACK:
                floatValues[24] = value;
                break;
            case ENV_2_DECAY:
                floatValues[25] = value;
                break;
            case ENV_2_SUSTAIN:
                floatValues[26] = value;
                break;
            case ENV_2_RELEASE:
                floatValues[27] = value;
                break;
            case LFO_1_PULSE_WIDTH:
                floatValues[28] = value;
                break;
            case LFO_1_RATE:
                floatValues[29] = value;
                break;
            case LFO_1_TRIGGER_PHASE:
                floatValues[30] = value;
                break;
            case LFO_1_TO_MASTER_TUNE:
                floatValues[31] = value;
                break;
            case LFO_2_PULSE_WIDTH:
                floatValues[32] = value;
                break;
            case LFO_2_RATE:
                floatValues[33] = value;
                break;
            case LFO_2_TRIGGER_PHASE:
                floatValues[34] = value;
                break;
            case LFO_2_TO_VCF_CUTOFF:
                floatValues[35] = value;
                break;
            case SH_TO_VCF_CUTOFF:
                floatValues[36] = value;
                break;
            case SH_RATE:
                floatValues[37] = value;
                break;
            case MOD_1_DEPTH:
                floatValues[38] = value;
                break;
            case MOD_2_DEPTH:
                floatValues[39] = value;
                break;
            case MOD_3_DEPTH:
                floatValues[40] = value;
                break;
            case MOD_4_DEPTH:
                floatValues[41] = value;
                break;
            case MOD_5_DEPTH:
                floatValues[42] = value;
                break;
            case MOD_6_DEPTH:
                floatValues[43] = value;
                break;
            case MOD_7_DEPTH:
                floatValues[44] = value;
                break;
            case MOD_8_DEPTH:
                floatValues[45] = value;
                break;
            case FX_1:
                floatValues[46] = value;
                break;
            case FX_2:
                floatValues[47] = value;
                break;
            case FX_3:
                floatValues[48] = value;
                break;
            case FX_4:
                floatValues[49] = value;
                break;
            case FX_5:
                floatValues[50] = value;
                break;
            case FX_REVERB:
                floatValues[51] = value;
                break;
            case GEN_BALANCE:
                floatValues[52] = value;
                break;
            case GEN_REVERB_DECAY:
                floatValues[53] = value;
                break;
            default:
                break;
        }
    }

    void PatchSettings::setValue(PatchSetting setting, bool value)
    {
        switch(setting) {
            case VCO_PORTAMENTO_ON:
                boolValues[0] = value;
                break;
            case VCO_2_ON:
                boolValues[1] = value;
                break;
            case VCO_3_ON:
                boolValues[2] = value;
                break;
            case VCO_NOISE_ON:
                boolValues[3] = value;
                break;
            case VCO_EXT_ON:
                boolValues[4] = value;
                break;
            case LFO_1_TRIGGER_RESET_ON:
                boolValues[5] = value;
                break;
            case LFO_2_TRIGGER_RESET_ON:
                boolValues[6] = value;
                break;
            case ENV_1_REVERSE_PHASE_ON:
                boolValues[7] = value;
                break;
            case ENV_2_REVERSE_PHASE_ON:
                boolValues[8] = value;
                break;
            case GEN_SELECT_BUTTON:
                boolValues[9] = value;
                break;
            default:
                break;
        }
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
        switch(setting) {
            case VCO_VOICES:
                return intValues[0];
            case VCO_1_WAVEFORM:
                return intValues[1];
            case VCO_2_WAVEFORM:
                return intValues[2];
            case VCO_2_OCTAVE:
                return intValues[3];
            case VCO_2_INTERVAL:
                return intValues[4];
            case VCO_3_WAVEFORM:
                return intValues[5];
            case VCO_3_OCTAVE:
                return intValues[6];
            case VCO_3_INTERVAL:
                return intValues[7];
            case VCO_NOISE_TYPE:
                return intValues[8];
            case VCF_FILTER_TYPE:
                return intValues[9];
            case LFO_1_WAVEFORM:
                return intValues[10];
            case LFO_2_WAVEFORM:
                return intValues[11];
            case MOD_1_SOURCE:
                return intValues[12];
            case MOD_1_DESTINATION:
                return intValues[13];
            case MOD_2_SOURCE:
                return intValues[14];
            case MOD_2_DESTINATION:
                return intValues[15];
            case MOD_3_SOURCE:
                return intValues[16];
            case MOD_3_DESTINATION:
                return intValues[17];
            case MOD_4_SOURCE:
                return intValues[18];
            case MOD_4_DESTINATION:
                return intValues[19];
            case MOD_5_SOURCE:
                return intValues[20];
            case MOD_5_DESTINATION:
                return intValues[21];
            case MOD_6_SOURCE:
                return intValues[22];
            case MOD_6_DESTINATION:
                return intValues[23];
            case MOD_7_SOURCE:
                return intValues[24];
            case MOD_7_DESTINATION:
                return intValues[25];
            case MOD_8_SOURCE:
                return intValues[26];
            case MOD_8_DESTINATION:
                return intValues[27];
            case GEN_SELECT:
                return intValues[28];
            case GEN_FX_SELECT:
                return intValues[29];
            case GEN_REVERB_SELECT:
                return intValues[30];
            default:
                return 0;
        }
    }

    float PatchSettings::getFloatValue(PatchSetting setting) {
        switch(setting) {
            case VCO_MASTER_TUNE:
                return floatValues[0];
            case VCO_PORTAMENTO_SPEED:
                return floatValues[1];
            case VCO_1_PULSE_WIDTH:
                return floatValues[2];
            case VCO_1_LEVEL:
                return floatValues[3];
            case VCO_2_PULSE_WIDTH:
                return floatValues[4];
            case VCO_2_LEVEL:
                return floatValues[5];
            case VCO_2_OCTAVE:
                return (float)(intValues[3]) / 4.0f;
            case VCO_2_INTERVAL:
                return (float)(intValues[4]) / 22.0f;
            case VCO_2_FINE_TUNE:
                return floatValues[6];
            case VCO_3_PULSE_WIDTH:
                return floatValues[7];
            case VCO_3_LEVEL:
                return floatValues[8];
            case VCO_3_OCTAVE:
                return (float)intValues[6] / 4.0f;
            case VCO_3_INTERVAL:
                return (float)(intValues[7]) / 22.0f;
            case VCO_3_FINE_TUNE:
                return floatValues[9];
            case VCO_NOISE_LEVEL:
                return floatValues[10];
            case VCO_EXT_TRIGGER_GATE:
                return floatValues[11];
            case VCO_EXT_LEVEL:
                return floatValues[12];
            case VCF_CUTOFF:
                return floatValues[13];
            case VCF_RESONANCE:
                return floatValues[14];
            case VCF_TRACKING:
                return floatValues[15];
            case VCF_ENV_1_DEPTH:
                return floatValues[16];
            case VCF_ENV_2_DEPTH:
                return floatValues[17];
            case VCA_LEVEL:
                return floatValues[18];
            case VCA_ENV_1_DEPTH:
                return floatValues[19];
            case ENV_1_ATTACK:
                return floatValues[20];
            case ENV_1_DECAY:
                return floatValues[21];
            case ENV_1_SUSTAIN:
                return floatValues[22];
            case ENV_1_RELEASE:
                return floatValues[23];
            case ENV_2_ATTACK:
                return floatValues[24];
            case ENV_2_DECAY:
                return floatValues[25];
            case ENV_2_SUSTAIN:
                return floatValues[26];
            case ENV_2_RELEASE:
                return floatValues[27];
            case LFO_1_PULSE_WIDTH:
                return floatValues[28];
            case LFO_1_RATE:
                return floatValues[29];
            case LFO_1_TRIGGER_PHASE:
                return floatValues[30];
            case LFO_1_TO_MASTER_TUNE:
                return floatValues[31];
            case LFO_2_PULSE_WIDTH:
                return floatValues[32];
            case LFO_2_RATE:
                return floatValues[33];
            case LFO_2_TRIGGER_PHASE:
                return floatValues[34];
            case LFO_2_TO_VCF_CUTOFF:
                return floatValues[35];
            case SH_TO_VCF_CUTOFF:
                return floatValues[36];
            case SH_RATE:
                return floatValues[37];
            case MOD_1_DEPTH:
                return floatValues[38];
            case MOD_2_DEPTH:
                return floatValues[39];
            case MOD_3_DEPTH:
                return floatValues[40];
            case MOD_4_DEPTH:
                return floatValues[41];
            case MOD_5_DEPTH:
                return floatValues[42];
            case MOD_6_DEPTH:
                return floatValues[43];
            case MOD_7_DEPTH:
                return floatValues[44];
            case MOD_8_DEPTH:
                return floatValues[45];
            case FX_1:
                return floatValues[46];
            case FX_2:
                return floatValues[47];
            case FX_3:
                return floatValues[48];
            case FX_4:
                return floatValues[49];
            case FX_5:
                return floatValues[50];
            case FX_REVERB:
                return floatValues[51];
            case GEN_BALANCE:
                return floatValues[52];
            case GEN_REVERB_DECAY:
                return floatValues[53];
            case GEN_AFTERTOUCH:
                return floatValues[54];
            case GEN_MOD_WHEEL:
                return floatValues[55];
            case GEN_PITCH_BEND:
                return floatValues[56];
            case GEN_EXPRESSION:
                return floatValues[57];
            default:
                return 0.0f;
        }
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

    float PatchSettings::octaveToFrequencyMultiplier(int octave)
    {
        if (octave == 0) {
            return 1.0F;
        } else if (octave > 0) {
            float multiplier = 1.0f;
            for (int i = 0; i < octave; i++) {
                multiplier *= 2.0f;
            }
            return multiplier;
        } else if (octave < 0) {
            float multiplier = 1.0F;
            for (int i = 0; i > octave; i--) {
                multiplier /= 2.0f;
            }
            return multiplier;
        }

        return 1.0F;

        //return pow(2, octave);
    }

    bool PatchSettings::getBoolValue(PatchSetting setting)
    {
        switch(setting) {
            case VCO_PORTAMENTO_ON:
                return boolValues[0];
            case VCO_2_ON:
                return boolValues[1];
            case VCO_3_ON:
                return boolValues[2];
            case VCO_NOISE_ON:
                return boolValues[3];
            case VCO_EXT_ON:
                return boolValues[4];
            case LFO_1_TRIGGER_RESET_ON:
                return boolValues[5];
            case LFO_2_TRIGGER_RESET_ON:
                return boolValues[6];
            case ENV_1_REVERSE_PHASE_ON:
                return boolValues[7];
            case ENV_2_REVERSE_PHASE_ON:
                return boolValues[8];
            case GEN_SELECT_BUTTON:
                return boolValues[9];
            default:
                return false;
        }
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
                } else if (currentValue == 0 && setting >= MOD_1_SOURCE && setting <= MOD_8_DESTINATION) {
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
