cmake_minimum_required(VERSION 3.21)
project(ftdi_i2c_com)

add_executable(ftdi_i2c_com main.cpp)

set_property(TARGET ftdi_i2c_com PROPERTY CXX_STANDARD 20)

target_link_libraries(ftdi_i2c_com PUBLIC -L"${CMAKE_CURRENT_SOURCE_DIR}/ftdi" ftd2xx)
target_include_directories(ftdi_i2c_com PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ftdi")
