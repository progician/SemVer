cmake_minimum_required(VERSION 3.13)
project(SemVer)

add_library(SemVer
    src/Interval.cpp
    src/Version.cpp
    src/VersionRange.cpp
)
target_compile_features(SemVer PUBLIC cxx_std_14)
target_include_directories(SemVer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

option(ENABLE_UNITTESTS "Build and run the unit tests." ON)
if (ENABLE_UNITTESTS)
    find_package(Catch2 REQUIRED)
    add_library(CatchTestMain OBJECT src/CatchTestMain.cpp)
    target_link_libraries(CatchTestMain PUBLIC Catch2::Catch2)
    
    add_executable(SemVer.UT
        src/Interval.Test.cpp
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

install(
    TARGETS SemVer
    EXPORT SemVerTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(EXPORT SemVerTargets NAMESPACE SemVer:: DESTINATION cmake)
install(DIRECTORY include/ DESTINATION include)
