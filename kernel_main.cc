#include "vga_terminal.h"

extern "C" {
void kernel_main(void);
}

class C {
 public:
  C(char c) : c_(c) {}
  ~C() {
    terminal.Write("dtor of ");
    terminal.PutChar(c_);
    terminal.PutChar('\n');
  }

 private:
  char c_;
};

void dtor_test(int depth) {
  C c('0' + depth);
  if (depth == 5) {
    return;
  }
  dtor_test(depth + 1);
}

void kernel_main(void) {
  terminal.Initialize();
  terminal.Write("Hello, kernel World!\n");
  terminal.Write("Hello, kernel World!\n");
  dtor_test(0);
}
