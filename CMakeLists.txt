cmake_minimum_required(VERSION 3.10)
project(Lab01_MaHoa CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ❌ Tránh link tĩnh bừa bãi, dễ lỗi MSVC/MinGW
# set(CMAKE_EXE_LINKER_FLAGS "-static")

# Headers dùng chung
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gom toàn bộ .cpp trong src
file(GLOB SRC_ALL ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Tách main.cpp ra để tạo lib “core”
set(MAIN_SRC "${PROJECT_SOURCE_DIR}/src/main.cpp")
set(SRC_CORE ${SRC_ALL})
list(REMOVE_ITEM SRC_CORE ${MAIN_SRC})

# Thư viện lõi (không có main)
add_library(lab01_core STATIC ${SRC_CORE})
target_include_directories(lab01_core PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Executable chính: build.sh sẽ chạy cái này
add_executable(${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE lab01_core)

# Bật test và add subdir
enable_testing()
add_subdirectory(tests)
