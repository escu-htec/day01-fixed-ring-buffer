cmake_minimum_required(VERSION 3.16)
project(day01_fixed_ring_buffer C)

# 1. Enforce C17 standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 2. Strict compiler warning flags
add_compile_options(-Wall -Wextra -Wpedantic)

# 3. Ring buffer library
add_library(int_ring src/int_ring.c)
target_include_directories(int_ring PUBLIC include)

# 4. Fetch and pin the Unity unit testing framework
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.0
)
FetchContent_MakeAvailable(unity)

# 5. Test executable and CTest registration
enable_testing()
add_executable(int_ring_test tests/int_ring_test.c)
target_link_libraries(int_ring_test PRIVATE int_ring unity)

add_test(NAME int_ring_test COMMAND int_ring_test)