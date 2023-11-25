cmake_minimum_required(VERSION 3.24)

project(MPSSE)

set(MPSSE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_SRC ${MPSSE_SRC_DIR}/ftdi_i2c.c
            ${MPSSE_SRC_DIR}/ftdi_infra.c
            ${MPSSE_SRC_DIR}/ftdi_mid.c
            ${MPSSE_SRC_DIR}/ftdi_spi.c
)

add_library(MPSSE STATIC ${LIB_SRC})

target_include_directories(MPSSE PRIVATE ${MPSSE_SRC_DIR}/src
                                 PUBLIC  include
                                         ftd2xx)

#install(DIRECTORY include DESTINATION include)
#install(DIRECTORY ftd2xx DESTINATION include)

#install(TARGETS MPSSE
#        LIBRARY DESTINATION lib)
