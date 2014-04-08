set(SRC GLContext.cpp)

if (XLI_PLATFORM_IOS)

    set(SRC ${SRC} sdl2/SDL2GLContext.cpp)
    include_directories(${PROJECT_SOURCE_DIR}/src/core/sdl2/3rdparty/SDL2/include)

elseif (XLI_PLATFORM_OSX)

    set(SRC ${SRC} 3rdparty/glew/src/glew.c)
    set(SRC ${SRC} sdl2/SDL2GLContext.cpp)
    add_definitions(-DXLI_HAS_GLEW -DGLEW_STATIC)
    include_directories(${PROJECT_SOURCE_DIR}/src/gl/3rdparty/glew/include)
    include_directories(${PROJECT_SOURCE_DIR}/src/core/sdl2/3rdparty/SDL2/include)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-framework,OpenGL")

elseif (XLI_PLATFORM_LINUX)

    set(SRC ${SRC} sdl2/SDL2GLContext.cpp)
    add_definitions(`sdl2-config --cflags`)

    if (NOT XLI_ARCH_ARM)
        add_definitions(-DXLI_HAS_GLEW)
    endif()

endif()

add_library(XliGL ${SRC})
target_link_libraries(XliGL Xli)

if (XLI_PLATFORM_LINUX)

    target_link_libraries(XliGL SDL2)

    if (NOT XLI_ARCH_ARM)
        target_link_libraries(GL GLEW)
    endif()

endif()
