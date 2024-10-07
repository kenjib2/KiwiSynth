#ifndef __KIWI_SYNTH_PATCH_TYPES_H__
#define __KIWI_SYNTH_PATCH_TYPES_H__

namespace kiwi_synth
{
    const static int MAX_PATCH_NAME_LENGTH = 15;
    const static int NUM_MOD_SOURCES = 15; // Including NONE
    const static int NUM_MOD_DESTINATIONS = 37; // Not including NONE
    const static int NUM_MODULATIONS = 15; // Including 8 variable and 7 fixed
    const static int NUM_PATCH_SETTINGS = 99;
    const static int NUM_PATCH_SETTINGS_SAVED = 92;
    const static float MIN_DISTORTION_GAIN = 5.0f;
    const static float MAX_DISTORTION_GAIN = 150.0f;

    typedef enum {
        PATCH_INIT = -1, // Initialized default patches
        PATCH_LEAD,
        PATCH_PAD,
        PATCH_SYNTH,
        PATCH_DRONE,
        PATCH_BASS,
        PATCH_PLUCK,
        PATCH_KEY,
        PATCH_BRASS,
        PATCH_STRING,
        PATCH_ARP,
        PATCH_PERCUSSION,
        PATCH_EFFECT,
        PATCH_OTHER
    } PatchType;
    static const int PATCH_TYPE_MAX = 13;

    typedef enum {
        VOICE_MODE_POLY,
        VOICE_MODE_MONO,
        VOICE_MODE_MULTI,
        VOICE_MODE_SPLIT,
        VOICE_MODE_PARA
    } VoiceMode;
    static const int VOICE_MODE_MAX = 5;

    typedef enum {
        FX_DISTORTION_DELAY,
        FX_CHORUS_DELAY,
        FX_PHASER_DELAY,
        FX_FLANGER_DELAY,
        FX_DISTORTION_BITCRUSH
    } EffectsMode;
    static const int FX_MODE_MAX = 5;

    typedef enum {
        REVERB_ROOM,
        REVERB_HALL,
        REVERB_CHAMBER,
        REVERB_CATHEDRAL,
        REVERB_BLOOM,
        REVERB_BRIGHT
    } ReverbMode;
    static const int REVERB_MODE_MAX = 6;

    typedef enum {
        WAVEFORM_SQUARE,
        WAVEFORM_SAWTOOTH,
        WAVEFORM_TRIANGLE,
        WAVEFORM_VARISHAPE,
        WAVEFORM_VARISAW,
        WAVEFORM_SINE,
        WAVEFORM_WAVEFOLDED_SAWTOOTH
    } Waveform;

    typedef enum {
        LFO_WAVEFORM_TRIANGLE,
        LFO_WAVEFORM_SAWTOOTH,
        LFO_WAVEFORM_RAMP,
        LFO_WAVEFORM_SQUARE
    } LfoWaveform;

    enum FilterType {
        LADDER_LOWPASS,
        SVF_LOWPASS,
        SVF_HIGHPASS,
        SVF_BANDPASS,
        SVF_NOTCH,
        SVF_PEAK,
        ONE_POLE_LOWPASS,
        ONE_POLE_HIGHPASS
    };
    
    /*
     * All controls for the Kiwi Synth.
     */
    typedef enum {
        VCO_VOICES,                 // 0
        VCO_MASTER_TUNE,                    // This does not get saved to patch settings or stored for patches. It is always loaded directly from live pot value
        VCO_PORTAMENTO_ON,
        VCO_PORTAMENTO_SPEED,

        VCO_1_WAVEFORM,
        VCO_2_WAVEFORM,             // 5
        VCO_3_WAVEFORM,

        VCO_1_PULSE_WIDTH,
        VCO_2_PULSE_WIDTH,
        VCO_3_PULSE_WIDTH,

        VCO_1_LEVEL,                // 10
        VCO_2_LEVEL,
        VCO_3_LEVEL,

        VCO_1_ON,
        VCO_2_ON,
        VCO_3_ON,                   // 15
        
        VCO_2_OCTAVE,
        VCO_3_OCTAVE,
        VCO_2_INTERVAL,
        VCO_3_INTERVAL,
        VCO_2_FINE_TUNE,            // 20
        VCO_3_FINE_TUNE,


        VCO_NOISE_ON,
        VCO_NOISE_TYPE,
        VCO_NOISE_LEVEL,
        VCO_DUST_DENSITY,           // 25
        VCO_EXT_ON,
        // External gain is analog only
        VCO_EXT_TRIGGER_GATE,
        VCO_EXT_LEVEL,

        VCF_FILTER_TYPE,
        VCF_CUTOFF,                 // 30
        VCF_RESONANCE,
        VCF_TRACKING,
        VCF_ENV_1_DEPTH,
        VCF_ENV_2_DEPTH,

        VCA_LEVEL,                  // 35
        VCA_ENV_1_DEPTH,

        ENV_1_ATTACK,
        ENV_2_ATTACK,
        ENV_1_DECAY,
        ENV_2_DECAY,                // 40
        ENV_1_SUSTAIN,
        ENV_2_SUSTAIN,
        ENV_1_RELEASE,
        ENV_2_RELEASE,
        ENV_1_REVERSE_PHASE_ON,     // 45
        ENV_2_REVERSE_PHASE_ON,

        LFO_1_WAVEFORM,
        LFO_2_WAVEFORM,
        LFO_1_PULSE_WIDTH,
        LFO_2_PULSE_WIDTH,          // 50
        LFO_1_RATE,
        LFO_2_RATE,
        LFO_1_TRIGGER_RESET_ON,
        LFO_2_TRIGGER_RESET_ON,
        LFO_1_TRIGGER_PHASE,        // 55
        LFO_2_TRIGGER_PHASE,
        LFO_1_TO_MASTER_TUNE,
        LFO_2_TO_VCF_CUTOFF,

        SH_TO_VCF_CUTOFF,
        SH_RATE,                    // 60

        MOD_1_SOURCE,
        MOD_2_SOURCE,
        MOD_3_SOURCE,
        MOD_4_SOURCE,
        MOD_5_SOURCE,               // 65
        MOD_6_SOURCE,
        MOD_7_SOURCE,
        MOD_8_SOURCE,

        MOD_1_DESTINATION,
        MOD_2_DESTINATION,          // 70
        MOD_3_DESTINATION,
        MOD_4_DESTINATION,
        MOD_5_DESTINATION,
        MOD_6_DESTINATION,
        MOD_7_DESTINATION,          // 75
        MOD_8_DESTINATION,

        MOD_1_DEPTH,
        MOD_2_DEPTH,
        MOD_3_DEPTH,
        MOD_4_DEPTH,                // 80
        MOD_5_DEPTH,
        MOD_6_DEPTH,
        MOD_7_DEPTH,
        MOD_8_DEPTH,

        FX_1,                       // 85
        FX_2,
        FX_3,
        FX_4,
        FX_5,
        FX_REVERB,                  // 90

        GEN_BALANCE,
        GEN_SELECT,
        GEN_SELECT_BUTTON,
        GEN_AFTERTOUCH,
        GEN_MOD_WHEEL,              // 95
        GEN_PITCH_BEND,
        GEN_EXPRESSION,
        GEN_SUSTAIN
        
        // Headphones is analog only
        // Output Level is analog only
        // Power switch is analog only
    } PatchSetting;

