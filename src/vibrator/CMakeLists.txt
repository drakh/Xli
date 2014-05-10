if (NOT IOS AND NOT ANDROID)
    return()
endif()

set(SRC)
set(LIBS Xli)

if (XLI_PLATFORM_IOS)

    set(SRC ${SRC} ios/Vibrator.mm)

elseif (XLI_PLATFORM_ANDROID)

    set(SRC ${SRC} android/Vibrator.cpp)

endif()

add_library(XliVibrator ${SRC})
target_link_libraries(XliVibrator ${LIBS})
