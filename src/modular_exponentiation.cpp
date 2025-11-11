#include "modular_exponentiation.h"

#include <stdexcept>

// (base^exponent) mod mod  — binary exponentiation
BigInt modular_exponentiation(BigInt base, BigInt exponent, const BigInt& mod) {
  BigInt zero(0), one(1), two(2);

  if (mod == zero) throw std::invalid_argument("mod must be non-zero");

  // Chuẩn hoá cơ số vào [0, mod)
  base = base % mod;

  // exp = 0 => trả về 1 % mod
  if (exponent == zero) return one % mod;

  BigInt result = one % mod;  // xử lý cả trường hợp mod = 1

  // Duyệt bit của exponent bằng dịch phải
  while (exponent > zero) {
    // Nếu bit thấp nhất của exponent = 1
    if ((exponent % two) == one) {
      result = (result * base) % mod;
    }
    // Bình phương cơ số
    base = (base * base) % mod;

    // Dịch phải 1 bit
    exponent = exponent >> 1;
  }
  return result;  // 0 <= result < mod
}