    typedef enum {
        MODS_MOD_MATRIX_1,
        MODS_MOD_MATRIX_2,
        MODS_MOD_MATRIX_3,
        MODS_MOD_MATRIX_4,
        MODS_MOD_MATRIX_5,
        MODS_MOD_MATRIX_6,
        MODS_MOD_MATRIX_7,
        MODS_MOD_MATRIX_8,
        MODS_LFO_1_TO_VCOS,
        MODS_ENV_1_TO_VCA,
        MODS_NOTE_TO_VCF_FREQ,
        MODS_ENV_1_TO_VCF_FREQ,
        MODS_ENV_2_TO_VCF_FREQ,
        MODS_LFO_2_TO_VCF_FREQ,
        MODS_SH_TO_VCF_FREQ
    } Modulations;

    typedef enum
    {
        SRC_NONE, // = 0,
        SRC_FIXED,  // 1
        SRC_LFO_1,  // 2
        SRC_LFO_2,  // 3
        SRC_ENV_1,  // 4
        SRC_ENV_2,  // 5
        SRC_SH,     // 6
        SRC_NOTE,   // 7
        SRC_VELOCITY,   // 8
        SRC_AFTERTOUCH, // 9
        SRC_MOD_WHEEL,  // 10
        SRC_PITCH_BEND, // 11
        SRC_EXPRESSION, // 12
        SRC_SUSTAIN,    // 13
        SRC_VOICE_NO    // 14
    } ModulationSource; // 15 sources

    typedef enum
    {
        DST_NONE = -1,
        DST_VCOS_FREQ,          // 0
        DST_VCOS_PULSE_WIDTH,   // 1
        DST_VCO_1_FREQ,         // 2
        DST_VCO_1_PULSE_WIDTH,  // 3
        DST_VCO_2_FREQ,         // 4

        DST_VCO_2_PULSE_WIDTH,  // 5
        DST_VCO_3_FREQ,         // 6
        DST_VCO_3_PULSE_WIDTH,  // 7
        DST_NOISE_DENSITY,      // 8
        DST_NOISE_LEVEL,        // 9

        DST_SH_RATE,            // 10
        DST_VCF_CUTOFF,         // 11
        DST_VCF_RESONANCE,      // 12
        DST_VCA_LEVEL,          // 13
        DST_VCA_ENV_1_DEPTH,    // 14

        DST_LFO_1_FREQ,         // 15
        DST_LFO_1_PULSE_WIDTH,  // 16
        DST_LFO_1_TRIGGER_PHASE,// 17
        DST_LFO_2_FREQ,         // 18
        DST_LFO_2_PULSE_WIDTH,  // 19

        DST_LFO_2_TRIGGER_PHASE,// 20
        DST_ENV_1_ATTACK,       // 21
        DST_ENV_1_DECAY,        // 22
        DST_ENV_1_SUSTAIN,      // 23
        DST_ENV_1_RELEASE,      // 24

        DST_ENV_2_ATTACK,       // 25
        DST_ENV_2_DECAY,        // 26
        DST_ENV_2_SUSTAIN,      // 27
        DST_ENV_2_RELEASE,      // 28
        DST_BALANCE,            // 29

        DST_LFO_1_TO_MASTER_TUNE,// 30
        DST_LFO_2_TO_VCF_CUTOFF,// 31
        DST_NOTE_TO_VCF_CUTOFF, // 32
        DST_ENV_1_TO_VCA,       // 33
        DST_ENV_1_TO_VCF_CUTOFF,// 34

        DST_ENV_2_TO_VCF_CUTOFF,// 35
        DST_SH_TO_VCF_CUTOFF    // 36

        //DST_FX_1,               // Can't modulate effects because source values are different for each voice
        //DST_FX_2,               // Could possibly modulate for sources 7-12 only or have only voice 1 apply or only in solo mode?
        //DST_FX_3,
        //DST_FX_4,
        //DST_FX_5,
        //DST_FX_REVERB
    } ModulationDestination; // 37 destinations not including None

} // namespace kiwi_synth
#endif // __KIWI_SYNTH_PATCH_TYPES_H__
