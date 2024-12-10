#include <typeinfo>

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

class Base {
 public:
  virtual void foo() const {
    terminal << "func in Base\n";
  }
};

class Derived : public Base {
  public:
  void foo() const override {
    terminal << "func in Derived\n";
   } 
};

void inherit_test() {
  Base b;
  Base* p_b = &b;
  Derived d;
  Base* p_d = &d;
  p_b->foo();
  p_d->foo();
  Derived *p_d_2 = static_cast<Derived*>(p_d);
  p_d_2->foo();
  terminal << typeid(*p_b).name() << '\n';
}

void all_experiments() {
  dtor_test(0);
  get_reg_cs_value();
  trigger_test_interrupt();
  inherit_test();
}
