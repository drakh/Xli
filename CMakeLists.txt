project("Xli Library")

cmake_minimum_required(VERSION 2.6)

set(XLI_INCLUDE ${PROJECT_SOURCE_DIR}/include)
include_directories(${XLI_INCLUDE})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
link_directories(${LIBRARY_OUTPUT_PATH})

if (MSVC)

	add_definitions(-DUNICODE -D_UNICODE)

	set(CMAKE_BUILD_SHARED_LIBS FALSE INTERNAL)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" INTERNAL)

	set(XLI_MT FALSE CACHE BOOL "Static Linked Runtime Library (/MT)")
	set(XLI_LTCG FALSE CACHE BOOL "Link-Time Code Generation (/LTCG)")
	set(XLI_SDL2 FALSE CACHE BOOL "Use Simple Direct Media Layer 2.0")

	set(CMAKE_C_FLAGS "/MP")
	set(CMAKE_CXX_FLAGS "/MP")

	if (XLI_MT)

		set(CMAKE_C_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
		set(CMAKE_C_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")

		set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
		set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")

		set(CMAKE_RELEASE_POSTFIX MT)
		set(CMAKE_DEBUG_POSTFIX MTD)

	else()

		set(CMAKE_RELEASE_POSTFIX MD)
		set(CMAKE_DEBUG_POSTFIX MDD)
		
	endif()

	if (XLI_LTCG)
	
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /LTCG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /LTCG")
		
	endif()

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH})

elseif (UNIX)

	set(BUILD_SHARED_LIBS TRUE CACHE BOOL "Build shared or static libs")

	if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type - Release or Debug" FORCE)
	endif()

	if (APPLE)

		if (NOT CMAKE_OSX_ARCHITECTURES)
			set(CMAKE_OSX_ARCHITECTURES "x86_64;i386" CACHE STRING "OSX architectures - 'i386', 'x86_64' or 'x86_64;i386' (universal)" FORCE)
		endif()

		include_directories(${PROJECT_SOURCE_DIR}/src/platform/sdl2/SDL2-OSX/include)
		link_directories(${PROJECT_SOURCE_DIR}/src/platform/sdl2/SDL2-OSX/lib)

		include_directories(/usr/X11/include /usr/X11/include/freetype2)
		link_directories(/usr/X11/lib)

		set(CMAKE_C_FLAGS "-Wno-switch-enum")
		set(CMAKE_CXX_FLAGS "-Wno-switch-enum")

	else()

		add_definitions("`sdl2-config --cflags`")
		set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

	endif()

	install(
		DIRECTORY ${PROJECT_SOURCE_DIR}/lib/ 
		DESTINATION lib 
		PATTERN ".svn" EXCLUDE
		)

	install(
		DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
		DESTINATION include 
		PATTERN ".svn" EXCLUDE
		)

endif()

add_subdirectory(src/core)
add_subdirectory(src/media)
add_subdirectory(src/audio)
add_subdirectory(src/main)

