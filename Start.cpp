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
 * Loaded patch mode updates values when abs(potValue - settings1.value) > changeDelta
 * System-wide default Mod settings 5-8 (not patch specific usually for pitch bend, modwheel, etc.). If any of 1-4 have that source, then the system wide one is overridden and ignored.
 * Alternate inputs to S&H (instead of noise)
 * Will fastSine work for LFOs and VCOs?
 * Paraphonic layered 3 "voice" mode.
 * Need to mod LFO1 to Pitch amount because of mod wheel
 * There is weird distortion with KiwiPhaser
 * Something is still popping faintly with note steal & retrigger
 * Note steal with all voice keys still held down does not retrigger envelope
 * Inverted amplitude envelope clicks when starting and stopping
 * Should some encoders like voice mode, waveform, and vcf type wraparound at max and min?
 * Separate FX 1 and FX 2 into separate settings?
 * Mod not applying to voice 2 sometimes -- see Vox Humana patch
 *  
 * Can we move any global or class vars to local stack vars?
 * Can we convert any ops into bitwise ops?
 * Can we absorb and optimize any more Daisy source?
 * Can we get the buffer lower
 * Can we implement portamento in paraphonic mode?
 * Save and load patches via Sysex
 * Layered patches are either not saving or not loading correctly. setting2 seems to not be playing with the expected settings. If you load twice it works.
 * Song mode: String together patches in a certain order. How to switch to next one though? ...but how to go backward and forward through the list?
 * Appegiator? Sequencer?
 * Can we get GPIO callbacks working?
 * For type vectors for PatchHeader lists I can't sort more than just by name. It crashes for some reason.
 * Can we get phaser back up to 4 or 6 pole?
 * Can we make the data structures a little smaller without losing performance?
 * This used to only be in UpdateSettings instead of Process. It caused a note blip because the note change delayed. Is there a way to make this an option again? It sounded cool. VCO.cpp line 53: playingNote = midiNote + octave + interval + fineTune + masterTune;
 * Make attack longer or a slower curve? Maybe also lower max release.
 * External audio in
 * FX Modes: Ensemble-Phaser. Sonic Annihilator.
 * Delay: Reverse, modulation, pitch shift when changing delay time (alter read/write speed instead of pointer position)
 * Ping Pong Delay
 * Bhāskara I's sine approximation formula
 * Chebyshev distortion like (4t^3-3t)+(2t^2-1)+t+1
 * ABS distortion like X-a(X*ABS(X))
 * Pre-filter vs output distortion
 * Modulating modulations and effects
 * New mod destination that is just Noise to VCA Level (constant noise outside of envelope)
 * Can dust noise optionally bypass the VCF somehow?
 * Performance %s are off if we increase above a 256 buffer
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

	if (__builtin_expect(display.mode, 0)) {
		kiwiSynth.AllNotesOff();
		kiwiSynth.ClearMidi();
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
	display.Init(&kiwiSynth, &performance);

	kiwiSynth.ProcessInputs(true);
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

	uint16_t counter = 0;
	uint16_t updateCounter = 0;
    while(1)
	{
		// It takes 16 iterations to cycle through all the inputs. So we only need to update settings for pots every 16th iteration.
		// However we will use 8 so knobs refresh more often. Otherwise the response quality might be unacceptable.
		updateCounter++;
		bool fullUpdate = false;
		if (updateCounter >= 8 || display.mode) { // For higher potentiometer refresh but worse encoder response, increase to updateCounter >= 16.
			fullUpdate = true;
			updateCounter = 0;
		}

		kiwiSynth.ProcessInputs(fullUpdate);
		if (fullUpdate) {
			kiwiSynth.UpdateSettings();
		}

		#ifdef __CPU_LOAD__
		if (!display.mode) {
			performance.Update(fullUpdate);
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
