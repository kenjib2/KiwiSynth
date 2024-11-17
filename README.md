# KiwiSynth
The Kiwi Synthesizer code

We will be mostly following these style guidelines:
https://geosoft.no/development/cppstyle.html

Code imported from other projects might not follow this same standard.

TO DO
* Since changing to 3 notes sometimes voices don't process NoteOff and keep playing
* Since changing to 3 notes sometimes voices don't start
* Load patches via Sysex
* Menu to change system-wide default mods and patch mod settings 5-8
* Inverted amplitude envelope clicks when starting and stopping
* External audio in
* Gate and Duck effects based on input
* Pitch detection - BACF, CMSIS, Yin Algorithm, Cycfi https://www.cycfi.com/2024/09/pitch-perfect-enhanced-pitch-detection-techniques-part-1/
* Song mode: String together patches in a certain order. How to switch to next one though? ...but how to go backward and forward through the list?
* Appegiator? Sequencer?
* Ping Pong Delay
* L/R Output noise: High pitched whine. It is there subtly on headphones too.

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
* Delay: Reverse, modulation, pitch shift when changing delay time (alter read/write speed instead of pointer position)
* Chebyshev distortion like (4t^3-3t)+(2t^2-1)+t+1
* ABS distortion like X-a(X*ABS(X))
* Pre-filter vs output distortion
* Modulating modulations and effects
* New mod destination that is just Noise to amplifer Level (constant noise outside of envelope)
* Can dust noise optionally bypass the filter somehow?
* Performance %s are off if we increase above a 256 buffer
