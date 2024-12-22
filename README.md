# KiwiSynth
The Kiwi Synthesizer is a synthesizer with the following features:

* Stereo output with balance control, including balance modulation for panning effects
* OLED screen with GUI
* Support for MIDI input, including mod wheel, pitch bend, aftertouch, sustain, and expression implementations
* Knob-per-function hardware support for a virtual-analog live experience
* Eight banks with 128 preset slots per bank for a total of 1024 available patch slots
* Instrument and line in inputs (not yet implemented, but eventually for gating, ducking, pitch-tracking, FX processing, etc. modes)

* Three voice polyphony with mono mode
* Six oscillator paraphonic mode (expansion to 9 oscillator to be implemented)
* Mono split and layered multi-timbral modes
* Subtractive synthesis
* Phase modulation synthesis: PM synthesis is very similar to, and often called, FM Synthesis. The DX7 used PM, for example

* Three oscillators per voice
* Multiple shapeable waveforms selectable for each oscillator, creating an infinite range of timbrality
    * Polyblep square wave with pulse width
    * Polyblep sawtooth with limiting: Boosts and clips the waveform, creating a sound similar to moving from sawtooth to square wave
    * Triangle wave with wave folding
    * Sawtooth <---> triangle shaping
    * Sawtooth with variable notch
    * Sine wave folding
    * Sawtooth wave folding
* Modulation of all waveform shaping modes, including pulse width modulation
* Oscillator hard sync
* White and dust noise
* Portamento

* A filter per voice with multiple modes, including:
    * Ladder filter
    * Low pass filter
    * High pass filter
    * Notch filter
    * Peak filter
    * One pole lowpass filter
    * One pole highpass filter

* Two LFOs with all of the selectable waveforms listed above plus a ramp waveform, all with the same shaping options
* LFO retrigger on note on, including control of initial phase
* Two ASDR envelopes with reverse phase mode
* Sample and Hold
* A four part patch modulation matrix (expansion to 8 to be implemented)
* Four system-wide default modulations for MIDI controllers

* An effects engine, including:
    * Distortion and Delay
    * Stereo chorus and Delay
    * Phaser and Delay
    * Flanger and Delay
    * Distortion and Decimator (aka Bitcrusher)
* Stereo reverb with the following modes:
    * Room
    * Hall
    * Chamber
    * Cathedral
    * Bloom
    * Bright

We will be mostly following these style guidelines:
https://geosoft.no/development/cppstyle.html

Code imported from other projects might not follow this same standard.

HOWEVER please note that due to this project pushing the limits of the Daisy Seed's capabilities, there are many instances
where algorithms are implemented in what would traditionally be considered bad form. Most of these instances are done this
way to micro-manage performance and complete the audio processing using the fewest number of instructions after compilation.
Fixing what looks like bad form should be done cautiously and with knowledge of how it will effect the compilation of the
code.

TO DO
* Sysex load and save need to go straight from/to saved patches
* Expression system mod is disabled until the jack is fixed. VoiceBank.cpp line 384+
* CC messages for bank and patch changes
* Change six osc paraphonic to nine osc
* Menu to change system-wide default mods and patch mod settings 5-8
* Inverted amplitude envelope clicks when starting and stopping
* External audio in
* Gate and Duck effects based on input
* Pitch detection - BACF, CMSIS, Yin Algorithm, Cycfi https://www.cycfi.com/2024/09/pitch-perfect-enhanced-pitch-detection-techniques-part-1/
* Song mode: String together patches in a certain order. How to switch to next one though? ...but how to go backward and forward through the list?
* Appegiator? Sequencer?
* Ping Pong Delay
* Reverse Delay
* L/R Output noise: High pitched whine. It is there subtly on headphones too.
* When the voice mode is changed, can we push a display update to the play screen and show the change?
* Clean up too many global variables
* Clean up public class members if compiler can avoid extra steps for getters/setters

MAYBE TO DO
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
* Delay: Modulation, pitch shift when changing delay time (alter read/write speed instead of pointer position)
* Chebyshev distortion like (4t^3-3t)+(2t^2-1)+t+1
* ABS distortion like X-a(X*ABS(X))
* Pre-filter vs output distortion
* Modulating modulations and effects
* New mod destination that is just Noise to amplifer Level (constant noise outside of envelope)
* Can dust noise optionally bypass the filter somehow?
* Performance %s are off if we increase above a 256 buffer
