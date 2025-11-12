#ifndef GENERATE_PRIVATE_KEY_H
#define GENERATE_PRIVATE_KEY_H

// đảm bảo BigInt được định nghĩa
#include <stdexcept>

#include "constants.h"    // hoặc tên header BigInt thực tế của bạn
#include "prime_utils.h"  // để chắc chắn compile nếu muốn (không bắt buộc)

BigInt generate_private_key(const BigInt& p);

#endif  // GENERATE_PRIVATE_KEY_H
