#include "daisy_seed.h"
#include "KiwiSynth/KiwiSynth.h"
#include "KiwiSynth/Controls/KiwiMcp23017.h"
#include "KiwiSynth/Controls/GpioExpansion.h"
#include "KiwiSynth/GUI/Display.h"

using namespace daisy;
using namespace kiwi_synth;

#define __CPU_LOAD__
// 80/87/81 remove lfo osc.process          3  (fmod to fclamp makes it 6, remove waveshaping also makes it 8)

// 83/90/85 all active
// 78/87/80 no envelope processing          5 (was 7)
// 73/80/84 no LFO processing               11 (was 15)
// 58/64/59 no vco processing				26 (was 30)
// 81/88/83 no noise/SH (was off anyway)    2 (was 5)
// 83/91/85 no vca							0 (was 0)
// 71/78/73 no vcf                          12 (was 22)
// 68/70/69 no reverb/effectsEngine			16 (was 20)
// 82/88/83 no MIDI							2 (was 4)
// 80/88/81 no calculateMods				4 (was 8)
// 72/79/74 fullFunctionality = false		11 (was 16)
// 81/88/82 remove fmod from lfo            3
// 76/83/77 remove waveshaping from vco     8
// Most expensive: reverb, lfo, vco, vcf, reverb



/*
 * TO DO
 * When a voice is triggered but not on, a second note can steal the voice so only one of the two sounds.
 * Test Expression
 * Test Pedal
 * Distortion atan softer, tanh harsher -- maybe upsample to fs * 2 then distort, then down sample to fs
 * Chebyshev distortion like (4t^3-3t)+(2t^2-1)+t+1
 * ABS distortion like X-a(X*ABS(X))
 * Pre-filter vs output distortion
 * Pitch bend down is off applied to VCO freq -- goes down farther than up
 * Delay
 * Chorus
 * Flanger
 * Phaser
 * Modulating modulations and effects
 * New mod destination that is just Noise to VCA Level (constant noise outside of envelope)
 * Dust noise has some clicks/pops
 * Re-implement additional VCF modes?
 * Save / Load
 * Polytimbral modes -- one or two effects engines? to which voice does the mod matrix apply? Probably one fx and a shared mod matrix.
 * Can we optimize to get 3 voice working with 3 VCOs again?
 * UI
 * L/R Output noise
 * Headphone out noise
 */

DaisySeed hw;
KiwiSynth kiwiSynth;
Display display;
#ifdef __CPU_LOAD__
	CpuLoadMeter load;
	char DSY_SDRAM_BSS buff[256];
	// 1 voice max 46, avg 41
	// 1 voice 1 VCO max 46, avg 38
	// 2 voice max 75, avg 70
	// 2 voice 1 VCO max 63, avg 62
	// 2 voice 2 VCO max 67, avg 65
	// 2 voice no reverb max 63, 60 avg 
	// 2 voice no noise or SH max 67, avg 65
	// 3 voice max 99, avg 96 then CRASH
	// 3 voice no noise or SH max 94, avg 92
	// 3 voice no noise or SH or polyblep max 90, avg 88
	// 3 voice 2 VCO no noise or SH max 88, avg 86
	// Reverb costs @ 10%
#endif // __CPU_LOAD__


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

// Test chords: Am7 C/G F9 C/E
int main(void)
{
	hw.Configure();
	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400MHz
	hw.SetAudioBlockSize(48); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	#if defined(__CPU_LOAD__) || defined(__DEBUG__)
		hw.StartLog(false);
	#endif // __CPU_LOAD__ || __DEBUG__

	display.Init();

	char message[64];
	kiwiSynth.Init(&hw, hw.AudioSampleRate());

	kiwiSynth.ProcessInputs();
	if (kiwiSynth.BootLoaderRequested())
	{
		sprintf(message, "Ready for Update...");
		display.TestOutput(message);
		System::ResetToBootloader(daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
	}
	sprintf(message, "Hello Kiwi!");
	display.TestOutput(message);

	#ifdef __CPU_LOAD__
		load.Init(hw.AudioSampleRate(), 48);
	#endif // __CPU_LOAD__

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);

	#if defined(__CPU_LOAD__) || defined(__DEBUG__)
		uint16_t counter = 0;
	#endif // __DEBUG__
    while(1)
	{
		System::DelayUs(5);

		kiwiSynth.ProcessInputs();
		kiwiSynth.UpdateSettings();

		#ifdef __DEBUG__
			if (counter == 499) {
				kiwiSynth.TestOutput(&hw);
			}
			counter = (counter + 1) % 500;
		#endif // __DEBUG__
		#ifdef __CPU_LOAD__
			if (counter == 499) {
				sprintf(buff, "Min: %d, Max: %d, Avg: %d", (int)(load.GetMinCpuLoad() * 100), (int)(load.GetMaxCpuLoad() * 100), (int)(load.GetAvgCpuLoad() * 100));
	            hw.PrintLine(buff);
			}
			counter = (counter + 1) % 500;
		#endif // __CPU_LOAD__
	}
}
