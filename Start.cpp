#include "daisy_seed.h"
#include "KiwiSynth/KiwiSynth.h"
#include "KiwiSynth/Controls/KiwiMcp23017.h"
#include "KiwiSynth/Controls/GpioExpansion.h"

using namespace daisy;
using namespace kiwi_synth;

DaisySeed hw;
KiwiSynth kiwiSynth;
Oscillator osc;
float sig;
//GpioExpansion ge;


//void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    for(size_t i = 0; i < size; i += 2)
    {
        // Process
        sig        = osc.Process();
        out[i]     = sig;
        out[i + 1] = sig;
    }
	//kiwiSynth->Process(out, size);
}

int main(void)
{
	hw.Configure();
	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400,000MHz
	hw.SetAudioBlockSize(48); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartLog(false);

	//GPIO gpio;
	//gpio.Init(seed::D18, GPIO::Mode::INTERRUPT_RISING, GPIO::Pull::NOPULL, GPIO::Speed::LOW, GpioExpansionInterruptCallback);

	/*GPIO gpio1;
	gpio1.Init(seed::D22, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL);
	GPIO gpio2;
	gpio2.Init(seed::D23, GPIO::Mode::OUTPUT, GPIO::Pull::NOPULL);*/

/*	GpioExpansionConfig cfg;
	cfg.Defaults();
	cfg.numGpioExpansions = 1;
	uint16_t map[1];
	map[0] = 0b0111111111111111;
	//map[1] = 0b1111111111111111;
	//map[2] = 0b1111111111111111;
	//map[3] = 0b1111111111111111;
	cfg.pullupMap = map;
	ge.Init(&cfg);
	//ge.StartTimer();*/

	osc.Init(hw.AudioSampleRate());
	osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
	osc.SetFreq(220);
	osc.SetAmp(0.5f);
	kiwiSynth.Init(&hw, hw.AudioSampleRate());

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);

	//bool led1 = false;
	//bool led2 = false;
	//char buff[256];
    while(1)
	{
		hw.DelayMs(500);
		kiwiSynth.TestOutput();

		//gpio1.Write(led1);
		//led1 = !led1;

		//gpio2.Write(led2);
		//led2 = !led2;

		/*bool val = ge.interrupt.Read();
		pinValues = ge.getPinValues(0);
		hw.PrintLine(tempBuffer);
		sprintf(buff, "pinValues: %d, interrupt %s", pinValues, val ? "true" : "false");
		hw.PrintLine(buff);*/
		/*sprintf(buff, "interrupt %s", gpioReadRequired ? "true" : "false");
		hw.PrintLine(buff);
		if (gpioReadRequired) {
			pinValues = ge.mcps[0].Read() & 0b0000000000000001;
			ge.mcps[0].clearInterrupts();
			sprintf(buff, "--Interrupted--");
			hw.PrintLine(buff);
			sprintf(buff, "Pin value: %d", pinValues);
			hw.PrintLine(buff);
			sprintf(buff, "Interrupt: %lu", kiwi_synth::gpioLastInterruptTime);
			hw.PrintLine(buff);
			gpioReadRequired = false;
		}*/
		/*if (gpioReadRequired) {
			//ProcessGpioExpansionTimer(&ge);
			//ProcessGpioExpansionTimer(nullptr);
			//pinRead = mcp.Read() & 0b0000000000000001;
			//mcp.clearInterrupts();
			ge.ClearInterrupts();
			sprintf(buff, "--Interrupted--");
			hw.PrintLine(buff);
			sprintf(buff, "Pin value: %d", pinRead);
			hw.PrintLine(buff);
			sprintf(buff, "Interrupt: %lu", gpioLastInterruptTime);
			hw.PrintLine(buff);
		}*/
	}
}
