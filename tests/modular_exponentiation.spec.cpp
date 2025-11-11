#include "modular_exponentiation.h"

#include <iostream>
#include <stdexcept>

#include "constants.h"

static void expect(const char* name, const BigInt& got, const BigInt& want) {
  if (!(got == want)) {
    std::cerr << "[FAIL] " << name << " got=" << got.toDecimalString()
              << " want=" << want.toDecimalString() << "\n";
    std::exit(1);
  } else {
    std::cout << "[OK]   " << name << "\n";
  }
}

int main() {
  expect("3^13 mod 17",
         modular_exponentiation(BigInt(3), BigInt(13), BigInt(17)), BigInt(13));
  expect("base>mod", modular_exponentiation(BigInt(100), BigInt(3), BigInt(1)),
         BigInt(1));
  expect("exp=0", modular_exponentiation(BigInt(12345), BigInt(0), BigInt(2)),
         BigInt(1 % 97));
  expect("mod=1", modular_exponentiation(BigInt(999), BigInt(999), 1),
         BigInt(0));
  std::cout << "All modular_exponentiation tests passed.\n";
  return 0;
}
