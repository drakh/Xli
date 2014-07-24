if (WIN32)

    set(XLI_PLATFORM_WIN32 True)

elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")

    if (IOS)
        set(XLI_PLATFORM_IOS True)
    else()
        set(XLI_PLATFORM_OSX True)
    endif()

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

    if (ANDROID)
        set(XLI_PLATFORM_ANDROID True)
    else()
        set(XLI_PLATFORM_LINUX True)
        execute_process(COMMAND uname -m OUTPUT_VARIABLE MACHINE)

        if (${MACHINE} MATCHES "arm*")
            set(XLI_ARCH_ARM True)
        endif()
    endif()

endif()
