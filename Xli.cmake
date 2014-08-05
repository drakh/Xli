include(${CMAKE_CURRENT_LIST_DIR}/cmake/modules/XliLinkProject.cmake)
link_project(${CMAKE_CURRENT_LIST_DIR})

if (XLI_PLATFORM_LINUX AND XLI_ARCH_ARM)
    # GLES on RPi
    include_directories(/opt/vc/include)
    link_directories(/opt/vc/lib)
endif()
