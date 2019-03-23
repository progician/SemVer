cmake_minimum_required(VERSION 3.13)
project(SemVer VERSION 0.1.0)

configure_file(include/SemVer/VersionNumbers.h.in include/SemVer/VersionNumbers.h)
add_library(SemVer)
target_sources(SemVer
    PRIVATE
        src/Version.cpp
        src/VersionRange.cpp
)
target_compile_features(SemVer PUBLIC cxx_std_14)
target_include_directories(SemVer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

option(ENABLE_UNITTESTS "Build and run the unit tests." ON)
if (ENABLE_UNITTESTS)
    find_package(Catch2 REQUIRED)
    add_library(CatchTestMain OBJECT src/CatchTestMain.cpp)
    target_link_libraries(CatchTestMain PUBLIC Catch2::Catch2)
    
    add_executable(SemVer.UT
        src/Version.Test.cpp
        src/VersionRange.Test.cpp
    )
    target_link_libraries(SemVer.UT PRIVATE CatchTestMain SemVer)

    add_custom_command(
        TARGET SemVer.UT POST_BUILD
        COMMAND SemVer.UT
    )
endif()

option(GENERATE_DOCS "Generate code documentation with Doxygen" OFF)
if (GENERATE_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(SemVerDocs
        include src
        COMMENT "Generating documentation..."
    )
endif()


add_executable(semver-cl src/SemVerCL.cpp)
target_compile_features(semver-cl PRIVATE cxx_std_14)
target_link_libraries(semver-cl PRIVATE SemVer)

option(ENABLE_TESTS "Exposes the black-box test suite" ON)
if (ENABLE_TESTS)
    find_package(Python REQUIRED)
    enable_testing()
    function(add_py_test TEST_NAME TEST_SCRIPT)
        add_test(
            NAME ${TEST_NAME}
            COMMAND Python::Interpreter ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SCRIPT} --semver-cli-exec=$<TARGET_FILE:semver-cl>
        )
    endfunction()

    add_py_test(command_line_options test/cli_options.py)
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
