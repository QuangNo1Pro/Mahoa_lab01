#ifndef CONSTANTS_H
#define CONSTANTS_H

// --- Đã sắp xếp lại và đảm bảo đủ header ---
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>  // Cần cho std::move
#include <vector>

const int BIT_SIZE = 512;
const long long GENERATOR = 2;

// Đảm bảo chỉ dùng C++14

class BigInt {
 private:
  // Lưu các chunk, base 2^32
  // m_chunks[0] là 32 bit thấp nhất (little-endian)
  std::vector<uint32_t> m_chunks;

  // (Có thể thêm biến bool m_is_negative để xử lý số âm)

  // Helper to remove leading zero chunks
  void trim() {
    while (m_chunks.size() > 1 && m_chunks.back() == 0) {
      m_chunks.pop_back();
    }
  }

 public:
  // --- Constructors ---
  // Constructor mặc định (tạo số 0)
  BigInt() { m_chunks.push_back(0); }

  // Constructor từ số long long
  BigInt(long long value) {
    if (value < 0) {
      throw std::invalid_argument("Negative values are not supported yet.");
    }
    if (value == 0) {
      m_chunks.push_back(0);
      return;
    }
    unsigned long long val = value;
    while (val > 0) {
      m_chunks.push_back((uint32_t)(val & 0xFFFFFFFF));
      val >>= 32;
    }
  }

  // Thẻ để nhận dạng constructor cho chuỗi hex
  struct HexTag {};

  // Constructor từ chuỗi hex (dùng tag để phân biệt)
  BigInt(const std::string& hex_str, HexTag) {
    m_chunks.clear();
    std::string s = hex_str;

    if (s.rfind("0x", 0) == 0 || s.rfind("0X", 0) == 0) {
      s = s.substr(2);
    }

    if (s.empty() || s == "0") {
      m_chunks.push_back(0);
      return;
    }

    for (int i = s.length(); i > 0; i -= 8) {
      int len = std::max(0, i - 8);
      std::string chunk_str = s.substr(len, i - len);
      m_chunks.push_back(std::stoul(chunk_str, nullptr, 16));
    }

    trim();
  }

  // Constructor từ chuỗi thập phân
  BigInt(const std::string& decimal_str) {
    // Tạo một đối tượng tạm thời để tính toán
    BigInt result;
    if (decimal_str.empty()) {
      m_chunks.push_back(0);
      return;
    }

    BigInt ten(10);
    for (char c : decimal_str) {
      if (c < '0' || c > '9') {
        throw std::invalid_argument("Invalid character in decimal string.");
      }
      result = (result * ten) + BigInt(c - '0');
    }

    // Sau khi tính toán xong, di chuyển kết quả vào đối tượng hiện tại
    this->m_chunks = std::move(result.m_chunks);
  }

  // Sử dụng các hàm mặc định do trình biên dịch tạo ra
  BigInt(const BigInt&) = default;
  BigInt& operator=(const BigInt&) = default;
  BigInt(BigInt&&) noexcept = default;
  BigInt& operator=(BigInt&&) noexcept = default;

  // --- Phép toán số học ---
  BigInt operator+(const BigInt& other) const {
    BigInt result;
    result.m_chunks.clear();

    uint64_t carry = 0;
    size_t n = std::max(m_chunks.size(), other.m_chunks.size());

    for (size_t i = 0; i < n; ++i) {
      uint64_t current_a = (i < m_chunks.size()) ? m_chunks[i] : 0;
      uint64_t current_b = (i < other.m_chunks.size()) ? other.m_chunks[i] : 0;
      uint64_t sum = current_a + current_b + carry;
      result.m_chunks.push_back((uint32_t)(sum & 0xFFFFFFFF));
      carry = sum >> 32;
    }

    if (carry > 0) {
      result.m_chunks.push_back(carry);
    }

    result.trim();
    return result;
  }

  BigInt operator-(const BigInt& other) const {
    if (*this < other) {
      throw std::invalid_argument(
          "Result would be negative, which is not supported yet.");
    }

    BigInt result;
    result.m_chunks.clear();

    long long borrow = 0;
    for (size_t i = 0; i < m_chunks.size(); ++i) {
      long long chunk_a = m_chunks[i];
      long long chunk_b = (i < other.m_chunks.size()) ? other.m_chunks[i] : 0;

      long long diff = chunk_a - chunk_b - borrow;

      if (diff < 0) {
        diff += 0x100000000LL;  // 2^32
        borrow = 1;
      } else {
        borrow = 0;
      }
      result.m_chunks.push_back((uint32_t)diff);
    }

    result.trim();
    return result;
  }

  BigInt operator*(const BigInt& other) const {
    BigInt zero_val(0);
    if (*this == zero_val || other == zero_val) {
      return zero_val;
    }

    BigInt result;
    result.m_chunks.assign(m_chunks.size() + other.m_chunks.size(), 0);

    for (size_t i = 0; i < m_chunks.size(); ++i) {
      uint64_t carry = 0;
      for (size_t j = 0; j < other.m_chunks.size(); ++j) {
        uint64_t product = (uint64_t)m_chunks[i] * other.m_chunks[j] +
                           result.m_chunks[i + j] + carry;
        result.m_chunks[i + j] = (uint32_t)(product & 0xFFFFFFFF);
        carry = product >> 32;
      }
      if (carry > 0) {
        result.m_chunks[i + other.m_chunks.size()] += carry;
      }
    }

    result.trim();
    return result;
  }

  BigInt operator/(const BigInt& other) const {
    BigInt quotient, remainder;
    divmod(other, quotient, remainder);
    return quotient;
  }

