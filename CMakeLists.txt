cmake_minimum_required(VERSION 3.14)
project(qasync VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(qasync/qasync.cmake)
list(TRANSFORM SRC_LIB PREPEND "qasync/")
add_custom_target(lib
    SOURCES
    ${SRC_LIB}
)
add_subdirectory(example)

