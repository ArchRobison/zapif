cmake_minimum_required(VERSION 4.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(zapif
    LANGUAGES C CXX
    VERSION 1.5.0
)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

add_executable(zapif)

add_subdirectory(src)

target_include_directories(zapif PRIVATE ${zapif_SOURCE_DIR})

if(NOT MSVC)
    target_compile_options(zapif PRIVATE -Wall -Wextra)
endif()

install(TARGETS zapif)

include(CTest)
enable_testing()
add_subdirectory(test)
