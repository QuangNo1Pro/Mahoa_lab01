// src/prime_utils.cpp

#include "modular_exponentiation.h" 
#include "prime_utils.h"
#include <iostream>
#include <random>       
#include <sstream>     


/**
 * @brief (Helper) Khởi tạo bộ sinh số ngẫu nhiên (PRNG) an toàn.
 */
std::mt19937_64& get_random_generator() {
    static std::random_device rd; // Lấy seed ngẫu nhiên từ phần cứng
    static std::mt19937_64 gen(rd()); // Khởi tạo Mersenne Twister 64-bit
    return gen;
}

/**
 * @brief Triển khai Hàm 1: Sinh số ngẫu nhiên lớn an toàn (ví dụ: 512 bit)
 */
BigInt generate_random_big_int(int bit_size) {
    // Tính số ký tự hex cần thiết
    int num_hex_chars = (bit_size + 3) / 4; 
    
    std::mt19937_64& gen = get_random_generator();
    std::uniform_int_distribution<int> hex_dis(0, 15); // Phân phối cho 0-F

    std::stringstream ss;
    
    // 1. Ký tự đầu tiên (Top bits): Phải > 8 (tức là bit cao nhất là 1)
    int first_val = hex_dis(gen);
    while (first_val < 8) {
        first_val = hex_dis(gen);
    }
    ss << std::hex << first_val;

    // 2. Các ký tự ở giữa
    for (int i = 0; i < num_hex_chars - 2; ++i) {
        ss << std::hex << hex_dis(gen);
    }

    // 3. Ký tự cuối cùng (Last bit): Phải là số lẻ (1, 3, 5, 7, 9, B, D, F)
    int last_val = hex_dis(gen);
    while (last_val % 2 == 0) {
        last_val = hex_dis(gen);
    }
    ss << std::hex << last_val;
    return BigInt(ss.str(), BigInt::HexTag{});
}

/**
 * @brief Triển khai Hàm 2: Sinh số ngẫu nhiên trong khoảng [min, max]
 */
BigInt generate_random_number_in_range(const BigInt& min, const BigInt& max) {
    // 1. Tính khoảng giá trị (range)
    BigInt range = max - min + BigInt(1);

    // 2. Lấy số bit của 'max' để sinh số ngẫu nhiên có cùng độ lớn
    std::string max_hex = max.toHexString();
    int bit_length = max_hex.length() * 4;

    // 3. Sinh số ngẫu nhiên R và lấy R % range
    BigInt rand_val = generate_random_big_int(bit_length);
    
    // 4. Trả về (R % range) + min
    return (rand_val % range) + min;
}


/**
 * @brief Hàm phụ trợ của Miller-Rabin: Kiểm tra n với một nhân chứng 'a'
 */
bool check_miller_rabin(const BigInt& n, const BigInt& a) {
    if (a <= 1 || a >= n - 1) return true; 

    BigInt d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d = d / 2;
        s++;
    }

    BigInt x = modular_exponentiation(a, d, n);

    if (x == 1 || x == n - 1) {
        return true;
    }

    for (int r = 0; r < s - 1; r++) {
        x = modular_exponentiation(x, 2, n); 
        if (x == n - 1) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Kiểm tra tính nguyên tố bằng thuật toán Miller-Rabin.
 */
bool is_prime(const BigInt& n, int k) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false; 

    for (int i = 0; i < k; i++) {
        BigInt a = generate_random_number_in_range(2, n - 2); 
        if (!check_miller_rabin(n, a)) {
            return false; 
        }
    }
    return true; 
}

/**
 * @brief Sinh số nguyên tố an toàn p = 2q + 1.
 */
BigInt generate_safe_prime(int bit_size) {
    std::cout << "-> Bat dau tim kiem so nguyen to an toan " << bit_size << " bit..." << std::endl;
    BigInt q, p;
    int q_bit_size = bit_size - 1; 

    do {
        q = generate_random_big_int(q_bit_size); 
        if (is_prime(q)) {
            std::cout << "   ... Da tim thay Q (Sophie Germain), kiem tra P..." << std::endl;
            p = q * 2 + 1; 
            if (is_prime(p)) {
                std::cout << "-> TIM THAY! P la so nguyen to an toan." << std::endl;
                return p;
            }
        }
    } while (true);
}