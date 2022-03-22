cmake_minimum_required(VERSION 3.5)

add_library(device device.cpp device.h)
add_library(Device::vscom ALIAS device)

target_link_libraries(device PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SerialPort
)
