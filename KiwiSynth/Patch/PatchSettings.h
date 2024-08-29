#ifndef __KIWI_SYNTH_PATCH_SETTINGS_H__
#define __KIWI_SYNTH_PATCH_SETTINGS_H__


#include <string>
#include "daisy_seed.h"
#include "../Controls/MultiPots.h"
#include "../Controls/GpioExpansion.h"
#include "../Controls/ControlListener.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{
    const int MAX_PATCH_NAME_LENGTH = 31;
    const static int NUM_MOD_SOURCES = 13; // Not including NONE
    const static int NUM_MOD_DESTINATIONS = 18; // Not including NONE
    const static int NUM_MODULATIONS = 15; // Including 8 variable and 7 fixed

    typedef enum {
        MULTIPOTS,
        GPIO_EXPANSION
    } ControlId;

    /*
     * All controls for the Kiwi Synth.
     */
    typedef enum {
        VCO_VOICES,
        VCO_MASTER_TUNE,
        VCO_PORTAMENTO_ON,
        VCO_PORTAMENTO_SPEED,

        VCO_1_WAVEFORM,
        VCO_1_PULSE_WIDTH,
        VCO_1_LEVEL,

        VCO_2_ON,
        VCO_2_WAVEFORM,
        VCO_2_PULSE_WIDTH,
        VCO_2_LEVEL,
        VCO_2_OCTAVE,
        VCO_2_INTERVAL,
        VCO_2_FINE_TUNE,

        VCO_3_ON,
        VCO_3_WAVEFORM,
        VCO_3_PULSE_WIDTH,
        VCO_3_LEVEL,
        VCO_3_OCTAVE,
        VCO_3_INTERVAL,
        VCO_3_FINE_TUNE,

        VCO_NOISE_ON,
        VCO_NOISE_TYPE,
        VCO_NOISE_LEVEL,
        VCO_EXT_ON,
        // External gain is analog only
        VCO_EXT_TRIGGER_GATE,
        VCO_EXT_LEVEL,

        VCF_FILTER_TYPE,
        VCF_CUTOFF,
        VCF_RESONANCE,
        VCF_TRACKING,
        VCF_ENV_1_DEPTH,
        VCF_ENV_2_DEPTH,

        VCA_LEVEL,
        VCA_ENV_1_DEPTH,

        ENV_1_ATTACK,
        ENV_1_DECAY,
        ENV_1_SUSTAIN,
        ENV_1_RELEASE,
        ENV_1_REVERSE_PHASE_ON,

        ENV_2_ATTACK,
        ENV_2_DECAY,
        ENV_2_SUSTAIN,
        ENV_2_RELEASE,
        ENV_2_REVERSE_PHASE_ON,

        LFO_1_WAVEFORM,
        LFO_1_PULSE_WIDTH,
        LFO_1_RATE,
        LFO_1_TRIGGER_RESET_ON,
        LFO_1_TRIGGER_PHASE,
        LFO_1_TO_MASTER_TUNE,

        LFO_2_WAVEFORM,
        LFO_2_PULSE_WIDTH,
        LFO_2_RATE,
        LFO_2_TRIGGER_RESET_ON,
        LFO_2_TRIGGER_PHASE,
        LFO_2_TO_VCF_CUTOFF,

        SH_TO_VCF_CUTOFF,
        SH_RATE,

        MOD_1_SOURCE,
        MOD_1_DESTINATION,
        MOD_1_DEPTH,
        
        MOD_2_SOURCE,
        MOD_2_DESTINATION,
        MOD_2_DEPTH,
        
        MOD_3_SOURCE,
        MOD_3_DESTINATION,
        MOD_3_DEPTH,
        
        MOD_4_SOURCE,
        MOD_4_DESTINATION,
        MOD_4_DEPTH,
        
        MOD_5_SOURCE,
        MOD_5_DESTINATION,
        MOD_5_DEPTH,
        
        MOD_6_SOURCE,
        MOD_6_DESTINATION,
        MOD_6_DEPTH,
        
        MOD_7_SOURCE,
        MOD_7_DESTINATION,
        MOD_7_DEPTH,
        
        MOD_8_SOURCE,
        MOD_8_DESTINATION,
        MOD_8_DEPTH,

        FX_1,
        FX_2,
        FX_3,
        FX_4,
        FX_5,
        FX_REVERB,

        GEN_BALANCE,
        GEN_SELECT,
        GEN_SELECT_BUTTON,
        GEN_FX_SELECT,
        GEN_REVERB_SELECT,
        GEN_REVERB_DECAY,
        GEN_NAME,
        // Headphones is analog only
        // Output Level is analog only
        // Power switch is analog only
    } PatchSetting;

    typedef enum
    {
        SRC_NONE = -1,
        SRC_FIXED,  // 0
        SRC_LFO_1,  // 1
        SRC_LFO_2,  // 2
        SRC_ENV_1,  // 3
        SRC_ENV_2,  // 4
        SRC_SH,     // 5
        SRC_NOTE,   // 6
        SRC_VELOCITY,   // 7
        SRC_AFTERTOUCH, // 8    *
        SRC_MOD_WHEEL,  // 9    *
        SRC_PITCH_BEND, // 10   *
        SRC_EXPRESSION, // 11   *
        SRC_PEDAL       // 12   *
    } ModulationSource; // 13 sources

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
        DST_VCF_CUTOFF,         // 8
        DST_VCF_RESONANCE,      // 9
        //DST_VCF_ENV_1_DEPTH,          *   // Modulating other modulations might be complicated
        //DST_VCF_ENV_2_DEPTH,          *
        DST_VCA_LEVEL,          // 10
        DST_VCA_ENV_1_DEPTH,    // 11
        //DST_VCA_ENV_2_DEPTH,          *
        DST_LFO_1_FREQ,         // 12
        DST_LFO_1_PULSE_WIDTH,  // 13
        //DST_LFO_1_TO_MASTER_TUNE,     *
        DST_LFO_1_TRIGGER_PHASE,// 14
        DST_LFO_2_FREQ,         // 15
        DST_LFO_2_PULSE_WIDTH,  // 16
        //DST_LFO_2_TO_VCF_CUTOFF,      *
        DST_LFO_2_TRIGGER_PHASE,// 17
        //DST_ENV_1_ATTACK,       //    *   // Envelope might be too expensive to modulate
        //DST_ENV_1_DECAY,        //    *
        //DST_ENV_1_SUSTAIN,      //    *
        //DST_ENV_1_RELEASE,      //    *
        //DST_ENV_2_ATTACK,       //    *
        //DST_ENV_2_DECAY,        //    *
        //DST_ENV_2_SUSTAIN,      //    *
        //DST_ENV_2_RELEASE,      //    *
        //DST_FX_1,               //    *   // Can't modulate effects because source values are different for each voice
        //DST_FX_2,               //    *   // Could possibly modulate for sources 7-12 only
        //DST_FX_3,               //    *
        //DST_FX_4,               //    *
        //DST_FX_5,               //    *
        //DST_FX_REVERB           //    *
    } ModulationDestination; // 18 destinations

    typedef enum
    {
        LINEAR,
        EXPONENTIAL,
        LOGARHITHMIC,
        OCTAVE
    } Scale;

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
            static constexpr float minValue = 0.0f;
            static constexpr float maxValue = 1.0f;
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

            /*
             * Converts a +/- number of octaves into a frequency multiplier
             */
            static float octaveToFrequencyMultiplier(int octave);
            /*
             * From ControlListener: Callback to update controls.
             */
            void controlUpdate(int controlNumber, int controlId);
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
            void setValue(PatchSetting setting, std::string value);

            /*
             * Gets an integer setting value. If a non-integer setting is attempted, 0 will be returned.
             */
            int8_t getIntValue(PatchSetting setting);
            /*
             * Gets a float setting value. If a non-float setting is attempted, 0.0f will be returned.
             */
            float getFloatValue(PatchSetting setting, float min = 0.0f, float max = 1.0f, Scale scale = LINEAR);
            /*
             * Gets a bool setting value. If a non-float setting is attempted, false will be returned.
             */
            bool getBoolValue(PatchSetting setting);
            /*
             * Gets a string setting value. If a non-string setting is attempted, "" will be returned.
             */
            std::string getStringValue(PatchSetting setting);
    }; // class PatchSettings
} // namespace kiwi_synth


#endif // __PATCH_SETTINGS_H__
