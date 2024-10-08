#include "EnumToText.h"

namespace kiwi_synth
{

    void EnumToText::GetWaveform(char* buffer, Waveform waveform) {
        switch (waveform) {
            case WAVEFORM_SQUARE:
                strcpy(buffer, "Sq");
                break;
            case WAVEFORM_SAWTOOTH:
                strcpy(buffer, "Sa");
                break;
            case WAVEFORM_TRIANGLE:
                strcpy(buffer, "Tr");
                break;
            case WAVEFORM_VARISHAPE:
                strcpy(buffer, "Sh");
                break;
            case WAVEFORM_VARISAW:
                strcpy(buffer, "VS");
                break;
            case WAVEFORM_SINE:
                strcpy(buffer, "Si");
                break;
            case WAVEFORM_WAVEFOLDED_SAWTOOTH:
                strcpy(buffer, "WS");
                break;
        }
    }

    void EnumToText::GetNoiseType(char* buffer, int8_t noiseType) {
        switch (noiseType) {
            case 0:
                strcpy(buffer, "Wh");
                break;
            case 1:
                strcpy(buffer, "Ds");
                break;
        }
    }

    void EnumToText::GetLfoWaveform(char* buffer, int8_t lfoWaveform) {
        switch (lfoWaveform) {
            case LFO_WAVEFORM_TRIANGLE:
                strcpy(buffer, "Tr");
                break;
            case LFO_WAVEFORM_SQUARE:
                strcpy(buffer, "Sq");
                break;
            case LFO_WAVEFORM_SAWTOOTH:
                strcpy(buffer, "Sa");
                break;
            case LFO_WAVEFORM_RAMP:
                strcpy(buffer, "Ra");
                break;
            case LFO_WAVEFORM_VARISHAPE:
                strcpy(buffer, "Sh");
                break;
            case LFO_WAVEFORM_VARISAW:
                strcpy(buffer, "VS");
                break;
            case LFO_WAVEFORM_SINE:
                strcpy(buffer, "Si");
                break;
            case LFO_WAVEFORM_WAVEFOLDED_SAWTOOTH:
                strcpy(buffer, "WS");
                break;
        }
    }

    void EnumToText::GetVcfType(char* buffer, int8_t vcfType) {
        switch (vcfType) {
            case 0:
                strcpy(buffer, "LddrLoPa");
                break;
            case 1:
                strcpy(buffer, "LowPass");
                break;
            case 2:
                strcpy(buffer, "HighPass");
                break;
            case 3:
                strcpy(buffer, "BandPass");
                break;
            case 4:
                strcpy(buffer, "Notch");
                break;
            case 5:
                strcpy(buffer, "Peak");
                break;
            case 6:
                strcpy(buffer, "1PoleHiP");
                break;
            case 7:
                strcpy(buffer, "1PoleLoP");
                break;
        }
    }

    void EnumToText::GetModSource(char* buffer, int8_t modSource) {
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

    void EnumToText::GetModDestination(char* buffer, int8_t modDestination) {
        switch (modDestination) {
            case DST_NONE:
                strcpy(buffer, "None ");
                break;
            case DST_VCOS_FREQ:
                strcpy(buffer, "Frequ");
                break;
            case DST_VCOS_PULSE_WIDTH:
                strcpy(buffer, "PWdth");
                break;
            case DST_VCO_1_FREQ:
                strcpy(buffer, "V1Frq");
                break;
            case DST_VCO_1_PULSE_WIDTH:
                strcpy(buffer, "V1Pwd");
                break;
            case DST_VCO_2_FREQ:
                strcpy(buffer, "V2Frq");
                break;
            case DST_VCO_2_PULSE_WIDTH:
                strcpy(buffer, "V2Pwd");
                break;
            case DST_VCO_3_FREQ:
                strcpy(buffer, "V3Frq");
                break;
            case DST_VCO_3_PULSE_WIDTH:
                strcpy(buffer, "V3Pwd");
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
            case DST_VCF_CUTOFF:
                strcpy(buffer, "VCFCt");
                break;
            case DST_VCF_RESONANCE:
                strcpy(buffer, "VCFRs");
                break;
            case DST_VCA_LEVEL:
                strcpy(buffer, "VCALv");
                break;
            case DST_VCA_ENV_1_DEPTH:
                strcpy(buffer, "VCAE1");
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
            case DST_LFO_2_TO_VCF_CUTOFF:
                strcpy(buffer, "L2>FC");
                break;
            case DST_NOTE_TO_VCF_CUTOFF:
                strcpy(buffer, "Nt>FC");
                break;
            case DST_ENV_1_TO_VCA:
                strcpy(buffer, "E1>Lv");
                break;
            case DST_ENV_1_TO_VCF_CUTOFF:
                strcpy(buffer, "E1>FC");
                break;
            case DST_ENV_2_TO_VCF_CUTOFF:
                strcpy(buffer, "E2>FC");
                break;
            case DST_SH_TO_VCF_CUTOFF:
                strcpy(buffer, "SH>FC");
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

} // namespace kiwi_synth
