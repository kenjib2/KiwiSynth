#ifndef __KIWI_SYNTH_KUTILS_H__
#define __KIWI_SYNTH_KUTILS_H__

// Compiler options
#define __CPU_LOAD__ // Output performance to serial console
//#define __PATCH_SETTINGS__ // Output patch settings to serial console

#include "daisy_seed.h"

using namespace daisy;

const static uint32_t MAX_XORSHIFT_VALUE = 4294967295;

class xorshiftPRNG {
  private:
  struct xorshift_state {
     uint64_t a32;
  };
  xorshift_state state;

  public:
  xorshiftPRNG(int seed)
  {
     state.a32 = seed;
  }


  uint32_t xorshift32()
  {
     uint32_t x = state.a32;
     x ^= x <<17;
     x ^= x >>7;
     x ^= x <<5;
     state.a32 = x;
     return (x);
  }


};

class Performance {
   public:
      float avg;
      float max;
      int updatesPerSec;

      void Update(CpuLoadMeter* load) {
			avg = load->GetAvgCpuLoad();
			max = load->GetMaxCpuLoad();
      }

   private:
      const float ticksPerS   = float(System::GetTickFreq());

      
};

#endif // __KIWI_SYNTH_KUTILS_H__