if (WIN32)
    return()
endif()

set(SRC)
set(LIBS Xli)

if (XLI_PLATFORM_IOS)


elseif (XLI_PLATFORM_ANDROID)

	set(SRC ${SRC} android/AHttpClient.cpp)

else()

	set(SRC ${SRC} curl/CurlHttpClient.cpp)
	set(LIBS ${LIBS} curl)

endif()

add_library(XliHttpClient ${SRC})
target_link_libraries(XliHttpClient ${LIBS})
