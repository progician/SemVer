cmake_minimum_required(VERSION 3.13)
project(SemVer VERSION 0.1.0)

message(STATUS "prefix path: ${CMAKE_PREFIX_PATH}")

configure_file(include/SemVer/VersionNumbers.h.in include/SemVer/VersionNumbers.h)
add_library(SemVer)
target_sources(SemVer
    PRIVATE
        src/Version.cpp
        src/VersionRange.cpp
)
target_compile_features(SemVer PUBLIC cxx_std_14)
target_compile_options(SemVer PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(SemVer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

option(ENABLE_UNITTESTS "Enable unit testing in general." ON)
option(ENABLE_UNITTESTS_IN_BUILD "Build and run the unit tests as part of the build." ON)
if (ENABLE_UNITTESTS)
    find_package(Catch2 REQUIRED)
    add_library(CatchTestMain OBJECT src/CatchTestMain.cpp)
    target_link_libraries(CatchTestMain PUBLIC Catch2::Catch2)

    if (ENABLE_UNITTESTS_IN_BUILD)
        set(UT_IN_ALL_TARGET EXCLUDE_FROM_ALL)
    endif()
    
    add_executable(SemVer.UT ${UT_IN_ALL_TARGET}
        src/Version.Test.cpp
        src/VersionRange.Test.cpp
    )
    target_link_libraries(SemVer.UT PRIVATE CatchTestMain SemVer)
    target_compile_options(SemVer.UT PRIVATE -Wall -Wextra -Wpedantic -Werror)


    if (ENABLE_UNITTESTS_IN_BUILD)
        add_custom_command(
            TARGET SemVer.UT POST_BUILD
            COMMAND SemVer.UT
            COMMENT "Running unit tests"
        )
    else()
        add_custom_target(check-unit
            COMMAND $<TARGET_FILE:SemVer.UT>
            DEPENDS SemVer.UT
            COMMENT "Running unit tests"
        )
    endif()
endif()

add_executable(semver-cl src/SemVerCL.cpp)
target_compile_features(semver-cl PRIVATE cxx_std_14)
target_link_libraries(semver-cl PRIVATE SemVer)

option(ENABLE_TIDY "Enable clang-tidy target for statis analysis." OFF)
option(INCREMENTAL_TIDY "Run clang-tidy as part of the build process (recommended for local development)" OFF)
if (ENABLE_TIDY)
    find_program(RUN_CLANG_TIDY_COMMAND NAMES run-clang-tidy DOC "Script executing clang-tidy on compiler command database.")
    if (NOT RUN_CLANG_TIDY_COMMAND)
        message(FATAL_ERROR "clang-tidy was not found!")
    endif()
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_custom_target(tidy
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${RUN_CLANG_TIDY_COMMAND} -quiet
        COMMENT "Running static analysis checks with ClangTidy"
    )
endif()

option(ENABLE_TESTS "Exposes the black-box test suite" ON)
if (ENABLE_TESTS)
    add_custom_target(check-functional
        COMMAND python -m pytest -q
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test
        COMMENT "Running functional test suite"
        DEPENDS semver-cl
    )
endif()

install(
    TARGETS SemVer semver-cl
    EXPORT SemVerTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(EXPORT SemVerTargets NAMESPACE SemVer:: DESTINATION cmake)
install(DIRECTORY include/ DESTINATION include)
