#include "EnumToText.h"

namespace kiwi_synth
{

    void EnumToText::GetWaveform(char* buffer, Waveform waveform) {
        switch (waveform) {
            case WAVEFORM_SQUARE:
                strcpy(buffer, "Squa");
                break;
            case WAVEFORM_SAWTOOTH:
                strcpy(buffer, "Sawt");
                break;
            case WAVEFORM_TRIANGLE:
                strcpy(buffer, "Tria");
                break;
            case WAVEFORM_VARISHAPE:
                strcpy(buffer, "VaSh");
                break;
            case WAVEFORM_VARISAW:
                strcpy(buffer, "VaSw");
                break;
            case WAVEFORM_SINE:
                strcpy(buffer, "Sine");
                break;
            case WAVEFORM_WAVEFOLDED_SAWTOOTH:
                strcpy(buffer, "WfSw");
                break;
        }
    }

    void EnumToText::GetNoiseType(char* buffer, NoiseType noiseType) {
        switch (noiseType) {
            case NOISE_TYPE_WHITE:
                strcpy(buffer, "Wh");
                break;
            case NOISE_TYPE_DUST:
                strcpy(buffer, "Ds");
                break;
        }
    }

    void EnumToText::GetLfoWaveform(char* buffer, LfoWaveform lfoWaveform) {
        switch (lfoWaveform) {
            case LFO_WAVEFORM_TRIANGLE:
                strcpy(buffer, "Tria");
                break;
            case LFO_WAVEFORM_SQUARE:
                strcpy(buffer, "Squa");
                break;
            case LFO_WAVEFORM_SAWTOOTH:
                strcpy(buffer, "Sawt");
                break;
            case LFO_WAVEFORM_RAMP:
                strcpy(buffer, "Ramp");
                break;
            case LFO_WAVEFORM_VARISHAPE:
                strcpy(buffer, "VaSh");
                break;
            case LFO_WAVEFORM_VARISAW:
                strcpy(buffer, "VaSw");
                break;
            case LFO_WAVEFORM_SINE:
                strcpy(buffer, "Sine");
                break;
            case LFO_WAVEFORM_WAVEFOLDED_SAWTOOTH:
                strcpy(buffer, "WfSw");
                break;
        }
    }

    void EnumToText::GetFilterType(char* buffer, FilterType filterType) {
        switch (filterType) {
            case LADDER_LOWPASS:
                strcpy(buffer, "LddrLoPa");
                break;
            case SVF_LOWPASS:
                strcpy(buffer, "LowPass ");
                break;
            case SVF_HIGHPASS:
                strcpy(buffer, "HighPass");
                break;
            case SVF_BANDPASS:
                strcpy(buffer, "BandPass");
                break;
            case SVF_NOTCH:
                strcpy(buffer, "Notch   ");
                break;
            case SVF_PEAK:
                strcpy(buffer, "Peak    ");
                break;
            case ONE_POLE_LOWPASS:
                strcpy(buffer, "1PoleHiP");
                break;
            case ONE_POLE_HIGHPASS:
                strcpy(buffer, "1PoleLoP");
                break;
        }
    }

    void EnumToText::GetModSource(char* buffer, ModulationSource modSource) {
        switch (modSource) {
            case SRC_NONE:
                strcpy(buffer, "None ");
                break;
            case SRC_FIXED:
                strcpy(buffer, "Fixed");
                break;
            case SRC_LFO_1:
                strcpy(buffer, "LFO 1");
                break;
            case SRC_LFO_2:
                strcpy(buffer, "LFO 2");
                break;
            case SRC_ENV_1:
                strcpy(buffer, "Env 1");
                break;
            case SRC_ENV_2:
                strcpy(buffer, "Env 2");
                break;
            case SRC_SH:
                strcpy(buffer, "SmHld");
                break;
            case SRC_NOTE:
                strcpy(buffer, "Note ");
                break;
            case SRC_VELOCITY:
                strcpy(buffer, "Veloc");
                break;
            case SRC_AFTERTOUCH:
                strcpy(buffer, "AftTo");
                break;
            case SRC_MOD_WHEEL:
                strcpy(buffer, "ModWh");
                break;
            case SRC_PITCH_BEND:
                strcpy(buffer, "PBend");
                break;
            case SRC_EXPRESSION:
                strcpy(buffer, "Expre");
                break;
            case SRC_SUSTAIN:
                strcpy(buffer, "Susta");
                break;
            case SRC_VOICE_NO:
                strcpy(buffer, "Voice");
                break;
        }
    }

