#include "Voice.h"

namespace kiwi_synth
{
    Voice::Voice(int numVCOs, PatchSettings* patchSettings) : numVCOs(numVCOs), patchSettings(patchSettings)
    {
        vcos = (VCO**)malloc(numVCOs * sizeof(VCO));
        for (int i = 0; i < numVCOs; i++) {
            vcos[i] = new VCO(patchSettings);
        }
    }

    Voice::~Voice()
    {
        free(vcos);
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
        //return !vca.IsPlaying();
        return true;
    }


    bool Voice::IsReleasing()
    {
        //return vca.IsReleasing();
        return false;
    }
}
