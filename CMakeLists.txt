cmake_minimum_required(VERSION 3.10)
project(msl_library C)

add_library(msl_library INTERFACE)

target_include_directories(msl_library
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
