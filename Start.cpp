#include "daisy_seed.h"

#include "KiwiSynth/Util/KUtils.h"
#include "KiwiSynth/KiwiSynth.h"
#include "KiwiSynth/Controls/KiwiMcp23017.h"
#include "KiwiSynth/Controls/GpioExpansion.h"
#include "KiwiSynth/GUI/Display.h"



using namespace daisy;
using namespace kiwisynth;



DaisySeed hw;
KiwiSynth kiwiSynth;
Display display;
Performance performance;
CpuLoadMeter load;
bool audioRunning;



void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
	#ifdef __CPU_LOAD__
	load.OnBlockStart();
	#endif // __CPU_LOAD__

	kiwiSynth.Process(out, size);

	#ifdef __CPU_LOAD__
	load.OnBlockEnd();
	#endif // __CPU_LOAD__
}



int main(void)
{
	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400MHz
	hw.SetAudioBlockSize(AUDIO_BLOCK_SIZE); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_32KHZ);

	kiwiSynth.Init(&hw, hw.AudioSampleRate());
	#ifdef __CPU_LOAD__
	load.Init(hw.AudioSampleRate(), AUDIO_BLOCK_SIZE);
	performance.Init(&load);
	#endif // __CPU_LOAD__
	display.Init(&kiwiSynth, &performance);

	kiwiSynth.ProcessInputs(true);
	if (kiwiSynth.BootLoaderRequested())
	{
		display.mode_ = MODE_BOOTLOADER;
		display.Update();
		System::ResetToBootloader(daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
	}
	display.Update();

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);
	audioRunning = true;

	uint16_t counter1 = 0;
	uint16_t counter2 = 0;
    while(1)
	{
		kiwiSynth.ProcessInputs(); // Note that if the GPIO interrupt is not high, we will give the multiplexer time to change channels in ProcessInputs for the GPIO.
		kiwiSynth.UpdateSettings();

		#ifdef __CPU_LOAD__
		if (!display.mode_) {
			performance.Update();
		}
		#endif // __CPU_LOAD__

		if (counter1 == 15) {
			display.HandleInput();

			if (display.mode_ && audioRunning) {
				hw.StopAudio();
				audioRunning = false;
			} else if (!display.mode_ && !audioRunning) {
				kiwiSynth.AllNotesOff();
				kiwiSynth.ClearMidi();
				hw.StartAudio(AudioCallback);
				audioRunning = true;
			}

			if (display.mode_ && counter2 == 15) {
				display.Update(); // Update to capture live control changes on the settings screens
			}
			counter2++;
			counter2 &= 0b000001111;
		}
		counter1++;
		counter1 &= 0b000011111;
	}
}
