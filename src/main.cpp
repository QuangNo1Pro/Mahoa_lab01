#include <iostream>
#include <stdexcept>

#include "constants.h"
#include "generate_private_key.h"
#include "modular_exponentiation.h"
#include "prime_utils.h"

// ===Giao thuc Diffie-Hellman ===
void run_diffie_hellman(int bit_size) {
  // --- Buoc 1: Thong nhat tham so cong khai (p va g) ---
  std::cout << "--- Giao thuc trao doi khoa Diffie-Hellman (" << bit_size
            << " bit) ---" << std::endl;
  std::cout << "\n1. Thong nhat tham so cong khai:" << std::endl;

  // Sinh so nguyen to an toan p.
  // So nguyen to an toan la so co dang p = 2q + 1, voi q cung la so nguyen to.
  std::cout << "   - Dang sinh so nguyen to an toan p..." << std::endl;
  BigInt p = generate_safe_prime(bit_size);
  std::cout << "     -> p (hex) = " << p.toHexString() << std::endl;

  // Chon mot phan tu sinh g cua nhom Z_p*.
  // Voi p la so nguyen to an toan, viec chon g=2 hoac g=5 thuong la an toan va
  // hieu qua.
  BigInt g(5);
  std::cout << "   - Chon phan tu sinh g = " << g << std::endl;

  // --- Buoc 2: Alice sinh khoa rieng va khoa cong khai ---
  std::cout << "\n2. Alice thuc hien:" << std::endl;
  // Sinh khoa rieng 'a' mot cach ngau nhien trong khoang [2, p-2].
  BigInt alice_private_key = generate_private_key(p);
  std::cout << "   - Khoa bi mat (a) (hex): " << alice_private_key.toHexString()
            << std::endl;

  // Tinh khoa cong khai A = g^a mod p.
  BigInt alice_public_key = modular_exponentiation(g, alice_private_key, p);
  std::cout << "   - Khoa cong khai (A = g^a mod p) (hex): "
            << alice_public_key.toHexString() << std::endl;

  // --- Buoc 3: Bob sinh khoa rieng va khoa cong khai ---
  std::cout << "\n3. Bob thuc hien:" << std::endl;
  // Sinh khoa rieng 'b' mot cach ngau nhien trong khoang [2, p-2].
  BigInt bob_private_key = generate_private_key(p);
  std::cout << "   - Khoa bi mat (b) (hex): " << bob_private_key.toHexString()
            << std::endl;

  // Tinh khoa cong khai B = g^b mod p.
  BigInt bob_public_key = modular_exponentiation(g, bob_private_key, p);
  std::cout << "   - Khoa cong khai (B = g^b mod p) (hex): "
            << bob_public_key.toHexString() << std::endl;

  // --- Buoc 4: Trao doi khoa cong khai va tinh toan bi mat chung ---
  std::cout << "\n4. Trao doi khoa va tinh toan bi mat chung:" << std::endl;

  // Alice nhan khoa cong khai B cua Bob va tinh khoa chung.
  // S_A = B^a mod p = (g^b)^a mod p = g^(ba) mod p.
  BigInt shared_secret_alice =
      modular_exponentiation(bob_public_key, alice_private_key, p);
  std::cout << "   - Alice tinh khoa chung (S_A = B^a mod p) (hex): "
            << shared_secret_alice.toHexString() << std::endl;

  // Bob nhan khoa cong khai A cua Alice va tinh khoa chung.
  // S_B = A^b mod p = (g^a)^b mod p = g^(ab) mod p.
  BigInt shared_secret_bob =
      modular_exponentiation(alice_public_key, bob_private_key, p);
  std::cout << "   - Bob tinh khoa chung (S_B = A^b mod p) (hex): "
            << shared_secret_bob.toHexString() << std::endl;

  // --- Buoc 5: Xac minh ---
  // Kiem tra xem hai khoa chung co khop nhau hay khong.
  std::cout << "\n5. Xac minh ket qua:" << std::endl;
  if (shared_secret_alice == shared_secret_bob) {
    std::cout << "   -> THANH CONG! Khoa bi mat chung cua Alice va Bob "
                 "trung khop."
              << std::endl;
  } else {
    std::cout << "   -> THAT BAI! Khoa bi mat chung khong khop." << std::endl;
  }

  std::cout << "\n--- Ket thuc chuong trinh ---" << std::endl;
}

int main(int argc, char* argv[]) {
  try {
    int bit_size = BIT_SIZE;

    std::cout << "Nhap so bit cho p (vd: 512, 1024, 2048): ";
    std::cin >> bit_size;

    if (!std::cin) {
      throw std::invalid_argument(
          "Bit size khong hop le (khong phai so nguyen).");
    }
    if (bit_size < 32) {
      throw std::invalid_argument("Bit size qua nho, vui long chon >= 32.");
    }

    run_diffie_hellman(bit_size);

  } catch (const std::exception& e) {
    std::cerr << "\n[LOI] Da xay ra mot ngoai le: " << e.what() << std::endl;
    std::cin.get();
    return 1;
  } catch (...) {
    std::cerr << "\n[LOI] Da xay ra mot loi khong xac dinh." << std::endl;
    std::cin.get();

    return 1;
  }
  std::cin.get();

  return 0;
}
