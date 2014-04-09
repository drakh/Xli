if (NOT IOS AND NOT ANDROID)
    return()
endif()

set(SRC)
set(LIBS Xli)

if (XLI_PLATFORM_IOS)

    set(SRC ${SRC} coreaudio/CoreSimpleAudio.mm)

elseif (XLI_PLATFORM_ANDROID)

    set(SRC ${SRC} opensles/SlesSimpleAudio.cpp)

endif()

add_library(XliSoundPlayer ${SRC})
target_link_libraries(XliAudio ${LIBS})
