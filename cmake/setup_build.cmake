function(setup_build)

    if (UNIX AND NOT WIN32)
        set(BUILD_SHARED_LIBS TRUE CACHE BOOL "Build shared or static libs")
    endif()

    if (CMAKE_COMPILER_IS_GNUCC AND NOT APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
    endif()

    if (CMAKE_SYSTEM_NAME MATCHES "Darwin")

        set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/OSX/x86)

    elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/linux/x86_64)
        else()
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/linux/x86_32)
        endif()

    elseif (MSVC)

        error("FIXME: Visual Studio builds are broken.")

        set(CMAKE_BUILD_SHARED_LIBS FALSE INTERNAL)
        set(CMAKE_CONFIGURATION_TYPES "Debug;Release" INTERNAL)

        set(XLI_MSVC_MT FALSE CACHE BOOL "Static Linked Runtime Library (/MT)")

        if (XLI_MSVC_MT)

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

        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/msvc/x64)
        else()
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/msvc/x86)
        endif()

        set(CMAKE_C_FLAGS "/MP")
        set(CMAKE_CXX_FLAGS "/MP /GR-")

        set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build Configurations (forced)" FORCE)

        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH})
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH})

    else()

        error("Unsupported system")

    endif()

    if (CMAKE_GENERATOR MATCHES "Unix Makefiles")

        install(
            DIRECTORY ${LIBRARY_OUTPUT_PATH}/
            DESTINATION lib
            PATTERN "Debug" EXCLUDE
            PATTERN "Release" EXCLUDE
            )
        install(
            DIRECTORY ${PROJECT_SOURCE_DIR}/include/
            DESTINATION include
            )

    endif()

    include_directories(${PROJECT_SOURCE_DIR}/include)
    link_directories(${LIBRARY_OUTPUT_PATH})

endfunction()
