#include "KiwiSynth.h"

namespace kiwi_synth
{
    KiwiSynth::KiwiSynth(DaisySeed* hw) : hw(hw)
    {
        ConfigureMultiPots();
        patchSettings = new PatchSettings(multiPots);
        for (int i = 0; i < NUM_VOICES; i++)
        {
            voices[i] = new Voice(NUM_VOICES, patchSettings);
        }
        multiPots->RegisterControlListener(patchSettings, ControlId::MULTIPOTS);
    }

    KiwiSynth::~KiwiSynth()
    {
        for (int i = 0; i < NUM_VOICES; i++)
        {
            if (voices[i]) {
                delete voices[i];
            }
        }
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
        multiPotsConfig.refreshRate = 50; // One half of a ms, which means 8ms for all pots to refresh. Shorter can cause timing problems.

        multiPots = new MultiPots(hw, &multiPotsConfig);
        multiPots->StartTimer();

        free(mpPins);
        free(directPins);
    }

    void KiwiSynth::Process(AudioHandle::OutputBuffer out, size_t size)
    {
        // FOR NOW JUST TAKE THE OUTPUT OF THE FIRST VOICE
        voices[0]->Process(out, size);
    }

    void KiwiSynth::TestOutput()
    {
    	char buff[256];
        sprintf(buff, "----------------------");
		hw->PrintLine(buff);

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
