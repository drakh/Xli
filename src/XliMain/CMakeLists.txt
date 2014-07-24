set(SRC Main.cpp)

if (XLI_PLATFORM_IOS)
	include_directories(../XliPlatform/3rdparty/SDL2/include)
elseif (XLI_PLATFORM_ANDROID)
	set(SRC ${SRC} ../XliPlatform/3rdparty/android_native_app_glue.c)
endif()

add_library(XliMain STATIC ${SRC})
