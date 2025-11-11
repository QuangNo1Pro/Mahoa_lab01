# tests/CMakeLists.txt

# Bắt mọi file *.spec.cpp trong thư mục tests
file(GLOB TEST_SPECS "${PROJECT_SOURCE_DIR}/tests/*.spec.cpp")

set(TEST_TARGETS "")
foreach(spec IN LISTS TEST_SPECS)
  get_filename_component(name ${spec} NAME_WE)

  add_executable(${name} ${spec})
  target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/include)
  target_link_libraries(${name} PRIVATE lab01_core)

  add_test(NAME ${name} COMMAND ${name})
  list(APPEND TEST_TARGETS ${name})
endforeach()

# Target gom để build tất cả test exe (test.sh sẽ gọi)
add_custom_target(tests_all DEPENDS ${TEST_TARGETS})
