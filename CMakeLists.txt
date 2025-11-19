cmake_minimum_required(VERSION 3.10)
project(OOP5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(OOP5
    main.cpp
    src/functions.cpp
)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests
    tests/test.cpp
    src/functions.cpp
)

target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
add_test(NAME all_tests COMMAND tests)