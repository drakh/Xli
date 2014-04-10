include(${CMAKE_CURRENT_LIST_DIR}/cmake/modules/XliLinkProject.cmake)
link_project(${CMAKE_CURRENT_LIST_DIR})

if (CMAKE_SYSTEM_NAME MATCHES "Linux")

    execute_process(COMMAND uname -m OUTPUT_VARIABLE MACHINE)

    if (${MACHINE} MATCHES "arm*")  
        # GLES on RPi
        include_directories(/opt/vc/include)
        link_directories(/opt/vc/lib)
    endif()

endif()
