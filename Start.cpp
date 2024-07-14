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

int main(void)
{
	hw.Configure();
	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400MHz
	hw.SetAudioBlockSize(48); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartLog(false);

	GPIO gpio1;
	gpio1.Init(seed::D22, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL);
	GPIO gpio2;
	gpio2.Init(seed::D23, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL);
	bool led1 = true;
	bool led2 = false;
	gpio1.Write(led1);
	gpio2.Write(led2);

	display.Init();

	kiwiSynth.Init(&hw, hw.AudioSampleRate());

	char message[64];
	kiwiSynth.ProcessInputs();
	if (kiwiSynth.BootLoaderRequested())
	{
		sprintf(message, "Starting BootLoader...");
		display.TestOutput(message);
		System::ResetToBootloader();
	}

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);

	sprintf(message, "Hello Kiwi!");
	display.TestOutput(message);
	uint16_t counter = 0;
    while(1)
	{
		hw.DelayMs(1);

		kiwiSynth.ProcessInputs();
		if (counter == 499) {
			kiwiSynth.TestOutput();

			gpio1.Write(led1);
			led1 = !led1;
			gpio2.Write(led2);
			led2 = !led2;
		}
		counter = (counter + 1) % 500;
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