    void EnumToText::GetModDestination(char* buffer, ModulationDestination modDestination) {
        switch (modDestination) {
            case DST_NONE:
                strcpy(buffer, "None ");
                break;
            case DST_OSCS_FREQ:
                strcpy(buffer, "Frequ");
                break;
            case DST_OSCS_PULSE_WIDTH:
                strcpy(buffer, "PWdth");
                break;
            case DST_OSC_1_FREQ:
                strcpy(buffer, "O1Frq");
                break;
            case DST_OSC_1_PULSE_WIDTH:
                strcpy(buffer, "O1Pwd");
                break;
            case DST_OSC_2_FREQ:
                strcpy(buffer, "O2Frq");
                break;
            case DST_OSC_2_PULSE_WIDTH:
                strcpy(buffer, "O2Pwd");
                break;
            case DST_OSC_3_FREQ:
                strcpy(buffer, "O3Frq");
                break;
            case DST_OSC_3_PULSE_WIDTH:
                strcpy(buffer, "O3Pwd");
                break;
            case DST_NOISE_DENSITY:
                strcpy(buffer, "NsDns");
                break;
            case DST_NOISE_LEVEL:
                strcpy(buffer, "NsLvl");
                break;
            case DST_SH_RATE:
                strcpy(buffer, "SHRat");
                break;
            case DST_FLT_CUTOFF:
                strcpy(buffer, "FltCt");
                break;
            case DST_FLT_RESONANCE:
                strcpy(buffer, "FltRs");
                break;
            case DST_AMP_LEVEL:
                strcpy(buffer, "AmpLv");
                break;
            case DST_AMP_ENV_1_DEPTH:
                strcpy(buffer, "AmpE1");
                break;
            case DST_LFO_1_FREQ:
                strcpy(buffer, "L1Frq");
                break;
            case DST_LFO_1_PULSE_WIDTH:
                strcpy(buffer, "L1Pwd");
                break;
            case DST_LFO_1_TRIGGER_PHASE:
                strcpy(buffer, "L1Phs");
                break;
            case DST_LFO_2_FREQ:
                strcpy(buffer, "L2Frq");
                break;
            case DST_LFO_2_PULSE_WIDTH:
                strcpy(buffer, "L2Pwd");
                break;
            case DST_LFO_2_TRIGGER_PHASE:
                strcpy(buffer, "L2Phs");
                break;
            case DST_ENV_1_ATTACK:
                strcpy(buffer, "E1Atk");
                break;
            case DST_ENV_1_DECAY:
                strcpy(buffer, "E1Dec");
                break;
            case DST_ENV_1_SUSTAIN:
                strcpy(buffer, "E1Sus");
                break;
            case DST_ENV_1_RELEASE:
                strcpy(buffer, "E1Rel");
                break;
            case DST_ENV_2_ATTACK:
                strcpy(buffer, "E2Atk");
                break;
            case DST_ENV_2_DECAY:
                strcpy(buffer, "E2Dec");
                break;
            case DST_ENV_2_SUSTAIN:
                strcpy(buffer, "E2Sus");
                break;
            case DST_ENV_2_RELEASE:
                strcpy(buffer, "E2Rel");
                break;
            case DST_BALANCE:
                strcpy(buffer, "Balnc");
                break;
            case DST_LFO_1_TO_MASTER_TUNE:
                strcpy(buffer, "L1>MT");
                break;
            case DST_LFO_2_TO_FLT_CUTOFF:
                strcpy(buffer, "L2>FC");
                break;
            case DST_NOTE_TO_FLT_CUTOFF:
                strcpy(buffer, "Nt>FC");
                break;
            case DST_ENV_1_TO_AMP:
                strcpy(buffer, "E1>Lv");
                break;
            case DST_ENV_1_TO_FLT_CUTOFF:
                strcpy(buffer, "E1>FC");
                break;
            case DST_ENV_2_TO_FLT_CUTOFF:
                strcpy(buffer, "E2>FC");
                break;
            case DST_SH_TO_FLT_CUTOFF:
                strcpy(buffer, "SH>FC");
                break;
            case DST_SH_IN:
                strcpy(buffer, "S&HIn");
                break;
            case DST_OSC_1_LEVEL:
                strcpy(buffer, "O1Lvl");
                break;
            case DST_OSC_2_LEVEL:
                strcpy(buffer, "O2Lvl");
                break;
            case DST_OSC_3_LEVEL:
                strcpy(buffer, "O3Lvl");
                break;
        }
    }

