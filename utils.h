#pragma once

#include <stdint.h>

inline uint32_t GetCodeSegmentID() {
  uint32_t cs;
  asm("movl %%cs, %0" : "=r"(cs));
  return cs;
}

inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

inline void outb(uint16_t port, uint8_t data) {
  asm volatile("outb %b0, %w1" : : "a"(data), "Nd"(port) : "memory");
}

inline void io_wait(void) {
  constexpr uint16_t kUnusedPort = 0x80;
  outb(kUnusedPort, 0);
}
