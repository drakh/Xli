project("Xli Library")

cmake_minimum_required(VERSION 2.6)

include(cmake/setup_build.cmake)

#setup_build()

if (APPLE)

    set(CMAKE_C_FLAGS "-Wno-switch-enum -Wno-logical-op-parentheses -Wno-parentheses-equality -Wno-extra-tokens -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS "-Wno-switch-enum -Wno-logical-op-parentheses -Wno-parentheses-equality")

endif()

add_subdirectory(src/core)
add_subdirectory(src/media)
add_subdirectory(src/audio)
add_subdirectory(src/main)
add_subdirectory(src/gl)
add_subdirectory(src/gl_helper)
add_subdirectory(src/http)
