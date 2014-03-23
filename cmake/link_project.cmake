function(link_project PROJECT_DIR)

	include_directories(${PROJECT_DIR}/include)

	if (CMAKE_SYSTEM_NAME MATCHES "Darwin")

		link_directories(${PROJECT_DIR}/lib/OSX/x86)

	elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

        execute_process(COMMAND uname -m OUTPUT_VARIABLE MACHINE)

        if (${MACHINE} MATCHES "arm*")
            link_directories(${PROJECT_DIR}/lib/linux/arm)
            
            # GLES on RPi
            include_directories(/opt/vc/include)
            link_directories(/opt/vc/lib)
            
		elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
			link_directories(${PROJECT_DIR}/lib/linux/x86_64)
		else()
			link_directories(${PROJECT_DIR}/lib/linux/x86_32)
		endif()

	elseif (MSVC)

		if (MSVC12)
			link_directories("${PROJECT_DIR}/lib/vs2013/$(PlatformShortName)")
		elseif (MSVC11)
			link_directories("${PROJECT_DIR}/lib/vs2012/$(PlatformShortName)")
		elseif (MSVC10)
			link_directories("${PROJECT_DIR}/lib/vs2010/$(PlatformShortName)")
		else()
			message(FATAL_ERROR "Unsupported MSVC version")
		endif()
		
    elseif (WIN32)

        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/mingw/x86_64)
        else()
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/mingw/x86_32)
        endif()
        
	else()

		message(FATAL_ERROR "Unsupported system")

	endif()

endfunction()
