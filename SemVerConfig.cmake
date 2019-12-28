if (TARGET SemVer::SemVer)
    return()
endif()

get_filename_component(_IMPORT_PATH ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
message(STATUS "Found SemVer: ${_IMPORT_PATH}")

include(${CMAKE_CURRENT_LIST_DIR}/SemVerTargets.cmake)
unset(_IMPORT_PATH)