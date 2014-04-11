if (NOT IOS AND NOT ANDROID)
    return()
endif()

set(SRC)
set(LIBS Xli)

if (XLI_PLATFORM_IOS)

    set(SRC ${SRC} coreaudio/CoreSoundPlayer.mm)

elseif (XLI_PLATFORM_ANDROID)

    set(SRC ${SRC} opensles/SlesSoundPlayer.cpp)
    set(LIBS ${LIBS} OpenSLES)

endif()

add_library(XliSoundPlayer ${SRC})
target_link_libraries(XliSoundPlayer ${LIBS})
