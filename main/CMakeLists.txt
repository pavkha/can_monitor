cmake_minimum_required(VERSION 3.5)

add_executable(can_monitor main.cpp
    mainwindow.cpp mainwindow.h
    version.h
    res.qrc)

set(VSCOM_CAN_LIB
    "libvs_can_api.a")

target_link_libraries(can_monitor PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SerialPort
    ${VSCOM_CAN_LIB}
    Device::vscom
    pthread)

set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,$ORIGIN -no-pie")
