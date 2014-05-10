project("XliLibrary")

cmake_minimum_required(VERSION 2.8)

include(cmake/modules/XliLibSetup.cmake)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${LIBRARY_OUTPUT_PATH})

if (APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-switch-enum -Wno-logical-op-parentheses -Wno-parentheses-equality -Wno-extra-tokens -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-enum -Wno-logical-op-parentheses -Wno-parentheses-equality")
endif()

add_subdirectory(src/core)

if (NOT XLI_NO_MAIN)
    add_subdirectory(src/main)
endif()

if (NOT XLI_NO_MEDIA)
    add_subdirectory(src/media)
endif()

if (NOT XLI_NO_GL)
    add_subdirectory(src/gl)
endif()

if (NOT XLI_NO_HTTP_CLIENT)
    add_subdirectory(src/http_client)
endif()

if (NOT XLI_NO_SOUND_PLAYER)
    add_subdirectory(src/sound_player)
endif()

if (NOT XLI_NO_VIBRATOR)
    add_subdirectory(src/vibrator)
endif()
