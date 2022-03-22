cmake_minimum_required(VERSION 3.5)

add_executable(device_test device_test.cpp device_test.h)

target_link_libraries(device_test
    device
    GTest::Main)

add_test(NAME device_test COMMAND device_test)
