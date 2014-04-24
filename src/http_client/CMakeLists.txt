if (WIN32 OR XLI_PLATFORM_LINUX OR XLI_PLATFORM_OSX)
    return()
endif()

set(SRC)
set(LIBS Xli)

if (XLI_PLATFORM_IOS)

	set(SRC ${SRC} cocoa/CHttpClient.mm)

elseif (XLI_PLATFORM_ANDROID)

	set(SRC ${SRC} android/AHttpClient.cpp)

else()

	set(SRC ${SRC} curl/CurlHttpClient.cpp)
	set(LIBS ${LIBS} curl)

endif()

add_library(XliHttpClient ${SRC})
target_link_libraries(XliHttpClient ${LIBS})
