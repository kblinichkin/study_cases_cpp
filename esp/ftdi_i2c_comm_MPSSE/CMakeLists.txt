cmake_minimum_required(VERSION 3.24)
project(ftdi_i2c_com)

add_executable(ftdi_i2c_com main.cpp)

set (libMPSSE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/libMPSSE)

add_subdirectory(${libMPSSE_DIR})
target_include_directories(ftdi_i2c_com PUBLIC
            "${libMPSSE_DIR}/include"
            "${libMPSSE_DIR}/ftd2xx"
)
target_link_libraries(ftdi_i2c_com PUBLIC MPSSE)
## TODO: investigate if it's possible to use toolchain instead of the next
configure_file(${libMPSSE_DIR}/ftd2xx/libftd2xx.1.4.24.dylib
               ${CMAKE_CURRENT_BINARY_DIR}/libftd2xx.dylib COPYONLY)

# If necessary to use ftd2xx API directly uncomment the next:
#target_link_libraries(ftdi_i2c_com PUBLIC
#            -L"${libMPSSE_DIR}/ftd2xx" ftd2xx
#)

set_property(TARGET ftdi_i2c_com PROPERTY CXX_STANDARD 20)
