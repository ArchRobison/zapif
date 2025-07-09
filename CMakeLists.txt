cmake_minimum_required(VERSION 4.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(STRINGS version.txt ZAPIF_VERSION)

project(zapif
    LANGUAGES C CXX
    VERSION ${ZAPIF_VERSION}
    DESCRIPTION "Simplifies C/C++ preprocessor directives in source files"
    HOMEPAGE_URL "https://github.com/ArchRobison/zapif"
)

# Enabling these options causes the CTest-based tests to fail due to how they check output, but can be useful for debugging.
option(ZAPIF_DEBUG_FLEX "If the debug flag is passed to flex" OFF)
option(ZAPIF_DEBUG_BISON "If the debug flag is passed to bison" OFF)

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
