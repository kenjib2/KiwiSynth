#include "KiwiDelayLine.h"

namespace kiwisynth
{

float DSY_SDRAM_BSS bufferMemory[6][DELAY_LINE_BUFFER_SIZE];

void KiwiDelayLine::Init(int bufferNumber) { size_mask = DELAY_LINE_BUFFER_SIZE - 1; line_ = bufferMemory[bufferNumber]; Reset(); }

} // namespace kiwisynth

