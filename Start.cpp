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
// 58/64/59 no oscillator processing			26 (was 30)
// 81/88/83 no noise/SH (was off anyway)    	2 (was 5)
// 83/91/85 no amplifer							0 (was 0)
// 71/78/73 no filter                       	12 (was 22)
// 68/70/69 no reverb/effectsEngine				16 (was 20)
// 82/88/83 no MIDI								2 (was 4)
// 80/88/81 no calculateMods					4 (was 8)
// 72/79/74 fullFunctionality = false			11 (was 16)
// 81/88/82 remove fmod from lfo            	3
// 76/83/77 remove waveshaping from oscillator	8
// Most expensive: reverb, lfo, oscillator, filter, reverb
// 85/98/91 distortion added                	6
// 87/96/90 new benchmark 3Osc+N no dist	 	*****
// 87/100/92 new benchmark 3Osc+N with dist	 	*****


/*
 * TO DO
 * 
 * On initial start up osc 2 & 3 don't play if turned on but do play if you turn them off and back on. Maybe only when flashing BIOS?
 * Inverted amplitude envelope clicks when starting and stopping
 * Menu to change system-wide default mods and patch mod settings 5-8
 * Save and load patches via Sysex
 * External audio in
 * Gate and Duck effects based on input
 * Pitch detection - BACF, CMSIS, Yin Algorithm, Cycfi https://www.cycfi.com/2024/09/pitch-perfect-enhanced-pitch-detection-techniques-part-1/
 * Song mode: String together patches in a certain order. How to switch to next one though? ...but how to go backward and forward through the list?
 * Appegiator? Sequencer?
 * Ping Pong Delay
 * L/R Output noise: It is from the OLED: It pretty much needs its own whole power section: own regulator, 100uF+ filter caps, and an RLC filter if you can. 
 * Headphone out noise
 * More text on display interferes with audio -- see DisplayWelcome. It is independent of the volume knob. Probably the main source of noise.
 *
 * Paraphonic layered 3 "voice" mode.
 * Separate FX 1 and FX 2 into separate settings? Can add a delay mode so you can swap out normal delay for ping-pong etc.
 * Should some encoders like voice mode, waveform, and filter type wraparound at max and min?
 * Removing IntScreen causes major performance problems -- ???
 * Is something still popping faintly with note steal & retrigger?
 * Can we make click only show up on the very lowest attack and decay settings somehow?
 * Can we move any global or class vars to local stack vars?
 * Can we convert any ops into bitwise ops?
 * Can we absorb and optimize any more Daisy source?
 * Can we get the buffer lower
 * Can we implement portamento in paraphonic mode?
 * Layered patches are either not saving or not loading correctly. setting2 seems to not be playing with the expected settings. If you load twice it works.
 * For type vectors for PatchHeader lists I can't sort more than just by name. It crashes for some reason.
 * Can we make the data structures a little smaller without losing performance?
 * This used to only be in UpdateSettings instead of Process. It caused a note blip because the note change delayed. Is there a way to make this an option again? It sounded cool. Oscillator.cpp line 53: playingNote = midiNote + octave + interval + fineTune + masterTune;
 * FX Modes: Ensemble-Phaser. Sonic Annihilator.
 * Delay: Reverse, modulation, pitch shift when changing delay time (alter read/write speed instead of pointer position)
 * Chebyshev distortion like (4t^3-3t)+(2t^2-1)+t+1
 * ABS distortion like X-a(X*ABS(X))
 * Pre-filter vs output distortion
 * Modulating modulations and effects
 * New mod destination that is just Noise to amplifer Level (constant noise outside of envelope)
 * Can dust noise optionally bypass the filter somehow?
 * Performance %s are off if we increase above a 256 buffer
 * Going out of GUI mode sometimes triggers note on(s)
 */

// Patch idea -- very slow square LFO with super short PW. Have it trigger on short PW first, then "blip" up/down to the note. Chip-tunesy

DaisySeed hw;
KiwiSynth kiwiSynth;
Display display;
const int AUDIO_BLOCK_SIZE = 384;
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

// Test chords: Am7 C/G F9 C/E
int main(void)
{
	hw.Init(true); // true boosts it to 480MHz clock speed. Default would be 400MHz
	hw.SetAudioBlockSize(AUDIO_BLOCK_SIZE); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	kiwiSynth.Init(&hw, hw.AudioSampleRate());
	#ifdef __CPU_LOAD__
	load.Init(hw.AudioSampleRate(), AUDIO_BLOCK_SIZE);
	performance.Init(&load);
	#endif // __CPU_LOAD__
	display.Init(&kiwiSynth, &performance);

	kiwiSynth.ProcessInputs();
	if (kiwiSynth.BootLoaderRequested())
	{
		display.mode = MODE_BOOTLOADER;
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
		if (!display.mode) {
			performance.Update();
		}
		#endif // __CPU_LOAD__

		if (counter1 == 15) {
			display.HandleInput();

			if (display.mode && audioRunning) {
				hw.StopAudio();
				audioRunning = false;
			} else if (!display.mode && !audioRunning) {
				kiwiSynth.AllNotesOff();
				kiwiSynth.ClearMidi();
				hw.StartAudio(AudioCallback);
				audioRunning = true;
			}

			if (display.mode && counter2 == 15) {
				display.Update(); // Update to capture live control changes on the settings screens
			}
			counter2++;
			counter2 &= 0b000001111;
		}
		counter1++;
		counter1 &= 0b000011111;
	}
}
