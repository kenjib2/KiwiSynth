#include "daisy_seed.h"
#include "KiwiSynth/KiwiSynth.h"

using namespace daisy;
using namespace kiwi_synth;

DaisySeed hw;
KiwiSynth* kiwiSynth;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	kiwiSynth->Process(out, size);
	/*for (size_t i = 0; i < size; i++)
	{
		//out[0][i] = in[0][i];
		//out[1][i] = in[1][i];
	}*/
}

int main(void)
{
	hw.Configure();
	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400,000MHz
	hw.StartLog(false);

	kiwiSynth = new KiwiSynth(&hw);

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);

    while(1)
	{
		hw.DelayMs(500);
		kiwiSynth->TestOutput();
	}
}
