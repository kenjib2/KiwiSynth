#include "daisy_seed.h"
#include "KiwiSynth/KiwiSynth.h"
#include "KiwiSynth/Controls/KiwiMcp23017.h"

using namespace daisy;
using namespace kiwi_synth;

DaisySeed hw;
KiwiSynth kiwiSynth;
Oscillator osc;
KiwiMcp23017 mcp;
float sig;
uint16_t pinValue = 0;

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
	mcp.PortMode(kiwi_synth::MCPPort::A, 0xFF); // Inputs
	mcp.PortMode(kiwi_synth::MCPPort::B, 0xFF); // Inputs

    mcp.interruptMode(MCP23017InterruptMode::Or);
	mcp.interrupt(kiwi_synth::MCPPort::A, CHANGE);
	mcp.interrupt(kiwi_synth::MCPPort::B, CHANGE);

	osc.Init(hw.AudioSampleRate());
	osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
	osc.SetFreq(220);
	//kiwiSynth.Init(&hw, hw.AudioSampleRate());

    //Start reading ADC values
    //hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	//hw.StartAudio(AudioCallback);

    while(1)
	{
		hw.DelayMs(500);
		//kiwiSynth->TestOutput();
	}
}
