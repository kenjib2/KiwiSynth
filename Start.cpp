//#define __DEBUG__

#include "daisy_seed.h"
#include "KiwiSynth/KiwiSynth.h"
#include "KiwiSynth/Controls/KiwiMcp23017.h"
#include "KiwiSynth/Controls/GpioExpansion.h"
#include "KiwiSynth/GUI/Display.h"

using namespace daisy;
using namespace kiwi_synth;

DaisySeed hw;
KiwiSynth kiwiSynth;
Display display;


void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
	kiwiSynth.Process(out, size);
}

// Test chords: Am7 C/G F9 C/E
int main(void)
{
	hw.Configure();
	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400MHz
	hw.SetAudioBlockSize(48); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartLog(false);

	display.Init();

	kiwiSynth.Init(&hw, hw.AudioSampleRate());

	char message[64];
	kiwiSynth.ProcessInputs();
	if (kiwiSynth.BootLoaderRequested())
	{
		sprintf(message, "BootLoader Started");
		display.TestOutput(message);
		System::ResetToBootloader();
	}
	sprintf(message, "Hello Kiwi!");
	display.TestOutput(message);

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);

	#ifdef __DEBUG__
		uint16_t counter = 0;
	#endif // __DEBUG__
    while(1)
	{
		System::DelayUs(5);

		kiwiSynth.ProcessInputs();
		kiwiSynth.UpdateSettings();

		#ifdef __DEBUG__
			if (counter == 999) {
				kiwiSynth.TestOutput(&hw);
			}
			counter = (counter + 1) % 1000;
		#endif // __DEBUG__
	}
}
