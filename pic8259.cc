#include "pic8259.h"

#include "utils.h"

namespace interrupt::PIC {

namespace Port {
constexpr uint16_t PIC0Command = 0x20;
constexpr uint16_t PIC0Data = 0x21;
constexpr uint16_t PIC1Command = 0xA0;
constexpr uint16_t PIC1Data = 0xA1;

};  // namespace Port

namespace Command {

constexpr uint8_t ICW1Init = 0x10;
constexpr uint8_t ICW1ICW4 = 0x01;
constexpr uint8_t ICW48086 = 0x01;
constexpr uint8_t EOI = 0x20;

};  // namespace Command

constexpr uint8_t PIC1PositionInPIC0 = 0b00000100;
constexpr uint8_t PIC1CascadeIdentity = 0x02;

void PICRemap(int offset_0, int offset_1) {
  uint8_t mask0 = inb(Port::PIC0Data);
  uint8_t mask1 = inb(Port::PIC1Data);
  outb(Port::PIC0Command, Command::ICW1Init | Command::ICW1ICW4);
  io_wait();
  outb(Port::PIC1Command, Command::ICW1Init | Command::ICW1ICW4);
  io_wait();
  outb(Port::PIC0Data, offset_0);
  io_wait();
  outb(Port::PIC1Data, offset_1);
  io_wait();
  outb(Port::PIC0Data, PIC1PositionInPIC0);
  io_wait();
  outb(Port::PIC1Data, PIC1CascadeIdentity);
  io_wait();
  outb(Port::PIC0Data, Command::ICW48086);
  io_wait();
  outb(Port::PIC1Data, Command::ICW48086);
  io_wait();
  outb(Port::PIC0Data, mask0);
  outb(Port::PIC1Data, mask1);
}

void SetMask(uint8_t irq) {
  uint16_t port;
  uint8_t mask;
  if (irq < 8) {
    port = Port::PIC0Data;
  } else {
    port = Port::PIC1Data;
    irq -= 8;
  }
  mask = inb(port) | (1 << irq);
  outb(port, mask);
}
void ClearMask(uint8_t irq) {
  uint16_t port;
  uint8_t mask;
  if (irq < 8) {
    port = Port::PIC0Data;
  } else {
    port = Port::PIC1Data;
    irq -= 8;
  }
  mask = inb(port) & ~(1 << irq);
  outb(port, mask);
}

void DisableAll() {
  outb(Port::PIC0Data, 0xFF);
  outb(Port::PIC1Data, 0xFF);
}

void SendEOI(uint8_t irq) {
  if (irq >= 8) {
    outb(Port::PIC1Command, Command::EOI);
  }
  outb(Port::PIC0Command, Command::EOI);
}

void Init() {
  PICRemap(IntNoOffset, IntNoOffset + 8);
  DisableAll();
  ClearMask(DeviceIRQ::Keyboard);
  asm("sti");
}
}  // namespace interrupt::PIC
