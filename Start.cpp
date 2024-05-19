#include "daisy_seed.h"
#include "KiwiSynth/KiwiSynth.h"

using namespace daisy;
using namespace kiwi_synth;

DaisySeed hw;
KiwiSynth* kiwiSynth;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	kiwiSynth->Process(out, size);
}

int main(void)
{
	hw.Configure();
	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400,000MHz
	hw.SetAudioBlockSize(48); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartLog(false);

	kiwiSynth = new KiwiSynth(&hw, hw.AudioSampleRate());

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);

    while(1)
	{
		hw.DelayMs(500);
		kiwiSynth->TestOutput();
	}
}
