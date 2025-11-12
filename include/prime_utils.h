// include/prime_utils.h

#ifndef PRIME_UTILS_H
#define PRIME_UTILS_H
#include "constants.h" 

BigInt generate_random_number_in_range(const BigInt& min, const BigInt& max);
BigInt generate_random_big_int(int bit_size);

/**
 * @brief Kiểm tra tính nguyên tố bằng thuật toán Miller-Rabin.
 * @param n Số cần kiểm tra.
 */
bool is_prime(const BigInt& n, int k = 40);

/**
 * @brief Sinh số nguyên tố an toàn p = 2q + 1.
 */
BigInt generate_safe_prime(int bit_size);

#endif // PRIME_UTILS_H