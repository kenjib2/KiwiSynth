#include "Voice.h"

namespace kiwi_synth
{
    void Voice::Init(int numVCOs, PatchSettings* patchSettings, float sampleRate)
    {
        this->numVCOs = numVCOs;
        this->patchSettings = patchSettings;
        for (int i = 0; i < numVCOs; i++) {
            VCO nextVco;
            nextVco.Init(patchSettings, sampleRate);
            vcos.push_back(nextVco);
        }
        vcf.Init(patchSettings, sampleRate);
        vca.Init(patchSettings, sampleRate);
    }

    void Voice::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
    {
        vcos[0].Process(out, size);
        vca.Process(out, size);
        /*float vcoBuffer[numVCOs][size];
        for (int i = 0; i < numVCOs; i++)
        {
            vcos[i].Process(vcoBuffer[i], size);
        }
        for(size_t i = 0; i < size; i += 2)
        {
            // Process
            out[i]     = vcoBuffer[0][i];
            out[i + 1] = vcoBuffer[0][i];
        }*/
    }

    bool Voice::IsAvailable()
    {
        return !vca.IsPlaying();
    }


    bool Voice::IsReleasing()
    {
        return vca.IsReleasing();
    }

    void Voice::NoteOn(int note, int velocity)
    {
        vcos[0].SetFreq(mtof(note));
        vca.NoteOn();
        // Voice = RequestVoice...
        // if (voice) voice->NoteOn and add to list of playing notes...
    }

    void Voice::NoteOff(int note, int velocity)
    {
        vca.NoteOff();
        /*for (size_t i = 0; i < voices.size(); i++) {
            Voice* voice = voices->FindVoice(); //if (voices[i]->noteTriggered && voices[i]->currentMidiNote == off.note) {
            if (voice) {
                voice->NoteOff(off.note, off.velocity);
                voiceBank->RemovePlayingVoice(voice);
                break;
            }
        }*/
    }

}
