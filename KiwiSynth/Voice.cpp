#include "Voice.h"

namespace kiwi_synth
{
    Voice::Voice(int numVCOs, PatchSettings* patchSettings, float sampleRate) : numVCOs(numVCOs), patchSettings(patchSettings)
    {
        for (int i = 0; i < numVCOs; i++) {
            VCO* nextVco = new VCO(patchSettings, sampleRate);
            vcos.push_back(nextVco);
        }
        vcf = new VCF(patchSettings, sampleRate);
        vca = new VCA(patchSettings, sampleRate);
    }

    Voice::~Voice()
    {
        for (const VCO* nextVco : vcos) {
            delete nextVco;
        }
        delete vcf;
        delete vca;
    }

    void Voice::Process(AudioHandle::OutputBuffer out, size_t size)
    {
        float vcoBuffer[numVCOs][size];
        for (int i = 0; i < numVCOs; i++)
        {
            vcos[i]->Process(vcoBuffer[i], size);
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
        return !vca->IsPlaying();
    }


    bool Voice::IsReleasing()
    {
        return vca->IsReleasing();
    }
}
