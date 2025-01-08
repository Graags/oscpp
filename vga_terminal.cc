#include "vga_terminal.h"

uint16_t* const VGATerminal::VGABuffer = (uint16_t*)0xB8000;

void VGATerminal::Initialize() {
  row_ = 0;
  column_ = 0;
  colour_ = EntryColour(Colour::LIGHT_GREY, Colour::BLACK);
  for (size_t i = 0; i < kWidth * kHeight; ++i) {
    VGABuffer[i] = Entry(' ', colour_);
  }
}

void VGATerminal::PutChar(char c) {
  if (c == '\n') {
    NewLine();
    return;
  }
  PutEntryAt(c, colour_, column_, row_);
  if ((++column_) == kWidth) {
    NewLine();
  }
}

void VGATerminal::WriteHex(uint32_t data) {
  Write("0x");
  for (int i = 7; i >= 0; --i) {
    uint32_t digit = (data & (15u << (i * 4))) >> (i * 4);
    char c;
    if (digit < 10) {
      c = '0' + (uint8_t)digit;
    } else {
      c = 'A' + (uint8_t)digit - 10;
    }
    PutChar(c);
  }
}

void VGATerminal::ScrollUpOneLine() {
  for (size_t y = 1; y < kHeight; ++y) {
    for (size_t x = 0; x < kWidth; ++x) {
      VGABuffer[(y - 1) * kWidth + x] = VGABuffer[y * kWidth + x];
    }
  }
  for (size_t x = 0; x < kWidth; ++x) {
    VGABuffer[(kHeight - 1) * kWidth + x] = Entry(' ', colour_);
  }
}

VGATerminal terminal;
