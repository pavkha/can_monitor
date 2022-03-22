cmake_minimum_required(VERSION 3.5)

project(can_monitor_vscom VERSION 2.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

configure_file(
    "${CMAKE_SOURCE_DIR}/main/version.h.in"
    "${CMAKE_SOURCE_DIR}/main/version.h")

set(QT_VERSION_MAJOR 5)

set(CMAKE_PREFIX_PATH "/usr/local/Qt-5.15.2")

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets SerialPort REQUIRED)

include_directories(
    "${CMAKE_SOURCE_DIR}/third_party/VSCAN_API_1_10_2/include"
    "${CMAKE_SOURCE_DIR}/device")

link_directories(
    "${CMAKE_SOURCE_DIR}/third_party/VSCAN_API_1_10_2/Linux x86-64"
    "${CMAKE_SOURCE_DIR}/third_party/VSCAN_API_1_10_2/Linux")

find_package(GTest REQUIRED)

enable_testing()

add_subdirectory(device)
add_subdirectory(main)
add_subdirectory(test)
