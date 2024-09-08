#include "daisy_seed.h"
#include "KiwiSynth/KiwiSynth.h"
#include "KiwiSynth/Controls/KiwiMcp23017.h"
#include "KiwiSynth/Controls/GpioExpansion.h"
#include "KiwiSynth/GUI/Display.h"

using namespace daisy;
using namespace kiwi_synth;

//#define __CPU_LOAD__
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
// 85/98/91 distortion added                6


/*
 * TO DO
 * When a voice is triggered but not on, a second note can steal the voice so only one of the two sounds.
 * Test Sustain Pedal
 * Save / Load
 * UI requires button push and stops audio processing during menu
 * Polytimbral modes -- one or two effects engines? to which voice does the mod matrix apply? Probably one fx and a shared mod matrix.
 * Delay: Reverse, modulation, pitch shift when changing delay time (alter read/write speed instead of pointer position)
 * Chorus
 * Flanger
 * Phaser
 * External audio in
 * VariableShapeOsc for saw > tri > ramp effect
 * VariableShapeSaw for notch etc. pw effect
 * Chebyshev distortion like (4t^3-3t)+(2t^2-1)+t+1
 * ABS distortion like X-a(X*ABS(X))
 * Pre-filter vs output distortion
 * Modulation is currently 0.0 - 1.0. Can we also do -1.0 to 0.0?
 * Modulating modulations and effects
 * New mod destination that is just Noise to VCA Level (constant noise outside of envelope)
 * Can dust noise optionally bypass the VCF somehow?
 * Dust noise has some clicks/pops
 * Re-implement additional VCF modes?
 * Can we optimize to get 3 voice working with 3 VCOs again?
 * L/R Output noise
 * Headphone out noise
 * More text on display interferes with audio -- see DisplayWelcome
 * Going out of GUI mode sometimes triggers note on(s)
 */

DaisySeed hw;
KiwiSynth kiwiSynth;
Display display;
const int AUDIO_BLOCK_SIZE = 96;
#ifdef __CPU_LOAD__
	CpuLoadMeter load;
	char DSY_SDRAM_BSS buff[256];
#endif // __CPU_LOAD__

bool guiButton;
bool guiMode;

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
	#ifdef __CPU_LOAD__
		load.OnBlockStart();
	#endif // __CPU_LOAD__

	if (guiMode) {
		memset(out, 0, sizeof(float) * size * 2);
	} else {
		kiwiSynth.Process(out, size);
	}

	#ifdef __CPU_LOAD__
		load.OnBlockEnd();
	#endif // __CPU_LOAD__
}

void checkGuiMode()
{
	bool prevGuiButton = guiButton;
	guiButton = kiwiSynth.patchSettings.getBoolValue(GEN_SELECT_BUTTON);
	if (prevGuiButton && !guiButton) {
		kiwiSynth.AllNotesOff();
		guiMode = !guiMode;
		if (guiMode) {
			display.mode = INTS;
		} else {
			display.mode = PLAY;
		}
		display.Update();
	}
}

// Test chords: Am7 C/G F9 C/E
int main(void)
{
	guiMode = false;
	guiButton = false;

	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400MHz
	hw.SetAudioBlockSize(AUDIO_BLOCK_SIZE); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	#if defined(__CPU_LOAD__) || defined(__DEBUG__)
		hw.StartLog(false);
	#endif // __CPU_LOAD__ || __DEBUG__

	display.Init();

	kiwiSynth.Init(&hw, hw.AudioSampleRate());
	display.patchSettings = &kiwiSynth.patchSettings;

	kiwiSynth.ProcessInputs();
	if (kiwiSynth.BootLoaderRequested())
	{
		display.mode = BOOTLOADER;
		display.Update();
		System::ResetToBootloader(daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
	}
	display.mode = PLAY;
	display.Update();

	#ifdef __CPU_LOAD__
		load.Init(hw.AudioSampleRate(), AUDIO_BLOCK_SIZE);
	#endif // __CPU_LOAD__

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);

	uint16_t tick_counter = 0;
    while(1)
	{
		System::DelayUs(5);

		kiwiSynth.ProcessInputs();
		kiwiSynth.UpdateSettings();

		checkGuiMode();
		if (guiMode) {
			if (tick_counter == 127) {
				display.Update();

				#ifdef __DEBUG__
					kiwiSynth.TestOutput(&hw);
				#endif // __DEBUG__
				#ifdef __CPU_LOAD__
					sprintf(buff, "Min: %d, Max: %d, Avg: %d", (int)(load.GetMinCpuLoad() * 100), (int)(load.GetMaxCpuLoad() * 100), (int)(load.GetAvgCpuLoad() * 100));
					hw.PrintLine(buff);
				#endif // __DEBUG__
			}
			tick_counter++;
			tick_counter &= 0b01111111;
		}
	}
}
