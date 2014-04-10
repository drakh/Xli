if (UNIX AND NOT WIN32 AND NOT IOS)
    set(BUILD_SHARED_LIBS TRUE CACHE BOOL "Build shared or static libs")
endif()

if (CMAKE_COMPILER_IS_GNUCC AND NOT APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")

    if (IOS)
    
        set(XLI_PLATFORM_IOS True)
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/iOS)

        if (IOS_PLATFORM MATCHES OS)
            set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH}/Release-iphoneos)
            set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH}/Debug-iphoneos)
        elseif (IOS_PLATFORM MATCHES SIMULATOR)
            set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH}/Release-iphonesimulator)
            set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH}/Debug-iphonesimulator)
        endif()
    
    else()
    
        set(XLI_PLATFORM_OSX True)
        set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/OSX/x86)
    
    endif()

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

    if (ANDROID)

        set(XLI_PLATFORM_ANDROID True)
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/android/${ANDROID_ABI})

    else()

        set(XLI_PLATFORM_LINUX True)
        execute_process(COMMAND uname -m OUTPUT_VARIABLE MACHINE)

        if (${MACHINE} MATCHES "arm*")
            set(XLI_ARCH_ARM True)
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/linux/arm)
            
            # GLES on RPi
            include_directories(/opt/vc/include)
            link_directories(/opt/vc/lib)

        elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/linux/x86_64)
        else()
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/linux/x86_32)
        endif()

    endif()

elseif (MSVC)

    set(XLI_PLATFORM_WIN32 True)

    if (MSVC12)
        set(LIBRARY_OUTPUT_PATH ${PROJECT_DIR}/lib/vs2013/$(PlatformShortName))
    elseif (MSVC11)
        set(LIBRARY_OUTPUT_PATH ${PROJECT_DIR}/lib/vs2012/$(PlatformShortName))
    elseif (MSVC10)
        set(LIBRARY_OUTPUT_PATH ${PROJECT_DIR}/lib/vs2010/$(PlatformShortName))
    else()
        message(FATAL_ERROR "Unsupported MSVC version")
    endif()

    if (CMAKE_CONFIGURATION_TYPES MATCHES "")
        set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Solution Configurations" FORCE)
    endif()

    if (CMAKE_CXX_FLAGS MATCHES "")
        set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /DUNICODE /D_UNICODE /W3 /GR- /EHsc /MP" CACHE STRING "C++ Compiler Flags" FORCE)
    endif()

    if (CMAKE_C_FLAGS MATCHES "")
        set(CMAKE_C_FLAGS "/ /DWIN32 /D_WINDOWS /DUNICODE /D_UNICODE /W3 /MP" CACHE STRING "C Compiler Flags" FORCE)
    endif()

    #set(XLI_MSVC_MT FALSE CACHE BOOL "Static Linked Runtime Library (/MT)")

    if (XLI_MSVC_MT)

        set(CMAKE_C_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
        set(CMAKE_C_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")

        set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
        set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
        
    endif()

elseif (WIN32)

    set(XLI_PLATFORM_WIN32 True)

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/mingw/x86_64)
    else()
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/mingw/x86_32)
    endif()

else()

    message(FATAL_ERROR "Unsupported system")

endif()

if (CMAKE_GENERATOR MATCHES "Unix Makefiles")

    install(DIRECTORY ${LIBRARY_OUTPUT_PATH}/
            DESTINATION lib
            PATTERN "Debug" EXCLUDE
            PATTERN "Release" EXCLUDE)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
            DESTINATION include)

endif()
