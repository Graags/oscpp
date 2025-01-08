#include <stdint.h>

#include "utils.h"

void InitializePIT() {
  constexpr uint32_t kPITMaxFreq = 1193182;
  constexpr uint16_t kPITCommandPort = 0x43;
  constexpr uint16_t kPITDataPort = 0x40;

  // Set the PIT frequency to 100 Hz
  const uint32_t divisor = kPITMaxFreq / 100;
  outb(kPITCommandPort, 0b00110100);  // See https://wiki.osdev.org/Programmable_Interval_Timer
  outb(kPITDataPort, divisor & 0xFF);
  outb(kPITDataPort, (divisor >> 8) & 0xFF);
}
