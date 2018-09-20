cmake_minimum_required(VERSION 3.12)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project("COMP 445 Lab Assignment 1")

enable_testing()

set(SOURCES src/main.c)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}" CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS "${warnings}" CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

add_executable(httpc ${SOURCES})
target_link_libraries(httpc httpClientLibrary)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL "A flag showing that CMake has configured at least once.")
