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

    void Voice::Process(AudioHandle::OutputBuffer out, size_t size)
    {
        float vcoBuffer[numVCOs][size];
        for (int i = 0; i < numVCOs; i++)
        {
            vcos[i].Process(vcoBuffer[i], size);
        }
        for (size_t i = 0; i < size; i++)
        {
            // FOR NOW: Just use the first VCO outputting to the left output
            out[0][i] = vcoBuffer[0][i];
            out[1][i] = 0.0f;
        }
    }

    bool Voice::IsAvailable()
    {
        return !vca.IsPlaying();
    }


    bool Voice::IsReleasing()
    {
        return vca.IsReleasing();
    }
}
