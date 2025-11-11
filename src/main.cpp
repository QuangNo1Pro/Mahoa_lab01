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

void test_bigint() {
    cout << "--- Bat dau kiem tra BigInt ---" << endl << endl;

    // Test 1: Khoi tao
    cout << "1. Kiem tra khoi tao:" << endl;
    BigInt a("12345678901234567890");
    BigInt b("DEADBEEF", BigInt::HexTag{});
    cout << "   a (dec): " << a.toDecimalString() << endl;
    cout << "   a (hex): " << a.toHexString() << endl;
    cout << "   Mong doi a (hex): ab54a98ceb1f0ad2" << endl; // Corrected expected value
    cout << "   b (hex): " << b.toHexString() << endl;
    cout << "   b (dec): " << b.toDecimalString() << endl;
    cout << "   Mong doi b (dec): 3735928559" << endl << endl;

    // Test 2: Phep cong
    cout << "2. Kiem tra phep cong:" << endl;
    BigInt c("87654321098765432110");
    BigInt sum = a + c;
    cout << "   " << a.toDecimalString() << " + " << c.toDecimalString() << " = " << sum.toDecimalString() << endl;
    cout << "   Mong doi: 100000000000000000000" << endl << endl;

    // Test 3: Phep tru
    cout << "3. Kiem tra phep tru:" << endl;
    BigInt d("10000");
    BigInt e("3456");
    BigInt diff = d - e;
    cout << "   10000 - 3456 = " << diff.toDecimalString() << endl;
    cout << "   Mong doi: 6544" << endl << endl;

    // Test 4: Phep nhan (don gian)
    cout << "4. Kiem tra phep nhan (don gian):" << endl;
    BigInt p1("4294967295");
    BigInt p2("4294967295");
    BigInt prod = p1 * p2;
    cout << "   4294967295 * 4294967295 = " << prod.toDecimalString() << endl;
    cout << "   Mong doi: 18446744065119617025" << endl;
    cout << "   Ket qua (hex): " << prod.toHexString() << endl;
    cout << "   Mong doi (hex): fffffffe00000001" << endl << endl;

    // Test 5: Phep chia va chia lay du (don gian)
    cout << "5. Kiem tra phep chia va chia lay du (don gian):" << endl;
    BigInt dividend("18446744065119617025");
    BigInt divisor("4294967295");
    BigInt quotient = dividend / divisor;
    BigInt remainder = dividend % divisor;
    cout << "   " << dividend.toDecimalString() << " / " << divisor.toDecimalString() << endl;
    cout << "   Thuong: " << quotient.toDecimalString() << " (Mong doi: 4294967295)" << endl;
    cout << "   Du: " << remainder.toDecimalString() << " (Mong doi: 0)" << endl << endl;

    // Test 6: So sanh
    cout << "6. Kiem tra so sanh:" << endl;
    BigInt s1("100");
    BigInt s2("200");
    BigInt s3("100");
    cout << "   100 < 200: " << (s1 < s2 ? "Dung" : "Sai") << " (Mong doi: Dung)" << endl;
    cout << "   200 > 100: " << (s2 > s1 ? "Dung" : "Sai") << " (Mong doi: Dung)" << endl;
    cout << "   100 == 100: " << (s1 == s3 ? "Dung" : "Sai") << " (Mong doi: Dung)" << endl;
    cout << "   100 != 200: " << (s1 != s2 ? "Dung" : "Sai") << " (Mong doi: Dung)" << endl;
    cout << "   100 <= 100: " << (s1 <= s3 ? "Dung" : "Sai") << " (Mong doi: Dung)" << endl;
    cout << "   200 >= 100: " << (s2 >= s1 ? "Dung" : "Sai") << " (Mong doi: Dung)" << endl << endl;

    cout << "--- Ket thuc kiem tra BigInt ---" << endl;
}

int main() {
  try {
    test_bigint();
  } catch (const std::exception& e) {
    cerr << "An exception occurred: " << e.what() << endl;
    return 1;
  } catch (...) {
    cerr << "An unknown exception occurred." << endl;
    return 1;
  }

  cout << endl << "Nhan Enter de tiep tuc...";
  cin.get();
  return 0;
}
