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
  if (depth == 3) {
    return;
  }
  dtor_test(depth + 1);
}

inline uint32_t GetCodeSegmentID() {
  uint32_t cs;
  asm("movl %%cs, %0" : "=r" (cs));
  return cs;
}

void kernel_main(void) {
  terminal.Initialize();
  terminal << "Hello, kernel World!\n";
  terminal << "CS = " << GetCodeSegmentID() << '\n';
  dtor_test(0);
}
