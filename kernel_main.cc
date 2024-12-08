#include "vga_terminal.h"

extern "C" {
void kernel_main(void);
}

void kernel_main(void) {
  terminal.Initialize();
  terminal.Write("Hello, kernel World!\n");
  terminal.Write("Hello, kernel World!\n");
}
