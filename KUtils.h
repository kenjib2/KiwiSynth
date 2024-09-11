#ifndef __KIWI_SYNTH_KUTILS_H__
#define __KIWI_SYNTH_KUTILS_H__

// Compiler options
//#define __CPU_LOAD__ // Output performance to serial console
//#define __PATCH_SETTINGS__ // Output patch settings to serial console
//#define __FUNCTIONALITY_OPTION__ // Creates an alternate voice mode with reduced functionality but 3 voices. Impacts performance of one and two voice mode and causes audible pops though.

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

#endif // __KIWI_SYNTH_KUTILS_H__