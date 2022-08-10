#include <string>

class B;

class A
{
public:
  A() {}
  static inline B* universalB = new B();
};


class B : public A
{
public:
  B() : value("initial value") { }
  std::string value;
};

int main() {
  B b;
  A a1;
  A a2;

  printf("\na1 B.value: %s", a1.universalB->value.c_str());
  printf("\na2 B.value: %s", a2.universalB->value.c_str());

  a1.universalB->value = "updated value";

  printf("\na1 B.value: %s", a1.universalB->value.c_str());
  printf("\na2 B.value: %s", a2.universalB->value.c_str());
}