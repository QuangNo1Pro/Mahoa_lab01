#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "constants.h"
using namespace std;

static inline uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod) {
#if defined(__SIZEOF_INT128__)
  // Nhan 128-bit rồi lấy mod (nhanh, tránh tràn 64-bit)
  return (unsigned __int128)a * b % mod;
#else
  // Fallback: nhân bằng cộng-dồn (chậm hơn nhưng không tràn)
  uint64_t res = 0;
  a %= mod;
  while (b) {
    if (b & 1) res = (res + a) % mod;
    a = (a << 1) % mod;
    b >>= 1;
  }
  return res;
#endif
}

uint64_t modular_exponentiation_u64(uint64_t base, uint64_t exp, uint64_t mod) {
  if (mod == 0) throw std::invalid_argument("mod must be non-zero");
  base %= mod;
  uint64_t result = 1 % mod;
  while (exp) {
    if (exp & 1) result = mul_mod_u64(result, base, mod);
    base = mul_mod_u64(base, base, mod);
    exp >>= 1;
  }
  return result;
}

int main() {
  uint64_t result = modular_exponentiation_u64(3, 13, 17);
  cout << "Result: " << result << endl;
  cin.get();
  cout << "Press Enter to continue...";
  return 0;
}