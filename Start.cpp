#include "daisy_seed.h"
#include "KiwiSynth/KiwiSynth.h"
//#include "KiwiSynth/Controls/GpioExpansion.h"
#include "dev/mcp23x17.h"

using namespace daisy;
using namespace kiwi_synth;

DaisySeed hw;
KiwiSynth kiwiSynth;
Oscillator osc;
//GpioExpansion* gpioExpansion1;
Mcp23017 mcp;
float sig;

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

	mcp.Init();
	mcp.PortMode(MCPPort::A, 0xFF); // Inputs
	mcp.PortMode(MCPPort::B, 0xFF); // Inputs
	//GpioExpansionConfig gpioExpansionConfig;
	//gpioExpansion1 = new GpioExpansion(&hw, &gpioExpansionConfig);

	osc.Init(hw.AudioSampleRate());
	osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
	osc.SetFreq(220);
	//kiwiSynth.Init(&hw, hw.AudioSampleRate());

    //Start reading ADC values
    //hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	//hw.StartAudio(AudioCallback);
    //Switch button;
	//button.Init(seed::D10, 0.0f, Switch::TYPE_TOGGLE, Switch::POLARITY_NORMAL, Switch::PULL_UP);
	/*dsy_gpio gpio;
	gpio.pin = hw.GetPin(9);
	gpio.mode = DSY_GPIO_MODE_INPUT;
	gpio.pull = DSY_GPIO_PULLUP;
	dsy_gpio_init(&gpio);*/

	uint8_t val = 2;
    while(1)
	{
		hw.DelayMs(500);
        mcp.Read();
        val = mcp.GetPin(0);
		//kiwiSynth->TestOutput();
		//u_int8_t value = gpioExpansion1->ReadPin(0);
		//value = dsy_gpio_read(&gpio);
		/*button.Debounce();
		if (button.Pressed()) {
			value = 1;
		} else {
			value = 0;
		}*/
    	char buff[256];
        sprintf(buff, "----------------------");
		hw.PrintLine(buff);
		hw.PrintLine(buff);
		sprintf(buff, "Pin 0 value: %d", val);
		hw.PrintLine(buff);
        sprintf(buff, "----------------------");
		hw.PrintLine(buff);
	}
}
