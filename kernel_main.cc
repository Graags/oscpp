#include <stddef.h>
#include <stdint.h>

extern "C" {
  void kernel_main(void);
}

enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

const uint8_t VGA_WIDTH = 80;
const uint8_t VGA_HEIGHT = 25;

uint8_t terminal_row;
uint8_t terminal_column;

uint8_t terminal_color;

uint16_t* const terminal_buffer = (uint16_t*)0xB8000;

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) { return fg | bg << 4; }
uint16_t vga_entry(unsigned char uc, uint8_t color) { return (uint16_t)uc | (uint16_t)color << 8; }

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
    terminal_buffer[i] = vga_entry(' ', terminal_color);
  }
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
      terminal_row = 0;
    }
  }
}

void terminal_writestring(const char* data) {
  size_t i = 0;
  while (data[i]) {
    terminal_putchar(data[i]);
    i++;
  }
}

void kernel_main(void) {
  terminal_initialize();
  terminal_writestring("Hello, kernel World!");
}
