file(GLOB SRC
    ../../3rdparty/ConvertUTF/ConvertUTF.h
    ../../3rdparty/ConvertUTF/ConvertUTF.c
    ../../include/Xli/*.h
    ../../include/Xli.h
    *.cpp)

if (WIN32)
    file(GLOB MORE_SRC win32/*.cpp)
    set(SRC ${SRC} ${MORE_SRC})
else()
    file(GLOB MORE_SRC posix/*.cpp)
    set(SRC ${SRC} ${MORE_SRC})
endif()

if (XLI_PLATFORM_LINUX)
	set(LIBS ${LIBS} pthread)
endif()

add_library(Xli ${SRC})
target_link_libraries(Xli ${LIBS})