    void EnumToText::GetPatchType(char* buffer, PatchType patchType)
    {
        switch (patchType) {
            case PATCH_ARP:
                strcpy(buffer, "Arpeggiated");
                break;
            case PATCH_BASS:
                strcpy(buffer, "Bass");
                break;
            case PATCH_BRASS:
                strcpy(buffer, "Brass");
                break;
            case PATCH_DRONE:
                strcpy(buffer, "Drone");
                break;
            case PATCH_EFFECT:
                strcpy(buffer, "Effect");
                break;
            case PATCH_INIT:
                strcpy(buffer, "Initialized");
                break;
            case PATCH_KEY:
                strcpy(buffer, "Key");
                break;
            case PATCH_LEAD:
                strcpy(buffer, "Lead");
                break;
            case PATCH_OTHER:
                strcpy(buffer, "Other");
                break;
            case PATCH_PAD:
                strcpy(buffer, "Pad");
                break;
            case PATCH_PERCUSSION:
                strcpy(buffer, "Percussion");
                break;
            case PATCH_PLUCK:
                strcpy(buffer, "Pluck");
                break;
            case PATCH_STRING:
                strcpy(buffer, "String");
                break;
            case PATCH_SYNTH:
                strcpy(buffer, "Synth");
                break;
        }
    }

    void EnumToText::GetVoiceMode(char* buffer, VoiceMode voiceMode) {
        switch (voiceMode) {
            case VOICE_MODE_POLY:
                strcpy(buffer, "Polyphonic");
                break;
            case VOICE_MODE_MONO:
                strcpy(buffer, "Monophonic");
                break;
            case VOICE_MODE_MULTI:
                strcpy(buffer, "Layered");
                break;
            case VOICE_MODE_SPLIT:
                strcpy(buffer, "Split");
                break;
            case VOICE_MODE_PARA:
                strcpy(buffer, "Paraphonic");
                break;
            case VOICE_MODE_HSYNC:
                strcpy(buffer, "Hard Sync");
                break;
            case VOICE_MODE_HSYNC_MONO:
                strcpy(buffer, "Hrd Sync Mono");
                break;
            case VOICE_MODE_PM_PARA:
                strcpy(buffer, "PM Parallel");
                break;
            case VOICE_MODE_PM_PARA_MONO:
                strcpy(buffer, "PM Para Mono");
                break;
            case VOICE_MODE_PM_SER:
                strcpy(buffer, "PM Serial");
                break;
            case VOICE_MODE_PM_SER_MONO:
                strcpy(buffer, "PM Ser Mono");
                break;
        }
    }

    void EnumToText::GetMidiNote(char* buffer, uint8_t midiNote){
        char notes[12][3] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
        int octave = midiNote / 12 - 1;
        char* note = notes[midiNote % 12];
        sprintf(buffer, "%s%d", note, octave);
    }

    void EnumToText::GetEffectsMode(char* buffer, EffectsMode effectsMode){
        switch (effectsMode) {
            case FX_DISTORTION_DELAY:
                strcpy(buffer, "Distortion-Delay");
                break;
            case FX_CHORUS_DELAY:
                strcpy(buffer, "Chorus-Delay");
                break;
            case FX_PHASER_DELAY:
                strcpy(buffer, "Phaser-Delay");
                break;
            case FX_FLANGER_DELAY:
                strcpy(buffer, "Flanger-Delay");
                break;
            case FX_DISTORTION_BITCRUSH:
                strcpy(buffer, "Distort-Bitcrush");
                break;
        }
    }

    void EnumToText::GetEffect1(char* buffer, EffectsMode effectsMode) {
        switch (effectsMode) {
            case FX_DISTORTION_DELAY:
                strcpy(buffer, "Distortion-----------");
                break;
            case FX_CHORUS_DELAY:
                strcpy(buffer, "Chorus---------------");
                break;
            case FX_PHASER_DELAY:
                strcpy(buffer, "Phaser---------------");
                break;
            case FX_FLANGER_DELAY:
                strcpy(buffer, "Flanger--------------");
                break;
            case FX_DISTORTION_BITCRUSH:
                strcpy(buffer, "Distortion-----------");
                break;
        }
    }

