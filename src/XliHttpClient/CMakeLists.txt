file(GLOB SRC
	../../include/XliHttpClient/*.h
    ../../include/XliHttpClient.h)

set(LIBS Xli XliPlatform)

if (XLI_PLATFORM_IOS)

	set(SRC ${SRC} ios/CHttpClient.mm)

elseif (XLI_PLATFORM_ANDROID)

	set(SRC ${SRC} android/AHttpClient.cpp)

else()

	set(SRC ${SRC} curl/CurlHttpClient.cpp)
	set(LIBS ${LIBS} curl)

endif()

add_library(XliHttpClient ${SRC})
target_link_libraries(XliHttpClient ${LIBS})