  BigInt operator%(const BigInt& other) const {
    BigInt quotient, remainder;
    divmod(other, quotient, remainder);
    return remainder;
  }

  // --- Phép toán so sánh ---
  bool operator==(const BigInt& other) const {
    if (m_chunks.size() != other.m_chunks.size()) {
      return false;
    }
    for (size_t i = 0; i < m_chunks.size(); ++i) {
      if (m_chunks[i] != other.m_chunks[i]) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const BigInt& other) const { return !(*this == other); }

  bool operator<(const BigInt& other) const {
    if (m_chunks.size() != other.m_chunks.size()) {
      return m_chunks.size() < other.m_chunks.size();
    }
    for (int i = m_chunks.size() - 1; i >= 0; --i) {
      if (m_chunks[i] != other.m_chunks[i]) {
        return m_chunks[i] < other.m_chunks[i];
      }
    }
    return false;  // Equal
  }

  bool operator>(const BigInt& other) const { return other < *this; }

  bool operator<=(const BigInt& other) const { return !(*this > other); }

  bool operator>=(const BigInt& other) const { return !(*this < other); }

  // --- Phép toán bitwise (nếu cần) ---
  BigInt operator<<(unsigned int bits) const {
    if (bits == 0) return *this;

    const unsigned int chunk_shift = bits / 32;
    const unsigned int bit_shift = bits % 32;

    BigInt result;
    result.m_chunks.resize(
        m_chunks.size() + chunk_shift + (bit_shift > 0 ? 1 : 0), 0);

    if (bit_shift == 0) {
      for (size_t i = 0; i < m_chunks.size(); ++i) {
        result.m_chunks[i + chunk_shift] = m_chunks[i];
      }
    } else {
      uint32_t carry = 0;
      for (size_t i = 0; i < m_chunks.size(); ++i) {
        uint64_t val = ((uint64_t)m_chunks[i] << bit_shift) | carry;
        result.m_chunks[i + chunk_shift] = (uint32_t)val;
        carry = val >> 32;
      }
      if (carry > 0) {
        result.m_chunks.back() = carry;
      }
    }

    result.trim();
    return result;
  }

  BigInt operator>>(unsigned int bits) const {
    if (bits == 0) return *this;

    const unsigned int chunk_shift = bits / 32;
    const unsigned int bit_shift = bits % 32;

    if (chunk_shift >= m_chunks.size()) {
      return BigInt(0);
    }

    BigInt result;
    result.m_chunks.resize(m_chunks.size() - chunk_shift, 0);

    if (bit_shift == 0) {
      for (size_t i = 0; i < result.m_chunks.size(); ++i) {
        result.m_chunks[i] = m_chunks[i + chunk_shift];
      }
    } else {
      uint32_t carry = 0;
      for (int i = m_chunks.size() - 1; i >= (int)chunk_shift; --i) {
        uint32_t current_chunk = m_chunks[i];
        result.m_chunks[i - chunk_shift] = (current_chunk >> bit_shift) | carry;
        carry = current_chunk << (32 - bit_shift);
      }
    }

    result.trim();
    return result;
  }

  // --- Hàm xuất (Output) ---
  std::string toDecimalString() const {
    if (*this == BigInt(0)) {
      return "0";
    }

    BigInt temp = *this;
    std::string result_str;
    BigInt ten(10);

    while (temp > BigInt(0)) {
      BigInt quotient, remainder;
      temp.divmod(ten, quotient, remainder);
      if (remainder.m_chunks
              .empty()) {  // Should not happen with non-zero numbers
        result_str += "0";
      } else {
        result_str += std::to_string(remainder.m_chunks[0]);
      }
      temp = quotient;
    }

    std::reverse(result_str.begin(), result_str.end());
    return result_str;
  }

  // Chuyển đổi BigInt sang chuỗi Hex
  std::string toHexString() const {
    if (m_chunks.empty() || (m_chunks.size() == 1 && m_chunks[0] == 0)) {
      return "0";
    }

    std::stringstream ss;
    ss << std::hex;

    // In chunk cuối cùng mà không có padding
    ss << m_chunks.back();

    // In các chunk còn lại với padding 8 số 0
    for (int i = m_chunks.size() - 2; i >= 0; --i) {
      ss << std::setw(8) << std::setfill('0') << m_chunks[i];
    }

    return ss.str();
  }

  // Hàm bạn bè để cho phép 'cout << myBigInt'
  friend std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    os << num.toDecimalString();
    return os;
  }

 private:
  // Helper for division and modulo
  void divmod(const BigInt& divisor, BigInt& quotient,
              BigInt& remainder) const {
    BigInt zero(0);
    if (divisor == zero) {
      throw std::invalid_argument("Division by zero.");
    }

    if (*this < divisor) {
      quotient = zero;
      remainder = *this;
      return;
    }

    remainder = *this;
    quotient = zero;

    BigInt current_divisor = divisor;
    int shifts = 0;
    while ((current_divisor << 1) <= remainder) {
      current_divisor = current_divisor << 1;
      shifts++;
    }

    while (shifts >= 0) {
      quotient = quotient << 1;
      if (remainder >= current_divisor) {
        remainder = remainder - current_divisor;
        if (quotient.m_chunks.empty()) quotient.m_chunks.push_back(0);
        quotient.m_chunks[0] |= 1;
      }
      current_divisor = current_divisor >> 1;
      shifts--;
    }
    quotient.trim();
    remainder.trim();
  }
};

#endif  // CONSTANTS_H