    void EnumToText::GetEffect2(char* buffer, EffectsMode effectsMode) {
        switch (effectsMode) {
            case FX_DISTORTION_DELAY:
                strcpy(buffer, "Delay----------------");
                break;
            case FX_CHORUS_DELAY:
                strcpy(buffer, "Delay----------------");
                break;
            case FX_PHASER_DELAY:
                strcpy(buffer, "Delay----------------");
                break;
            case FX_FLANGER_DELAY:
                strcpy(buffer, "Delay----------------");
                break;
            case FX_DISTORTION_BITCRUSH:
                strcpy(buffer, "Bit Crusher----------");
                break;
        }
    }

    void EnumToText::GetEffectSetting(char* buffer, EffectsMode effectsMode, int settingNum) {
        switch (effectsMode) {
            case FX_DISTORTION_DELAY:
                switch (settingNum) {
                    case 1:
                        strcpy(buffer, "Drive ");
                        break;
                    case 2:
                        strcpy(buffer, "Level ");
                        break;
                    case 3:
                        strcpy(buffer, "Level ");
                        break;
                    case 4:
                        strcpy(buffer, "Time  ");
                        break;
                    case 5:
                        strcpy(buffer, "Feedbk");
                        break;
                }
                break;
            case FX_CHORUS_DELAY:
                switch (settingNum) {
                    case 1:
                        strcpy(buffer, "Rate  ");
                        break;
                    case 2:
                        strcpy(buffer, "Depth ");
                        break;
                    case 3:
                        strcpy(buffer, "Level ");
                        break;
                    case 4:
                        strcpy(buffer, "Time  ");
                        break;
                    case 5:
                        strcpy(buffer, "Feedbk");
                        break;
                }
                break;
            case FX_PHASER_DELAY:
                switch (settingNum) {
                    case 1:
                        strcpy(buffer, "Rate  ");
                        break;
                    case 2:
                        strcpy(buffer, "Depth ");
                        break;
                    case 3:
                        strcpy(buffer, "Level ");
                        break;
                    case 4:
                        strcpy(buffer, "Time  ");
                        break;
                    case 5:
                        strcpy(buffer, "Feedbk");
                        break;
                }
                break;
            case FX_FLANGER_DELAY:
                switch (settingNum) {
                    case 1:
                        strcpy(buffer, "Rate  ");
                        break;
                    case 2:
                        strcpy(buffer, "Depth ");
                        break;
                    case 3:
                        strcpy(buffer, "Level ");
                        break;
                    case 4:
                        strcpy(buffer, "Time  ");
                        break;
                    case 5:
                        strcpy(buffer, "Feedbk");
                        break;
                }
                break;
            case FX_DISTORTION_BITCRUSH:
                switch (settingNum) {
                    case 1:
                        strcpy(buffer, "Gain  ");
                        break;
                    case 2:
                        strcpy(buffer, "Level ");
                        break;
                    case 3:
                        strcpy(buffer, "Crush ");
                        break;
                    case 4:
                        strcpy(buffer, "DwnSmp");
                        break;
                    case 5:
                        strcpy(buffer, "Decimt");
                        break;
                }
                break;
        }
    }

    void EnumToText::GetReverbMode(char* buffer, ReverbMode reverbMode) {
        switch (reverbMode) {
            case REVERB_ROOM:
                strcpy(buffer, "Room");
                break;
            case REVERB_HALL:
                strcpy(buffer, "Hall");
                break;
            case REVERB_CHAMBER:
                strcpy(buffer, "Chamber");
                break;
            case REVERB_CATHEDRAL:
                strcpy(buffer, "Cathedral");
                break;
            case REVERB_BLOOM:
                strcpy(buffer, "Bloom");
                break;
            case REVERB_BRIGHT:
                strcpy(buffer, "Bright");
                break;
        }
    }

    void EnumToText::GetReverbModePadded(char* buffer, ReverbMode reverbMode) {
        switch (reverbMode) {
            case REVERB_ROOM:
                strcpy(buffer, "Room Reverb----------");
                break;
            case REVERB_HALL:
                strcpy(buffer, "Hall Reverb----------");
                break;
            case REVERB_CHAMBER:
                strcpy(buffer, "Chamber Reverb-------");
                break;
            case REVERB_CATHEDRAL:
                strcpy(buffer, "Cathedral Reverb-----");
                break;
            case REVERB_BLOOM:
                strcpy(buffer, "Bloom Reverb---------");
                break;
            case REVERB_BRIGHT:
                strcpy(buffer, "Bright Reverb--------");
                break;
        }
    }

} // namespace kiwi_synth
