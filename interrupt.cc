#include "interrupt.h"

#include <stdint.h>

#include "utils.h"
#include "vga_terminal.h"

namespace interrupt {

constexpr int kNumInterrupt = 256;
struct Descriptor {
  uint16_t isr_low;  // The lower 16 bits of the ISR's address
  // The GDT segment selector that the CPU will load into CS before calling the ISR
  uint16_t kernel_cs;
  uint8_t reserved;    // Set to zero
  uint8_t attributes;  // Type and attributes
  uint16_t isr_high;   // The higher 16 bits of the ISR's address
  void SetISR(uint32_t routine, uint16_t reg_cs) {
    kernel_cs = reg_cs;
    reserved = 0;
    isr_low = routine & 0xFFFF;
    isr_high = routine >> 16;
    attributes = 0x8Eu;  // rint 0 32bit gate
  }
} __attribute__((packed));

struct IDTInfo {
  uint16_t size;
  uintptr_t offset;
} __attribute__((packed));

__attribute__((aligned(16))) Descriptor descriptor_table[kNumInterrupt];

__attribute__((aligned(16))) IDTInfo idt_info{.size = sizeof(descriptor_table) - 1,
                                              .offset = (uintptr_t)descriptor_table};

__attribute__((noreturn)) void DefaultISR() { asm("cli;hlt"); }

__attribute__((noreturn)) void TestRoutine() {
  terminal << "in ISR\n";
  asm("cli;hlt");
}

void InitializeIDT() {
  uint32_t reg_cs_value = GetCodeSegmentID();
  for (int i = 0; i < kNumInterrupt; ++i) {
    descriptor_table[i].SetISR((uint32_t)DefaultISR, reg_cs_value);
  }
  descriptor_table[0x80].SetISR((uint32_t)TestRoutine, reg_cs_value);
  asm("lidt %0" ::"m"(idt_info));
}

}  // namespace interrupt
