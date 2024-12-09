#pragma once

#include <stdint.h>

inline uint32_t GetCodeSegmentID() {
  uint32_t cs;
  asm("movl %%cs, %0" : "=r"(cs));
  return cs;
}
