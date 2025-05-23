#ifndef __KIWI_SYNTH_PATCH_TYPES_H__
#define __KIWI_SYNTH_PATCH_TYPES_H__



namespace kiwisynth
{



const static int MAX_PATCH_NAME_LENGTH = 15;
const static int NUM_MOD_SOURCES = 15; // Including NONE
const static int NUM_MOD_DESTINATIONS = 41; // Not including NONE
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
    VOICE_MODE_PARA,
    VOICE_MODE_HSYNC,
    VOICE_MODE_HSYNC_MONO,
    VOICE_MODE_PM_PARA,
    VOICE_MODE_PM_PARA_MONO,
    VOICE_MODE_PM_SER,
    VOICE_MODE_PM_SER_MONO
} VoiceMode;
static const int VOICE_MODE_MAX = 11;

typedef enum {
    PM_MODE_OFF,
    PM_MODE_PARALLEL,
    PM_MODE_SERIAL
} PmMode;
static const int PM_MODE_MAX = 3;

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
static const int WAVEFORM_MAX = 7;

typedef enum {
    NOISE_TYPE_WHITE,
    NOISE_TYPE_DUST
} NoiseType;
static const int NOISE_TYPE_MAX = 2;

typedef enum {
    LFO_WAVEFORM_TRIANGLE,
    LFO_WAVEFORM_SAWTOOTH,
    LFO_WAVEFORM_RAMP,
    LFO_WAVEFORM_SQUARE,
    LFO_WAVEFORM_SINE,
    LFO_WAVEFORM_VARISHAPE,
    LFO_WAVEFORM_VARISAW,
    LFO_WAVEFORM_WAVEFOLDED_SAWTOOTH
} LfoWaveform;
static const int LFO_WAVEFORM_MAX = 8;

typedef enum {
    LADDER_LOWPASS,
    SVF_LOWPASS,
    SVF_HIGHPASS,
    SVF_BANDPASS,
    SVF_NOTCH,
    SVF_PEAK,
    ONE_POLE_LOWPASS,
    ONE_POLE_HIGHPASS
} FilterType;
static const int FILTER_TYPE_MAX = 8;

/*
    * All controls for the Kiwi Synth.
    */
typedef enum {
    OSC_VOICES,                 // 0
    OSC_MASTER_TUNE,                    // This does not get saved to patch settings or stored for patches. It is always loaded directly from live pot value
    OSC_PORTAMENTO_ON,
    OSC_PORTAMENTO_SPEED,

    OSC_1_WAVEFORM,
    OSC_2_WAVEFORM,             // 5
    OSC_3_WAVEFORM,

    OSC_1_PULSE_WIDTH,
    OSC_2_PULSE_WIDTH,
    OSC_3_PULSE_WIDTH,

    OSC_1_LEVEL,                // 10
    OSC_2_LEVEL,
    OSC_3_LEVEL,

    OSC_1_ON,
    OSC_2_ON,
    OSC_3_ON,                   // 15
    
    OSC_2_OCTAVE,
    OSC_3_OCTAVE,
    OSC_2_INTERVAL,
    OSC_3_INTERVAL,
    OSC_2_FINE_TUNE,            // 20
    OSC_3_FINE_TUNE,


    OSC_NOISE_ON,
    OSC_NOISE_TYPE,
    OSC_NOISE_LEVEL,
    OSC_DUST_DENSITY,           // 25
    OSC_INPUT_ON,
    OSC_INPUT_THRESHOLD,
    OSC_INPUT_LEVEL,
    // Input gain is analog only

    FLT_FILTER_TYPE,
    FLT_CUTOFF,                 // 30
    FLT_RESONANCE,
    FLT_TRACKING,
    FLT_ENV_1_DEPTH,
    FLT_ENV_2_DEPTH,

    AMP_LEVEL,                  // 35
    AMP_ENV_1_DEPTH,

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
    LFO_2_TO_FLT_CUTOFF,

    SH_TO_FLT_CUTOFF,
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
    MODS_LFO_1_TO_OSCS,
    MODS_ENV_1_TO_AMP,
    MODS_NOTE_TO_FLT_FREQ,
    MODS_ENV_1_TO_FLT_FREQ,
    MODS_ENV_2_TO_FLT_FREQ,
    MODS_LFO_2_TO_FLT_FREQ,
    MODS_SH_TO_FLT_FREQ
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
    DST_OSCS_FREQ,          // 0
    DST_OSCS_PULSE_WIDTH,   // 1
    DST_OSC_1_LEVEL,        // 2
    DST_OSC_1_FREQ,         // 3
    DST_OSC_1_PULSE_WIDTH,  // 4

    DST_OSC_2_LEVEL,        // 5
    DST_OSC_2_FREQ,         // 6
    DST_OSC_2_PULSE_WIDTH,  // 7
    DST_OSC_3_LEVEL,        // 8
    DST_OSC_3_FREQ,         // 9

    DST_OSC_3_PULSE_WIDTH,  // 10
    DST_NOISE_DENSITY,      // 11
    DST_NOISE_LEVEL,        // 12
    DST_SH_IN,              // 13
    DST_SH_RATE,            // 14

    DST_SH_TO_FLT_CUTOFF,   // 15
    DST_FLT_CUTOFF,         // 16
    DST_FLT_RESONANCE,      // 17
    DST_NOTE_TO_FLT_CUTOFF, // 18
    DST_AMP_LEVEL,          // 19

    DST_AMP_ENV_1_DEPTH,    // 20
    DST_LFO_1_FREQ,         // 21
    DST_LFO_1_PULSE_WIDTH,  // 22
    DST_LFO_1_TRIGGER_PHASE,// 23
    DST_LFO_1_TO_MASTER_TUNE,// 24

    DST_LFO_2_FREQ,         // 25
    DST_LFO_2_PULSE_WIDTH,  // 26
    DST_LFO_2_TRIGGER_PHASE,// 27
    DST_LFO_2_TO_FLT_CUTOFF,// 28
    DST_ENV_1_ATTACK,       // 29

    DST_ENV_1_DECAY,        // 30
    DST_ENV_1_SUSTAIN,      // 31
    DST_ENV_1_RELEASE,      // 32
    DST_ENV_1_TO_AMP,       // 33
    DST_ENV_1_TO_FLT_CUTOFF,// 34

    DST_ENV_2_ATTACK,       // 35
    DST_ENV_2_DECAY,        // 36
    DST_ENV_2_SUSTAIN,      // 37
    DST_ENV_2_RELEASE,      // 38
    DST_ENV_2_TO_FLT_CUTOFF,// 39

    DST_BALANCE             // 40

    //DST_FX_1,               // Can't modulate effects because source values are different for each voice
    //DST_FX_2,               // Could possibly modulate for sources 7-12 only or have only voice 1 apply or only in solo mode?
    //DST_FX_3,
    //DST_FX_4,
    //DST_FX_5,
    //DST_FX_REVERB
} ModulationDestination; // 37 destinations not including None



} // namespace kiwisynth
#endif // __KIWI_SYNTH_PATCH_TYPES_H__
