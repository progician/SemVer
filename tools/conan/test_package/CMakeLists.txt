cmake_minimum_required(VERSION 3.13)
project(SemVerTestPackage LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_set_find_paths()

find_package(SemVer REQUIRED)

add_executable(test_package test_package.cpp)
target_link_libraries(test_package PRIVATE SemVer::SemVer)