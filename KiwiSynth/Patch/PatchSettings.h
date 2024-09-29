#ifndef __KIWI_SYNTH_PATCH_SETTINGS_H__
#define __KIWI_SYNTH_PATCH_SETTINGS_H__


#include <string>

#include "daisy_seed.h"

#include "../Controls/MultiPots.h"
#include "../Controls/GpioExpansion.h"
#include "../Controls/IControlListener.h"
#include "PatchTypes.h"
#include "SavedPatch.h"

using namespace daisy;
using namespace daisy::seed;

namespace kiwi_synth
{

    /*
     * Manages update and access to all of the settings for the Kiwi Synth. PatchSettings maintains its own value buffers independent
     * of MultiPots. This is done to support saving and loading of patches. When a patch is loaded, it's state will be different from that
     * of the controls. When in this state, the PatchSetting values will not start being updated with MultiPot values until the control cross
     * the saved setting state. After that occurs, PatchSetting values will be constantly updated using the control values.
     * 
     * When the synth first starts up, before a patch is loaded, all values will be constantly updated from the controls. The synth can also
     * be initialized to use all live control settings at any time via the menu.
     */
    class PatchSettings : public IControlListener
    {
        private:
            static int8_t* maxIntValues; // Set the max values for int variables
            static float* lMinLookup; // Set the range for logarhithmic float variables
            static float* lMaxLookup; // Set the range for logarhithmic float variables

            // We have a lot of unused array members in order to prevent branching code when setting and retrieving values of different data types.
            float floatValues[NUM_PATCH_SETTINGS];
            int8_t intValues[NUM_PATCH_SETTINGS];
            bool boolValues[NUM_PATCH_SETTINGS];
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
            inline void Copy(PatchSettings* patchSettings) { Copy((bool*)patchSettings->boolValues, (int8_t*)patchSettings->intValues, (float*)patchSettings->floatValues); }
            void Copy(bool* boolValues, int8_t* intValues, float* floatValues);
            void Load(SavedPatch* savedPatch, int voiceNumber);

            /*
             * From ControlListener: Callback to update MultiPot controls.
             */
            inline void controlMpUpdate(int controlNumber) { updatePotValues(controlNumber); }
            /*
             * From ControlListener: Callback to update GPIO Expander controls.
             */
            inline void controlGpioUpdate(int controlNumber) { updateGpioExpansionValues(controlNumber); }
            
            /*
             * Sets an integer setting value. If a non-integer setting is attempted, no action will be performed.
             */
            inline void setValue(PatchSetting setting, int8_t value) { intValues[setting] = std::min(value, maxIntValues[setting]); }
            /*
             * Sets a float setting value. If a non-float setting is attempted, no action will be performed.
             */
            inline void setValue(PatchSetting setting, float value) { floatValues[setting] = value; }
            /*
             * Sets a bool setting value. If a non-bool setting is attempted, no action will be performed.
             */
            inline void setValue(PatchSetting setting, bool value) { boolValues[setting] = value; }

            /*
             * Gets an integer setting value. If a non-integer setting is attempted, 0 will be returned.
             */
            inline int8_t getIntValue(PatchSetting setting) { return intValues[setting]; }
            /* In case we need OCTAVE scaling again later.
            float range = max - min;
            // For plus or minus one octave we want 2^(2x-1)
            // More generally it's 2^(rangex - range/2)
            // This only works if max + min = 0
            return pow(2, range * value - range / 2);*/
            /*
             * Gets a float setting value. If a non-float setting is attempted, 0.0f will be returned.
             */
            inline float getFloatValue(PatchSetting setting) { return floatValues[setting]; }
            /*
             * Gets a float setting value using exponential scale. If a non-float setting is attempted, 0.0f will be returned.
             */
            inline float getFloatValueExponential(PatchSetting setting) { float value = getFloatValue(setting); return value * value; }
            /*
             * Gets a float setting using logarhithmic scale and lookup tables for min and max. If a non-float setting is attempted, 0.0f will be returned.
             */
            inline float getFloatValueLogLookup(PatchSetting setting)
            {
                float value;
                value = getFloatValue(setting);
                if (setting == MOD_1_DEPTH) { // Using MOD_1_DEPTH as a hacky fill in for alternate FX_1 value
                    value = getFloatValue(FX_1);
                }
                float lmin = lMinLookup[setting];
                float lmax = lMaxLookup[setting];
                return expf((value * (lmax - lmin)) + lmin);
            }
            /*
             * Gets a float setting using exponential scale and min and max. If a non-float setting is attempted, 0.0f will be returned.
             */
            inline float getFloatValueExponential(PatchSetting setting, float min, float max)
            {
                float value = getFloatValue(setting);
                float range = max - min;
                return value * value * range + min;
            }
            /*
             * Gets a float setting value with linear scale and min and max. If a non-float setting is attempted, 0.0f will be returned.
             */
            inline float getFloatValueLinear(PatchSetting setting, float min, float max)
            {
                float value = getFloatValue(setting);
                float range = max - min;
                return value * range + min;
            }
            /*
             * Gets a bool setting value. If a non-float setting is attempted, false will be returned.
             */
            inline bool getBoolValue(PatchSetting setting) { return boolValues[setting]; }
    }; // class PatchSettings
} // namespace kiwi_synth


#endif // __PATCH_SETTINGS_H__
