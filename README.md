# 🔐 Lab01 — Số nguyên tố & Trao đổi khóa Diffie–Hellman

## 🧩 Giới thiệu

Đây là bài **Lab 01** của môn _Nhập môn Mã hóa Mật mã_ — Bộ môn **Công nghệ Tri thức**, với mục tiêu giúp sinh viên làm quen với các khái niệm và kỹ thuật cơ bản trong **mật mã học khóa công khai**.

Dự án hiện thực **trao đổi khóa Diffie–Hellman**, trong đó hai bên (Alice và Bob) có thể tính toán được cùng một **bí mật chung** thông qua các phép toán số học mô-đun mà **không cần gửi trực tiếp khóa bí mật** qua kênh truyền.

---

## 🧮 Tổng quan nội dung lập trình

Project bao gồm các phần chính:

- **Xử lý số lớn (Big Integer)** và phép **lũy thừa mô-đun nhanh**
- **Sinh số nguyên tố an toàn (Safe Prime)** — có thể sử dụng thuật toán kiểm tra nguyên tố như _Miller–Rabin_
- **Sinh khóa riêng** trong khoảng `[2, p-2]`
- **Triển khai Diffie–Hellman** để tính bí mật chung và xác minh kết quả của Alice và Bob trùng nhau

---

## ⚙️ Ngôn ngữ và yêu cầu kỹ thuật

- Ngôn ngữ: **C/C++ (chuẩn C++14 trở xuống)**
- Compiler: **g++**
- Dự án được cấu trúc và build bằng **CMake** để dễ dàng biên dịch trên nhiều hệ thống.
- Kích thước số nguyên tố tối thiểu: **512 bit**

---

## 🧩 Thiết lập môi trường VS Code (khuyến nghị)

Khi bạn mở project lần đầu bằng **Visual Studio Code**, sẽ xuất hiện thông báo như sau:

> 💡 _“Do you want to install the recommended extensions for this repository?”_

Hãy chọn **Install** ✅ để tự động cài các extension C++ cần thiết cho project, bao gồm:

- **C/C++ (Microsoft)** — hỗ trợ IntelliSense, format, debug
- **CMake Tools (Microsoft)** — tô màu cú pháp trong file CMake

Nếu không thấy thông báo, bạn có thể tự mở thủ công:

- Nhấn `Ctrl + Shift + X` → gõ “C/C++”, “CMake Tools”
- Hoặc mở Command Palette (`Ctrl + Shift + P`) → “Show Recommended Extensions”

---

# Hướng dẫn chạy project C++ bằng MSYS2 + CMake (Windows)

## 1 Cài MSYS2

- Truy cập: [https://www.msys2.org](https://www.msys2.org)
- Tải file `.exe` ứng với phiên bản hệ điều hành đang sử dụng
- Chạy file `.exe` vừa tải để cài `MSYS2` vào máy

## 2 Mở đúng terminal

- Vào Start Menu → mở **MSYS2 UCRT64**.
- Dòng lệnh phải hiển thị dạng: `username@PC UCRT64 ~` (có chữ **UCRT64**).

## 3 Cài g++/cmake/make (chỉ cần 1 lần)

```sh
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake make
```

Kiểm tra:

```sh
g++ --version
cmake --version
```

## 4 Quy ước đường dẫn trong MSYS2

MSYS2 dùng cú pháp linux-style, không phải `E:\...`.

| Windows                   | Trong MSYS2               |
| ------------------------- | ------------------------- |
| `C:\Users\name\Documents` | `/c/Users/name/Documents` |
| `D:\Projects`             | `/d/Projects`             |
| `E:\CPP\Mahoa_lab01`      | `/e/CPP/Mahoa_lab01`      |

Ví dụ vào thư mục project:

```sh
cd /e/CPP/Mahoa_lab01
```

## 5 Cấu trúc project

```
Mahoa_lab01/
├─ CMakeLists.txt
├─ src/
│  └─ main.cpp
├─ include/
├─ build.sh
└─ build/            (tự tạo khi build)
```

## 6 Chạy chương trình

Lần đầu:

```sh
chmod +x build.sh
./build.sh
```

Các lần sau chỉ cần:

```sh
./build.sh
```

Script sẽ tự:

1. Tạo `build/` nếu chưa có
2. `cmake -B build` để generate
3. `cmake --build build` để biên dịch
4. Chạy file `.exe` vừa build xong

## 7) Lỗi thường gặp

| Vấn đề                                  | Nguyên nhân                         | Cách khắc phục                               |
| --------------------------------------- | ----------------------------------- | -------------------------------------------- |
| `g++: command not found`                | Mở nhầm **MSYS** thay vì **UCRT64** | Mở lại **MSYS2 UCRT64**                      |
| `Permission denied` khi chạy `build.sh` | Chưa cấp quyền                      | `chmod +x build.sh`                          |
| `No CMakeLists.txt`                     | Sai thư mục                         | `cd /e/.../Mahoa_lab01`                      |
| Không thấy `.exe`                       | Mới `cmake`, chưa build             | Dùng `cmake --build build` hoặc `./build.sh` |

---
