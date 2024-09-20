#include "KiwiDelayLine.h"

namespace kiwi_synth
{

float DSY_SDRAM_BSS bufferMemory[6][2400];

void KiwiDelayLine::Init(int bufferNumber, size_t maxSize) { max_size = maxSize; line_ = bufferMemory[bufferNumber]; Reset(); }

} // namespace kiwi_synth
