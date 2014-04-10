set(SRC XliMain.cpp)

if (XLI_PLATFORM_IOS)
	include_directories(${PROJECT_SOURCE_DIR}/src/core/sdl2/3rdparty/SDL2/include)
elseif (XLI_PLATFORM_ANDROID)
	set(SRC ${SRC} ${PROJECT_SOURCE_DIR}/src/core/android/3rdparty/android_native_app_glue.c)
endif()

add_library(XliMain STATIC ${SRC})
