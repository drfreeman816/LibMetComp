#include "MetComp.h"

int main() {

  // Atomic number
  const size_t Z = 1;

  supp::printfln("Atom Z = $", Z);

  complex x(5, 2), y(3, 4), z = log(sin(x / y - Iu * math::pi));

  supp::printfln("z = $", z.tostr());

  return 0;
}
