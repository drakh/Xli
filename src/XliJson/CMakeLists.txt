file(GLOB SRC
    ../../include/XliJson/*.h
    ../../include/XliJson.h
    3rdparty/*.h
    3rdparty/*.c
    *.cpp)

set(LIBS Xli)

add_library(XliJson ${SRC})
target_link_libraries(XliJson ${LIBS})
