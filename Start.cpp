#include "daisy_seed.h"

#include "KUtils.h"
#include "KiwiSynth/KiwiSynth.h"
#include "KiwiSynth/Controls/KiwiMcp23017.h"
#include "KiwiSynth/Controls/GpioExpansion.h"
#include "KiwiSynth/GUI/Display.h"

using namespace daisy;
using namespace kiwi_synth;

// 83/90/85 all active
// 78/87/80 no envelope processing          	5 (was 7)
// 73/80/84 no LFO processing               	11 (was 15)
// 58/64/59 no vco processing					26 (was 30)
// 81/88/83 no noise/SH (was off anyway)    	2 (was 5)
// 83/91/85 no vca								0 (was 0)
// 71/78/73 no vcf                          	12 (was 22)
// 68/70/69 no reverb/effectsEngine				16 (was 20)
// 82/88/83 no MIDI								2 (was 4)
// 80/88/81 no calculateMods					4 (was 8)
// 72/79/74 fullFunctionality = false			11 (was 16)
// 81/88/82 remove fmod from lfo            	3
// 76/83/77 remove waveshaping from vco     	8
// Most expensive: reverb, lfo, vco, vcf, reverb
// 85/98/91 distortion added                	6
// 87/96/90 new benchmark 3VCO+N no dist	 	*****
// 87/100/92 new benchmark 3VCO+N with dist	 	*****


/*
 * TO DO
 * 
 * Save / Load / Rename
 * UI select and modify
 * Loaded patch mode (turns off panel input -- or only updates values when abs(potValue - settings1.value) > changeDelta)
 * Alternate inputs to S&H (instead of noise)
 * Will fastSine work for LFOs and VCOs?
 * FX Modes: Ensemble-Phaser. Sonic Annihilator.
 * 
 * Can we get phaser back up to 4 or 6 pole?
 * Can we make the data structures a little smaller without losing performance?
 * Update Zita to handle larger delay values for Bloom reverb
 * This used to only be in UpdateSettings instead of Process. It caused a note blip because the note change delayed. Is there a way to make this an option again? It sounded cool. VCO.cpp line 53: playingNote = midiNote + octave + interval + fineTune + masterTune;
 * Is there some way to read more than one channel per pass from the multiplexer or speed it up in any other way???
 * Make attack longer or a slower curve?
 * External audio in
 * Delay: Reverse, modulation, pitch shift when changing delay time (alter read/write speed instead of pointer position)
 * Bhāskara I's sine approximation formula
 * Chebyshev distortion like (4t^3-3t)+(2t^2-1)+t+1
 * ABS distortion like X-a(X*ABS(X))
 * Pre-filter vs output distortion
 * Modulation is currently 0.0 - 1.0. Can we also do -1.0 to 0.0?
 * Modulating modulations and effects
 * New mod destination that is just Noise to VCA Level (constant noise outside of envelope)
 * Can dust noise optionally bypass the VCF somehow?
 * L/R Output noise
 * Headphone out noise
 * More text on display interferes with audio -- see DisplayWelcome. It is independent of the volume knob. Probably the main source of noise.
 * Very occasional pops when distortion is on
 * Going out of GUI mode sometimes triggers note on(s)
 * Can we optimize more to get the audio buffer size lower?
 */

// Patch idea -- very slow square LFO with super short PW. Have it trigger on short PW first, then "blip" up/down to the note. Chip-tunesy

DaisySeed hw;
KiwiSynth kiwiSynth;
Display display;
const int AUDIO_BLOCK_SIZE = 256;
Performance performance;
CpuLoadMeter load;

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
	#ifdef __CPU_LOAD__
	load.OnBlockStart();
	#endif // __CPU_LOAD__

	if (display.mode) {
		kiwiSynth.AllNotesOff();
		memset(out, 0, sizeof(float) * size * 2);
	} else {
		kiwiSynth.Process(out, size);
	}

	#ifdef __CPU_LOAD__
	load.OnBlockEnd();
	#endif // __CPU_LOAD__
}

// Test chords: Am7 C/G F9 C/E
int main(void)
{
	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400MHz
	hw.SetAudioBlockSize(AUDIO_BLOCK_SIZE); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	#if defined(__PATCH_SETTINGS__)
	hw.StartLog(false);
	#endif // __PATCH_SETTINGS__

	kiwiSynth.Init(&hw, hw.AudioSampleRate());
	#ifdef __CPU_LOAD__
	load.Init(hw.AudioSampleRate(), AUDIO_BLOCK_SIZE);
	performance.Init(&load);
	#endif // __CPU_LOAD__
	display.Init(&(kiwiSynth.patch), &performance);

	kiwiSynth.ProcessInputs();
	if (kiwiSynth.BootLoaderRequested())
	{
		display.mode = BOOTLOADER;
		display.Update();
		System::ResetToBootloader(daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
	}
	display.mode = PLAY;
	display.Update();

    //Start reading ADC values
    hw.adc.Start(); // The start up will hang for @20 seconds if this is attempted before creating KiwiSynth (and initializing pins)
	hw.StartAudio(AudioCallback);

	uint16_t counter = 0;
    while(1)
	{
		// We used to need a System::DelayUs(5); here to give the Multiplexer time to switch channels. No longer needed because of
		// display.HandleInput() taking some time to finish and serving the same purpose.
		kiwiSynth.ProcessInputs();
		kiwiSynth.UpdateSettings();

		#ifdef __CPU_LOAD__
		if (!display.mode) {
			performance.Update();
		}
		#endif // __CPU_LOAD__

		display.HandleInput();
		if (counter == 255) {
			if (display.mode) {
				display.Update();
			}
			#ifdef __PATCH_SETTINGS__
			kiwiSynth.TestOutput(&hw);
			#endif // __PATCH_SETTINGS__
		}
		counter++;
		counter &= 0b011111111;
	}
}
