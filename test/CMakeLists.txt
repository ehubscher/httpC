cmake_minimum_required(VERSION 3.12)

project("HTTP CLI Tests")

set(CMOCKA_DIR "./cmocka/1.1.2" CACHE PATH "The path to the cmocka test framework.")

add_subdirectory(${CMOCKA_DIR} ${CMAKE_BINARY_DIR}/cmocka)

include_directories(SYSTEM ${CMOCKA_DIR}/cmockery ${CMOCKA_DIR}/include)
