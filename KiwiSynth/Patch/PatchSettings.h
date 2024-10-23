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
            int8_t floatValuesProtected[NUM_PATCH_SETTINGS];
            int8_t boolValuesProtected[NUM_PATCH_SETTINGS];
            // Since encoders work by incrementing or decrementing, we can just let them all be live all the time so no need to track them as protected.
            bool floatProtectedUpdateNeeded; // True indicates that we have switched to loaded mode and need to set the floatValuesProtected on the next update.
            uint8_t floatProtectedUpdateCounter; // Used to make sure we iterate through all 16 cycles of reading the pots.
            bool isLive;

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
             * Updates expansion based values with the current values in the GpioExpansion buffer.
             */
            void updateGpioExpansionValues(int controlNumber);
            /*
             * Sets up whether mod depth values are positive or negative depending on whether the current
             * values are positive or negative. 0.0f will count as positive.
             */
            void SetModSigns();
            /*
             * Sets values for float protection for the potentiometers for the given control number.
             */
            void UpdateFloatProtect(int controlNumber);
            /*
             * Sets values for float protection. -1 means the current pot value is below the current patch
             * setting. 1 means the current pot value is above the current patch setting. 0 means that the patch
             * setting has been crossed and the control is now reading live values.
             */
            void SetFloatProtect(PatchSetting patchSetting, float newValue);
            /*
             * Sets values for bool protection for the potentiometers for the given control number.
             */
            void UpdateBoolProtect(int controlNumber);
            /*
             * Sets values for bool protection. 1 means the current pot value is different from the current patch
             * setting. 0 means that the patch setting has been matched and the control is now reading live values.
             */
            inline void SetBoolProtect(PatchSetting patchSetting, float newValue) { if (newValue == getBoolValue(patchSetting)) boolValuesProtected[patchSetting] = 0; }

        public:
            float modSigns[8]; // values should be 1.0f or -1.0f only

            PatchSettings() {}
            ~PatchSettings() {}
            void Init(MultiPots* multiPots, GpioExpansion* ge);
            void DefaultSettings();
            void SetControlsLive(bool isLive);
            void Copy(SavedPatch* savedPatch, int voiceNumber);
            void Copy(PatchSettings* patchSettings);
            void CopyVoiceSettings(PatchSettings* patchSettings);
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
            inline void setValue(PatchSetting setting, float value) { if (!floatValuesProtected[setting]) floatValues[setting] = value; }
            /*
             * Sets a bool setting value. If a non-bool setting is attempted, no action will be performed.
             */
            inline void setValue(PatchSetting setting, bool value) { if (!boolValuesProtected[setting]) boolValues[setting] = value; }

            /*
             * Sets a float setting value regardless of loaded or live settings. This should only be used for live performance settings, like
             * pitch bend and mod wheel.
             */
            inline void forceSetValue(PatchSetting setting, float value) { floatValues[setting] = value; }
            /*
             * Sets a bool setting value regardless of loaded or live settings. This should only be used for live performance settings, like
             * pitch bend and mod wheel.
             */
            inline void forceSetValue(PatchSetting setting, bool value) { boolValues[setting] = value; }

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
