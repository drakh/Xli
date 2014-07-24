include(${CMAKE_CURRENT_LIST_DIR}/XliVars.cmake)

function(link_project PROJECT_DIR)

    include_directories(${PROJECT_DIR}/include)

    if (XLI_PLATFORM_OSX)

        link_directories(${PROJECT_DIR}/lib/OSX/x86)

    elseif (XLI_PLATFORM_LINUX)

        if (XLI_ARCH_ARM)
            link_directories(${PROJECT_DIR}/lib/linux/arm)            
        elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
            link_directories(${PROJECT_DIR}/lib/linux/x86_64)
        else()
            link_directories(${PROJECT_DIR}/lib/linux/x86_32)
        endif()

    elseif (XLI_PLATFORM_WIN32)

        if (MSVC)
            if (MSVC12)
                link_directories("${PROJECT_DIR}/lib/vs2013/$(PlatformShortName)")
            elseif (MSVC11)
                link_directories("${PROJECT_DIR}/lib/vs2012/$(PlatformShortName)")
            elseif (MSVC10)
                link_directories("${PROJECT_DIR}/lib/vs2010/$(PlatformShortName)")
            else()
                message(FATAL_ERROR "Unsupported MSVC version")
            endif()
        else()
            if (CMAKE_SIZEOF_VOID_P EQUAL 8)
                set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/mingw/x86_64)
            else()
                set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/mingw/x86_32)
            endif()
        endif()
        
    else()

        message(FATAL_ERROR "Unsupported system")

    endif()

endfunction()
