#include "generate_private_key.h"

#include <stdexcept>

#include "prime_utils.h"  // cần prototype generate_random_number_in_range

// Sinh khóa riêng trong [2, p-2]
BigInt generate_private_key(const BigInt& p) {
  BigInt two(2);
  BigInt three(3);

  if (p <= three) {
    throw std::invalid_argument(
        "Modulus p must be >= 4 to generate a private key in [2, p-2].");
  }

  BigInt min = two;
  BigInt max = p - two;  // p-2

  // generate_random_number_in_range phải được khai báo trong prime_utils.h và
  // định nghĩa trong prime_utils.cpp
  BigInt priv = generate_random_number_in_range(min, max);

  // defensive loop (thường không cần)
  while (priv < min || priv > max) {
    priv = generate_random_number_in_range(min, max);
  }
  return priv;
}
