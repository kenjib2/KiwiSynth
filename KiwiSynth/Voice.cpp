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

        osc.Init(sampleRate);
        osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SQUARE);
        osc.SetFreq(220);
        osc.SetAmp(0.5f);
    }

    void Voice::Process(AudioHandle::InterleavingOutputBuffer out, size_t size)
    {
        for(size_t i = 0; i < size; i += 2)
        {
            // Process
            float sig        = osc.Process();
            out[i]     = sig;
            out[i + 1] = sig;
        }
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

    void Voice::SetFreq(int oscNumber, float frequency)
    {
        osc.SetFreq(frequency);
    }

}
