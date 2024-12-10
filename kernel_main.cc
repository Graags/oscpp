#include "interrupt.h"
#include "utils.h"
#include "vga_terminal.h"

extern "C" {
void kernel_main(void);
}

void all_experiments();

void kernel_main(void) {
  terminal.Initialize();
  terminal << "Hello, kernel World!\n";
  interrupt::InitializeIDT();
  all_experiments();
}
