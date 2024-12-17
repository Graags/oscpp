#pragma once

#include <stdint.h>

namespace interrupt::PIC {

constexpr int IntNoOffset = 0x20;

namespace DeviceIRQ {

constexpr uint8_t Timer = 0;
constexpr uint8_t Keyboard = 1;

}  // namespace DeviceIRQ

void Init();
void SendEOI(uint8_t irq);

}  // namespace interrupts::PIC
