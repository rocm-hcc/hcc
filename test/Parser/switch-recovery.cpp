// RUN: %clang_cc1 -fsyntax-only -verify %s

// <rdar://problem/7971948>
struct A {};
struct B {
  void foo(int b) {
    switch (a) { // expected-error{{use of undeclared identifier 'a'}}
    default:
      return;
    }
    
    switch (b) {
    case 17 // expected-error{{expected ':' after 'case'}}
      break;

    default // expected-error{{expected ':' after 'default'}}
      return;
    }
  }

  void test2() {
    enum X { Xa, Xb } x;

    switch (x) { // expected-warning {{enumeration value 'Xb' not handled in switch}}
    case Xa; // expected-error {{expected ':' after 'case'}}
      break;
    }

    switch (x) {
    default; // expected-error {{expected ':' after 'default'}}
      break;
    }
  }

  int test3(int i) {
    switch (i) {
      case 1: return 0;
      2: return 1;  // expected-error {{expected 'case' keyword before expression}}
      default: return 5;
    }
  }
};

int test4(int i) {
  switch (i)
    1: return -1;  // expected-error {{expected 'case' keyword before expression}}
  return 0;
}

int test5(int i) {
  switch (i) {
    case 1: case 2: case 3: return 1;
    {
    4:5:6:7: return 2;  // expected-error 4{{expected 'case' keyword before expression}}
    }
    default: return -1;
  }
}

int test6(int i) {
  switch (i) {
    case 1:
    case 4:
      // This class provides extra single colon tokens.  Make sure no
      // errors are seen here.
      class foo{
        public:
        protected:
        private:
      };
    case 2:
    5:  // expected-error {{expected 'case' keyword before expression}}
    default: return 1;
  }
}

int test7(int i) {
  switch (i) {
    case false ? 1 : 2:
    true ? 1 : 2:  // expected-error {{expected 'case' keyword before expression}}
    case 10:
      14 ? 3 : 4;
    default:
      return 1;
  }
}

enum foo { A, B, C};
int test8( foo x ) {
  switch (x) {
    A: return 0;  // FIXME: give a warning for unused labels that could also be
                  // a case expression.
    default: return 1;
  }
}

// Stress test to make sure Clang doesn't crash.
void test9(int x) {
  switch(x) {
    case 1: return;
    2: case; // expected-error {{expected 'case' keyword before expression}} \
                expected-error {{expected expression}}
    4:5:6: return; // expected-error 3{{expected 'case' keyword before expression}}
    7: :x; // expected-error {{expected 'case' keyword before expression}} \
              expected-error {{expected expression}}
    8:: x; // expected-error {{expected ';' after expression}} \
              expected-error {{no member named 'x' in the global namespace}} \
              expected-warning {{expression result unused}}
    9:: :y; // expected-error {{expected ';' after expression}} \
               expected-error {{expected unqualified-id}} \
               expected-warning {{expression result unused}}
    :; // expected-error {{expected expression}}
    ::; // expected-error {{expected unqualified-id}}
  }
}

void test10(int x) {
  switch (x) {
    case 1: {
      struct Inner {
        void g(int y) {
          2: y++;  // expected-error {{expected ';' after expression}} \
                   // expected-warning {{expression result unused}}
        }
      };
      break;
    }
  }
}

template<typename T>
struct test11 {
  enum { E };

  void f(int x) {
    switch (x) {
      E: break;    // FIXME: give a 'case' fix-it for unused labels that
                   // could also be an expression an a case label.
      E+1: break;  // expected-error {{expected 'case' keyword before expression}}
    }
  }
};

void test12(int x) {
  switch (x) {
    0:  // expected-error {{expected 'case' keyword before expression}}
    while (x) {
      1:  // expected-error {{expected 'case' keyword before expression}}
      for (;x;) {
        2:  // expected-error {{expected 'case' keyword before expression}}
        if (x > 0) {
          3:  // expected-error {{expected 'case' keyword before expression}}
          --x;
        }
      }
    }
  }
}
