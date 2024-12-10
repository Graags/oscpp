#include "utils.h"
#include "vga_terminal.h"

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

void get_reg_cs_value() { terminal << "CS = " << GetCodeSegmentID() << '\n'; }

void trigger_test_interrupt() {
  asm("int $0x81");
  asm("int $0x81");
}

void all_experiments() {
  dtor_test(0);
  get_reg_cs_value();
  trigger_test_interrupt();
}
