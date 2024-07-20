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


	/*GPIO gpio2;
	gpio2.Init(seed::D23, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL);
	gpio2.Write(false);*/

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

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);

	sprintf(message, "Hello Kiwi!");
	display.TestOutput(message);
	//uint16_t counter = 0;
    while(1)
	{
		System::DelayUs(5);

		kiwiSynth.ProcessInputs();
		/*if (counter == 999) {
			//kiwiSynth.TestOutput(&hw);

			gpio1.Write(led1);
			led1 = !led1;
			gpio2.Write(led2);
			led2 = !led2;
		}
		counter = (counter + 1) % 1000;*/
	}
}


/*
void RebootToBootloader()
{
	// Initialize Boot Pin
	dsy_gpio_pin bootpin = {DSY_GPIOG, 3};
	dsy_gpio pin;
	pin.mode = DSY_GPIO_MODE_OUTPUT_PP;
	pin.pin = bootpin;
	dsy_gpio_init(&pin);

	// Pull Pin HIGH
	dsy_gpio_write(&pin, 1);

	// wait a few ms for cap to charge
	hardware.DelayMs(10);

	// Software Reset
	HAL_NVIC_SystemReset();
}
*/