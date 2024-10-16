#ifndef __KIWI_SYNTH_KUTILS_H__
#define __KIWI_SYNTH_KUTILS_H__

// Compiler options
//#define __CPU_LOAD__ // Monitor performance in GUI
#define EXTRA_PRECISION // Higher precision fastCos and fastSin

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;

inline float fastCos(float x)
{
    constexpr float tp = 1.f/(2.f*M_PI);
    x *= tp;
    x -= .25f + std::floor(x + .25f);
    x *= 16.f * (std::fabs(x) - .5f);
    #ifdef EXTRA_PRECISION
    x += .225f * x * (std::fabs(x) - 1.f);
    #endif
    return x;
}

inline float fastSine(float x)
{
    const float B = 4/M_PI;
    const float C = -4/(M_PI*M_PI);

    float y = B * x + C * x * std::fabs(x);

    #ifdef EXTRA_PRECISION
    //  const float Q = 0.775;
        const float P = 0.225;

        y = P * (y * abs(y) - y) + y;   // Q * y + P * y * abs(y)
    #endif

    return y;
}

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
         readCount = 0;
         readsPerSec = 0;

         ticksPerS = System::GetTickFreq();
         nextReadTick = System::GetTick() + ticksPerS;
         channelMultiplier = 1.0f / 16.0f;
      }

      void Update() {
         readCount++;
         uint32_t tick = System::GetTick();
         if (tick > nextReadTick) {
            avg = load->GetAvgCpuLoad();
            max = load->GetMaxCpuLoad();
            readsPerSec = readCount * channelMultiplier;
            readCount = 0;
            nextReadTick = tick + ticksPerS;
         }
      }

      inline float Avg() { return avg; }
      inline float Max() { return max; }
      inline int ReadsPerSec() { return readsPerSec; }

   private:
      float avg;
      float max;
      uint32_t readsPerSec;
      uint32_t readCount;
      uint32_t nextReadTick;
      uint32_t ticksPerS;
      float channelMultiplier; // Accounts for updates cycle through multiplexer channels, so each pot only gets updated once per n cycles.
      CpuLoadMeter* load;
      
};

#endif // __KIWI_SYNTH_KUTILS_H__