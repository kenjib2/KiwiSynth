#ifndef __KIWI_SYNTH_PATCH_SETTINGS_H__
#define __KIWI_SYNTH_PATCH_SETTINGS_H__


#include <string>

#include "daisy_seed.h"

#include "../../KUtils.h"
#include "../Controls/MultiPots.h"
#include "../Controls/GpioExpansion.h"
#include "../Controls/ControlListener.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    const static int MAX_PATCH_NAME_LENGTH = 31;
    const static int NUM_MOD_SOURCES = 15; // Including NONE
    const static int NUM_MOD_DESTINATIONS = 29; // Not including NONE
    const static int NUM_MODULATIONS = 15; // Including 8 variable and 7 fixed
    const static int NUM_PATCH_SETTINGS = 102;
    const static float MIN_DISTORTION_GAIN = 5.0f;
    const static float MAX_DISTORTION_GAIN = 150.0f;

    /*
     * All controls for the Kiwi Synth.
     */
    typedef enum {
        VCO_VOICES,                 // 0
        VCO_MASTER_TUNE,
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
        VCO_EXT_ON,                 // 25
        // External gain is analog only
        VCO_EXT_TRIGGER_GATE,
        VCO_EXT_LEVEL,

        VCF_FILTER_TYPE,
        VCF_CUTOFF,
        VCF_RESONANCE,              // 30
        VCF_TRACKING,
        VCF_ENV_1_DEPTH,
        VCF_ENV_2_DEPTH,

        VCA_LEVEL,
        VCA_ENV_1_DEPTH,            // 35

        ENV_1_ATTACK,
        ENV_2_ATTACK,
        ENV_1_DECAY,
        ENV_2_DECAY,
        ENV_1_SUSTAIN,              // 40
        ENV_2_SUSTAIN,
        ENV_1_RELEASE,
        ENV_2_RELEASE,
        ENV_1_REVERSE_PHASE_ON,
        ENV_2_REVERSE_PHASE_ON,     // 45

        LFO_1_WAVEFORM,
        LFO_2_WAVEFORM,
        LFO_1_PULSE_WIDTH,
        LFO_2_PULSE_WIDTH,
        LFO_1_RATE,                 // 50
        LFO_2_RATE,
        LFO_1_TRIGGER_RESET_ON,
        LFO_2_TRIGGER_RESET_ON,
        LFO_1_TRIGGER_PHASE,
        LFO_2_TRIGGER_PHASE,        // 55
        LFO_1_TO_MASTER_TUNE,
        LFO_2_TO_VCF_CUTOFF,

        SH_TO_VCF_CUTOFF,
        SH_RATE,

        MOD_1_SOURCE,               // 60
        MOD_2_SOURCE,
        MOD_3_SOURCE,
        MOD_4_SOURCE,
        MOD_5_SOURCE,
        MOD_6_SOURCE,               // 65
        MOD_7_SOURCE,
        MOD_8_SOURCE,

        MOD_1_DESTINATION,
        MOD_2_DESTINATION,
        MOD_3_DESTINATION,          // 70
        MOD_4_DESTINATION,
        MOD_5_DESTINATION,
        MOD_6_DESTINATION,
        MOD_7_DESTINATION,
        MOD_8_DESTINATION,          // 75

        MOD_1_DEPTH,
        MOD_2_DEPTH,
        MOD_3_DEPTH,
        MOD_4_DEPTH,
        MOD_5_DEPTH,                // 80
        MOD_6_DEPTH,
        MOD_7_DEPTH,
        MOD_8_DEPTH,

        FX_1,
        FX_2,                       // 85
        FX_3,
        FX_4,
        FX_5,
        FX_REVERB,

        GEN_BALANCE,                // 90
        GEN_SELECT,
        GEN_SELECT_BUTTON,
        GEN_FX_SELECT,
        GEN_REVERB_SELECT,
        GEN_REVERB_DECAY,           // 95
        GEN_NAME,
        GEN_AFTERTOUCH,
        GEN_MOD_WHEEL,
        GEN_PITCH_BEND,
        GEN_EXPRESSION,             // 100
        GEN_SUSTAIN
        
        // Headphones is analog only
        // Output Level is analog only
        // Power switch is analog only
    } PatchSetting;

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
        DST_NOISE_LEVEL,        // 8
        DST_SH_RATE,            // 9
        DST_VCF_CUTOFF,         // 10
        DST_VCF_RESONANCE,      // 11
        //DST_VCF_ENV_1_DEPTH,          // Modulating other modulations might be complicated
        //DST_VCF_ENV_2_DEPTH,
        DST_VCA_LEVEL,          // 12
        DST_VCA_ENV_1_DEPTH,    // 13
        //DST_VCA_ENV_2_DEPTH,
        DST_LFO_1_FREQ,         // 14
        DST_LFO_1_PULSE_WIDTH,  // 15
        //DST_LFO_1_TO_MASTER_TUNE,
        DST_LFO_1_TRIGGER_PHASE,// 16
        DST_LFO_2_FREQ,         // 17
        DST_LFO_2_PULSE_WIDTH,  // 18
        //DST_LFO_2_TO_VCF_CUTOFF,
        DST_LFO_2_TRIGGER_PHASE,// 19
        DST_ENV_1_ATTACK,       // 20
        DST_ENV_1_DECAY,        // 21
        DST_ENV_1_SUSTAIN,      // 22
        DST_ENV_1_RELEASE,      // 23
        DST_ENV_2_ATTACK,       // 24
        DST_ENV_2_DECAY,        // 25
        DST_ENV_2_SUSTAIN,      // 26
        DST_ENV_2_RELEASE,      // 27
        DST_BALANCE             // 28
        //DST_FX_1,               // Can't modulate effects because source values are different for each voice
        //DST_FX_2,               // Could possibly modulate for sources 7-12 only or have only voice 1 apply or only in solo mode?
        //DST_FX_3,
        //DST_FX_4,
        //DST_FX_5,
        //DST_FX_REVERB
    } ModulationDestination; // 28 destinations not including None

    /*
     * Manages update and access to all of the settings for the Kiwi Synth. PatchSettings maintains its own value buffers independent
     * of MultiPots. This is done to support saving and loading of patches. When a patch is loaded, it's state will be different from that
     * of the controls. When in this state, the PatchSetting values will not start being updated with MultiPot values until the control cross
     * the saved setting state. After that occurs, PatchSetting values will be constantly updated using the control values.
     * 
     * When the synth first starts up, before a patch is loaded, all values will be constantly updated from the controls. The synth can also
     * be initialized to use all live control settings at any time via the menu.
     */
    class PatchSettings : public ControlListener
    {
        private:
            static int8_t* maxIntValues; // Set the max values for int variables
            static float* lMinLookup; // Set the range for logarhithmic float variables
            static float* lMaxLookup; // Set the range for logarhithmic float variables

            // We have a lot of unused array members in order to prevent branching code when setting and retrieving values of different data types.
            char name[MAX_PATCH_NAME_LENGTH + 1];
            float floatValues[NUM_PATCH_SETTINGS]; // was float floatValues[59];
            int8_t intValues[NUM_PATCH_SETTINGS]; // was int8_t intValues[31];
            bool boolValues[NUM_PATCH_SETTINGS]; // was bool boolValues[10];
            bool lastPinValues[4][16]; // Used to track changes to the rotary encoder state

            MultiPots* multiPots;
            GpioExpansion* ge;

            /*
             * Updates a value based on encoder settings.
             */
            void processEncoder(PatchSetting setting, int controlNumber, int pinA, int pinB);
            /*
             * Updates all potentiometer based settings with the current values in the MultiPots buffer.
             */
            void updatePotValues(int controlNumber);
            /*
             * Updates expansion basaed values with the current values in the GpioExpansion buffer.
             */
            void updateGpioExpansionValues(int controlNumber);

        public:
            PatchSettings() {}
            ~PatchSettings() {}
            void Init(MultiPots* multiPots, GpioExpansion* ge);
            void DefaultSettings();
            void Copy(PatchSettings* patchSettings);

            /*
             * From ControlListener: Callback to update MultiPot controls.
             */
            void controlMpUpdate(int controlNumber);
            /*
             * From ControlListener: Callback to update GPIO Expander controls.
             */
            void controlGpioUpdate(int controlNumber);
            /*
             * Sets an integer setting value. If a non-integer setting is attempted, no action will be performed.
             */
            void setValue(PatchSetting setting, int8_t value);
            /*
             * Sets a float setting value. If a non-float setting is attempted, no action will be performed.
             */
            void setValue(PatchSetting setting, float value);
            /*
             * Sets a bool setting value. If a non-bool setting is attempted, no action will be performed.
             */
            void setValue(PatchSetting setting, bool value);
            /*
             * Sets a string/char* setting value. If a non-string setting is attempted, no action will be performed.
             */
            void setValue(PatchSetting setting, char* value);

            /*
             * Gets an integer setting value. If a non-integer setting is attempted, 0 will be returned.
             */
            int8_t getIntValue(PatchSetting setting);
            /*
             * Gets a float setting value. If a non-float setting is attempted, 0.0f will be returned.
             */
            float getFloatValue(PatchSetting setting);
            /*
             * Gets a float setting value using exponential scale. If a non-float setting is attempted, 0.0f will be returned.
             */
            float getFloatValueExponential(PatchSetting setting);
            /*
             * Gets a float setting using logarhithmic scale and lookup tables for min and max. If a non-float setting is attempted, 0.0f will be returned.
             */
            float getFloatValueLogLookup(PatchSetting setting);
            /*
             * Gets a float setting using exponential scale and min and max. If a non-float setting is attempted, 0.0f will be returned.
             */
            float getFloatValueExponential(PatchSetting setting, float min, float max);
            /*
             * Gets a float setting value with linear scale and min and max. If a non-float setting is attempted, 0.0f will be returned.
             */
            float getFloatValueLinear(PatchSetting setting, float min, float max);
            /*
             * Gets a bool setting value. If a non-float setting is attempted, false will be returned.
             */
            bool getBoolValue(PatchSetting setting);
            /*
             * Gets a string setting value. If a non-string setting is attempted, "" will be returned.
             */
            char* getStringValue(PatchSetting setting);
    }; // class PatchSettings
} // namespace kiwi_synth


#endif // __PATCH_SETTINGS_H__
