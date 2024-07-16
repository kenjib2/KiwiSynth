#include "PatchSettings.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    void PatchSettings::Init(MultiPots* multiPots, GpioExpansion* ge)
    {
        this->multiPots = multiPots;
        this->ge = ge;
    }

    void PatchSettings::setValue(PatchSetting setting, uint8_t value)
    {
        switch(setting) {
            case VCO_VOICES:
                intValues[0] = value;
                break;
            case VCO_1_WAVEFORM:
                intValues[1] = value;
                break;
            case VCO_2_WAVEFORM:
                intValues[2] = value;
                break;
            case VCO_2_OCTAVE:
                intValues[3] = value;
                break;
            case VCO_2_INTERVAL:
                intValues[4] = value;
                break;
            case VCO_3_WAVEFORM:
                intValues[5] = value;
                break;
            case VCO_3_OCTAVE:
                intValues[6] = value;
                break;
            case VCO_3_INTERVAL:
                intValues[7] = value;
                break;
            case VCF_FILTER_TYPE:
                intValues[8] = value;
                break;
            case LFO_1_WAVEFORM:
                intValues[9] = value;
                break;
            case LFO_2_WAVEFORM:
                intValues[10] = value;
                break;
            case MOD_1_SOURCE:
                intValues[11] = value;
                break;
            case MOD_1_DESTINATION:
                intValues[12] = value;
                break;
            case MOD_2_SOURCE:
                intValues[13] = value;
                break;
            case MOD_2_DESTINATION:
                intValues[14] = value;
                break;
            case MOD_3_SOURCE:
                intValues[15] = value;
                break;
            case MOD_3_DESTINATION:
                intValues[16] = value;
                break;
            case MOD_4_SOURCE:
                intValues[17] = value;
                break;
            case MOD_4_DESTINATION:
                intValues[18] = value;
                break;
            case MOD_5_SOURCE:
                intValues[19] = value;
                break;
            case MOD_5_DESTINATION:
                intValues[20] = value;
                break;
            case MOD_6_SOURCE:
                intValues[21] = value;
                break;
            case MOD_6_DESTINATION:
                intValues[22] = value;
                break;
            case MOD_7_SOURCE:
                intValues[23] = value;
                break;
            case MOD_7_DESTINATION:
                intValues[24] = value;
                break;
            case MOD_8_SOURCE:
                intValues[25] = value;
                break;
            case MOD_8_DESTINATION:
                intValues[26] = value;
                break;
            case GEN_SELECT:
                intValues[27] = value;
                break;
            case GEN_FX_SELECT:
                intValues[28] = value;
                break;
            case GEN_REVERB_SELECT:
                intValues[29] = value;
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
            case SH_LEVEL:
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

    uint8_t PatchSettings::getIntValue(PatchSetting setting)
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
            case VCF_FILTER_TYPE:
                return intValues[8];
            case LFO_1_WAVEFORM:
                return intValues[9];
            case LFO_2_WAVEFORM:
                return intValues[10];
            case MOD_1_SOURCE:
                return intValues[11];
            case MOD_1_DESTINATION:
                return intValues[12];
            case MOD_2_SOURCE:
                return intValues[13];
            case MOD_2_DESTINATION:
                return intValues[14];
            case MOD_3_SOURCE:
                return intValues[15];
            case MOD_3_DESTINATION:
                return intValues[16];
            case MOD_4_SOURCE:
                return intValues[17];
            case MOD_4_DESTINATION:
                return intValues[18];
            case MOD_5_SOURCE:
                return intValues[19];
            case MOD_5_DESTINATION:
                return intValues[20];
            case MOD_6_SOURCE:
                return intValues[21];
            case MOD_6_DESTINATION:
                return intValues[22];
            case MOD_7_SOURCE:
                return intValues[23];
            case MOD_7_DESTINATION:
                return intValues[24];
            case MOD_8_SOURCE:
                return intValues[25];
            case MOD_8_DESTINATION:
                return intValues[26];
            case GEN_SELECT:
                return intValues[27];
            case GEN_FX_SELECT:
                return intValues[28];
            case GEN_REVERB_SELECT:
                return intValues[29];
            default:
                return 0;
        }
    }

    float PatchSettings::getFloatValue(PatchSetting setting, float min, float max, Scale scale)
    {
        float value;

        switch(setting) {
            case VCO_MASTER_TUNE:
                value = floatValues[0];
                break;
            case VCO_PORTAMENTO_SPEED:
                value = floatValues[1];
                break;
            case VCO_1_PULSE_WIDTH:
                value = floatValues[2];
                break;
            case VCO_1_LEVEL:
                value = floatValues[3];
                break;
            case VCO_2_PULSE_WIDTH:
                value = floatValues[4];
                break;
            case VCO_2_LEVEL:
                value = floatValues[5];
                break;
            case VCO_2_FINE_TUNE:
                value = floatValues[6];
                break;
            case VCO_3_PULSE_WIDTH:
                value = floatValues[7];
                break;
            case VCO_3_LEVEL:
                value = floatValues[8];
                break;
            case VCO_3_FINE_TUNE:
                value = floatValues[9];
                break;
            case VCO_NOISE_LEVEL:
                value = floatValues[10];
                break;
            case VCO_EXT_TRIGGER_GATE:
                value = floatValues[11];
                break;
            case VCO_EXT_LEVEL:
                value = floatValues[12];
                break;
            case VCF_CUTOFF:
                value = floatValues[13];
                break;
            case VCF_RESONANCE:
                value = floatValues[14];
                break;
            case VCF_TRACKING:
                value = floatValues[15];
                break;
            case VCF_ENV_1_DEPTH:
                value = floatValues[16];
                break;
            case VCF_ENV_2_DEPTH:
                value = floatValues[17];
                break;
            case VCA_LEVEL:
                value = floatValues[18];
                break;
            case VCA_ENV_1_DEPTH:
                value = floatValues[19];
                break;
            case ENV_1_ATTACK:
                value = floatValues[20];
                break;
            case ENV_1_DECAY:
                value = floatValues[21];
                break;
            case ENV_1_SUSTAIN:
                value = floatValues[22];
                break;
            case ENV_1_RELEASE:
                value = floatValues[23];
                break;
            case ENV_2_ATTACK:
                value = floatValues[24];
                break;
            case ENV_2_DECAY:
                value = floatValues[25];
                break;
            case ENV_2_SUSTAIN:
                value = floatValues[26];
                break;
            case ENV_2_RELEASE:
                value = floatValues[27];
                break;
            case LFO_1_PULSE_WIDTH:
                value = floatValues[28];
                break;
            case LFO_1_RATE:
                value = floatValues[29];
                break;
            case LFO_1_TRIGGER_PHASE:
                value = floatValues[30];
                break;
            case LFO_1_TO_MASTER_TUNE:
                value = floatValues[31];
                break;
            case LFO_2_PULSE_WIDTH:
                value = floatValues[32];
                break;
            case LFO_2_RATE:
                value = floatValues[33];
                break;
            case LFO_2_TRIGGER_PHASE:
                value = floatValues[34];
                break;
            case LFO_2_TO_VCF_CUTOFF:
                value = floatValues[35];
                break;
            case SH_LEVEL:
                value = floatValues[36];
                break;
            case SH_RATE:
                value = floatValues[37];
                break;
            case MOD_1_DEPTH:
                value = floatValues[38];
                break;
            case MOD_2_DEPTH:
                value = floatValues[39];
                break;
            case MOD_3_DEPTH:
                value = floatValues[40];
                break;
            case MOD_4_DEPTH:
                value = floatValues[41];
                break;
            case MOD_5_DEPTH:
                value = floatValues[42];
                break;
            case MOD_6_DEPTH:
                value = floatValues[43];
                break;
            case MOD_7_DEPTH:
                value = floatValues[44];
                break;
            case MOD_8_DEPTH:
                value = floatValues[45];
                break;
            case FX_1:
                value = floatValues[46];
                break;
            case FX_2:
                value = floatValues[47];
                break;
            case FX_3:
                value = floatValues[48];
                break;
            case FX_4:
                value = floatValues[49];
                break;
            case FX_5:
                value = floatValues[50];
                break;
            case FX_REVERB:
                value = floatValues[51];
                break;
            case GEN_BALANCE:
                value = floatValues[52];
                break;
            case GEN_REVERB_DECAY:
                value = floatValues[53];
                break;
            default:
                value = 0.0f;
                break;
        }

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
                setValue(PatchSetting::SH_LEVEL, multiPots->GetMpValue(0, controlNumber));
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
                break;
            case 0x21:
                setValue(PatchSetting::VCO_2_ON, ge->getPinValue(controlNumber, 5));
                break;
            case 0x22:
                break;
            case 0x23:
                setValue(PatchSetting::VCO_NOISE_ON, ge->getPinValue(controlNumber, 5));
                setValue(PatchSetting::VCO_3_ON, ge->getPinValue(controlNumber, 6));
                setValue(PatchSetting::VCO_EXT_ON, ge->getPinValue(controlNumber, 9));
                setValue(PatchSetting::LFO_1_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 10));
                setValue(PatchSetting::LFO_2_TRIGGER_RESET_ON, ge->getPinValue(controlNumber, 11));
                break;
        }
    }

} // namespace kiwi_synth
