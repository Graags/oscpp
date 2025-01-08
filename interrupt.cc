#include "interrupt.h"

#include <stdint.h>

#include "pic8259.h"
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

struct InterruptStackFrame {
  uint32_t ip;
  uint32_t cs;
  uint32_t flags;
} __attribute__((packed));

void DefaultISR() {
  terminal << "Unexpected interrupt!\n";
  asm("cli;hlt");
}

__attribute__((interrupt)) void TestRoutine(InterruptStackFrame *frame) {
  terminal << frame->ip << ' ' << frame->cs << ' ' << frame->flags << '\n';
  terminal << "in ISR\n";
}

__attribute__((interrupt)) void KeyboardISR(InterruptStackFrame *frame) {
  terminal << "Keyboard interrupt\n";
  uint8_t scancode = inb(0x60);
  terminal << "Scan code: " << (uint32_t)scancode << '\n';
  PIC::SendEOI(PIC::DeviceIRQ::Keyboard);
}

__attribute__((interrupt)) void TimerISR(InterruptStackFrame *frame) {
  static uint32_t tick = 0;
  tick++;
  if (tick % 100 == 0) {
    terminal << "Tick: " << tick << '\n';
  }
  PIC::SendEOI(PIC::DeviceIRQ::Timer);
}

void InitializeIDT() {
  uint32_t reg_cs_value = GetCodeSegmentID();
  for (int i = 0; i < kNumInterrupt; ++i) {
    descriptor_table[i].SetISR((uint32_t)DefaultISR, reg_cs_value);
  }
  descriptor_table[0x81].SetISR((uint32_t)TestRoutine, reg_cs_value);
  descriptor_table[PIC::IntNbOffset + PIC::DeviceIRQ::Keyboard].SetISR((uint32_t)KeyboardISR,
                                                                       reg_cs_value);
  descriptor_table[PIC::IntNbOffset + PIC::DeviceIRQ::Timer].SetISR((uint32_t)TimerISR,
                                                                    reg_cs_value);
  asm("lidt %0" ::"m"(idt_info));
}

}  // namespace interrupt
