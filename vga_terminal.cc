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
VGATerminal terminal;
