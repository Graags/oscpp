#pragma once

#include <stddef.h>
#include <stdint.h>

class VGATerminal {
 public:
  VGATerminal() = default;
  void Initialize();
  void Write(const char* data) {
    for (size_t i = 0; data[i]; ++i) {
      PutChar(data[i]);
    }
  }

 private:
  uint8_t row_;
  uint8_t column_;
  uint8_t colour_;

  enum class Colour : uint8_t {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
  };

  static uint16_t* const VGABuffer;
  static constexpr uint8_t kWidth = 80;
  static constexpr uint8_t kHeight = 25;
  static uint8_t EntryColour(Colour fg, Colour bg) { return (uint8_t)fg | (uint8_t)bg << 4; }
  static uint16_t Entry(unsigned char uc, uint8_t colour) {
    return (uint16_t)uc | (uint16_t)colour << 8;
  }
  void PutEntryAt(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * kWidth + x;
    VGABuffer[index] = Entry(c, color);
  }
  void PutChar(char c);
  void NewLine() {
    column_ = 0;
    if ((++row_) == kHeight) {
      row_ = 0;
    }
  }
};

extern VGATerminal terminal;
