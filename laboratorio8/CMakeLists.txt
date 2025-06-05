cmake_minimum_required(VERSION 3.10)
project(CalculatorTests)

# Añadir Google Test (y Google Mock)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# test_calculator
add_executable(test_calculator test_calculator.cpp)
target_link_libraries(test_calculator gtest_main)
add_test(NAME test_calculator COMMAND test_calculator)

# test_authentication
add_executable(test_authentication test_authentication.cpp)
target_link_libraries(test_authentication gmock_main)

# IMPORTANTE: Añadir las rutas de inclusión para Google Mock y Google Test
target_include_directories(test_authentication PRIVATE
    ${gtest_SOURCE_DIR}/googlemock/include
    ${gtest_SOURCE_DIR}/googletest/include
)
add_test(NAME test_authentication COMMAND test_authentication)

# test_seed
add_executable(test_seed test_seed.cpp)
target_link_libraries(test_seed gtest_main)
add_test(NAME test_seed COMMAND test_seed)

# main_random (opcional para imprimir)
add_executable(main_random main_random.cpp)

