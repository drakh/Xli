include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")

	link_directories(${CMAKE_CURRENT_LIST_DIR}/lib/OSX/x86)

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		link_directories(${CMAKE_CURRENT_LIST_DIR}/lib/linux/x86_64)
	else()
		link_directories(${CMAKE_CURRENT_LIST_DIR}/lib/linux/x86_32)
	endif()

elseif (MSVC)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build Configurations (forced)" FORCE)

	if (MSVC12)
		link_directories("${CMAKE_CURRENT_LIST_DIR}/lib/vs2013/$(PlatformShortName)")
	elseif (MSVC11)
		link_directories("${CMAKE_CURRENT_LIST_DIR}/lib/vs2012/$(PlatformShortName)")
	elseif (MSVC10)
		link_directories("${CMAKE_CURRENT_LIST_DIR}/lib/vs2010/$(PlatformShortName)")
	else()
		error("Unsupported MSVC version")
	endif()
	
else()

	error("Unsupported system")

endif()
