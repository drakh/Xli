set(SRC GLContext.cpp)
set(LIBS Xli)

if (XLI_PLATFORM_IOS)

    include_directories(${PROJECT_SOURCE_DIR}/src/core/sdl2/3rdparty/SDL2/include)
    set(SRC ${SRC} sdl2/SDL2GLContext.cpp)

elseif (XLI_PLATFORM_OSX)

    add_definitions(-DXLI_HAS_GLEW -DGLEW_STATIC)
    include_directories(${PROJECT_SOURCE_DIR}/src/core/sdl2/3rdparty/SDL2/include)
    include_directories(${PROJECT_SOURCE_DIR}/src/gl/3rdparty/glew/include)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-framework,OpenGL")
    set(SRC ${SRC} 3rdparty/glew/src/glew.c sdl2/SDL2GLContext.cpp)

elseif (XLI_PLATFORM_LINUX)

    add_definitions(`sdl2-config --cflags`)
    set(SRC ${SRC} sdl2/SDL2GLContext.cpp)
    set(LIBS ${LIBS} SDL2)

    if (NOT XLI_ARCH_ARM)
        add_definitions(-DXLI_HAS_GLEW)
        set(LIBS ${LIBS} GL GLEW)
    endif()

endif()

add_library(XliGL ${SRC})
target_link_libraries(XliGL ${LIBS})
