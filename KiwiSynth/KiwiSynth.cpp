#include "KiwiSynth.h"

namespace kiwi_synth
{
    KiwiSynth::KiwiSynth(DaisySeed* hw)
    {
        this->hw = hw;
        ConfigureMultiPots();
        patchSettings = new PatchSettings(multiPots);
    }

    void KiwiSynth::ConfigureMultiPots()
    {
        int numMps = 3;
        int numChannels = 16;
        int numDirectPots = 1;

        Pin *mpPins = new Pin[numMps];
        mpPins[0] = A0;
        mpPins[1] = A1;
        mpPins[2] = A2;

        Pin *directPins = new Pin[numDirectPots];
        directPins[0] = A4;

        MultiPotsConfig multiPotsConfig;
        multiPotsConfig.numMps = numMps;
        multiPotsConfig.numChannels = numChannels;
        multiPotsConfig.numDirectPots = numDirectPots;
        multiPotsConfig.pinA0 = D7;
        multiPotsConfig.pinA1 = D8;
        multiPotsConfig.pinA2 = D9;
        multiPotsConfig.pinA3 = D10;
        multiPotsConfig.pinsSignal = mpPins;
        multiPotsConfig.pinsDirect = directPins;
        multiPotsConfig.useTimer = true;
        multiPotsConfig.refreshRateMs = 2.5f;

        multiPots = new MultiPots(hw, &multiPotsConfig);
        multiPots->StartTimer();

        free(mpPins);
        free(directPins);
    }

    void KiwiSynth::Process()
    {
		patchSettings->updatePotValues();

    	char buff[256];

		float val0 = patchSettings->getFloatValue(PatchSetting::VCO_MASTER_TUNE);
		sprintf(buff, "Pot 0-0 value: %.3f", val0);
		hw->PrintLine(buff);

		float val1 = patchSettings->getFloatValue(PatchSetting::VCO_PORTAMENTO_SPEED);
		sprintf(buff, "Pot 0-1 value: %.3f", val1);
		hw->PrintLine(buff);

		float valD0 = patchSettings->getFloatValue(PatchSetting::GEN_BALANCE);
		sprintf(buff, "Pot D0 value: %.3f", valD0);
		hw->PrintLine(buff);
    }
} // namespace kiwi_synth
