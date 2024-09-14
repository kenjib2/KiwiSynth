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

      void Init(CpuLoadMeter* load) {
         this->load = load;
         
         avg = 0.0;
         max = 0.0;
         updateCount = 0;
         updatesPerSec = 0;

         ticksPerS = System::GetTickFreq();
         nextUpdateTick = System::GetTick() + ticksPerS;
         channelMultiplier = 1.0f / 16.0f;
      }

      void Update() {
         updateCount++;
         uint32_t tick = System::GetTick();
         if (tick > nextUpdateTick) {
            avg = load->GetAvgCpuLoad();
            max = load->GetMaxCpuLoad();
            updatesPerSec = updateCount * channelMultiplier;
            updateCount = 0;
            nextUpdateTick = tick + ticksPerS;
         }
      }

      inline float Avg() { return avg; }
      inline float Max() { return max; }
      inline int UpdatesPerSec() { return updatesPerSec; }

   private:
      float avg;
      float max;
      uint32_t updatesPerSec;
      uint32_t updateCount;
      uint32_t nextUpdateTick;
      uint32_t ticksPerS;
      float channelMultiplier; // Accounts for updates cycle through multiplexer channels, so each pot only gets updated once per n cycles.
      CpuLoadMeter* load;
      
};

#endif // __KIWI_SYNTH_KUTILS_H__