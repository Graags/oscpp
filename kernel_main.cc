#include "interrupt.h"
#include "utils.h"
#include "vga_terminal.h"
#include "pic8259.h"

extern "C" {
void kernel_main(void);
}

void all_experiments();

void kernel_main(void) {
  terminal.Initialize();
  terminal << "Hello, kernel World!\n";
  interrupt::InitializeIDT();
  interrupt::PIC::Init();
  all_experiments();
  terminal << "Reach end of kernel_main\n";
  for (;;) {
    asm("hlt");
  }
}
