file(GLOB SRC
    ../../include/XliPlatform/*.h
    ../../include/XliPlatform.h
    *.cpp)

set(LIBS Xli)

set(SDL2_DIR ../../3rdparty/SDL2)

set(SDL2_SRC
    generic/GenericBundle.cpp
    generic/GenericDialogs.cpp
    posix/PosixFileSystemBase.cpp
    sdl2/SDL2Window.cpp
    sdl2/SDL2Display.cpp
    sdl2/SDL2MessageBox.cpp)

set(SDL2_STATIC_SRC
    cocoa/CocoaFileSystem.mm
    ${SDL2_DIR}/src/atomic/SDL_atomic.c
    ${SDL2_DIR}/src/atomic/SDL_spinlock.c
    ${SDL2_DIR}/src/audio/coreaudio/SDL_coreaudio.c
    ${SDL2_DIR}/src/audio/dummy/SDL_dummyaudio.c
    ${SDL2_DIR}/src/audio/SDL_audio.c
    ${SDL2_DIR}/src/audio/SDL_audiocvt.c
    ${SDL2_DIR}/src/audio/SDL_audiodev.c
    ${SDL2_DIR}/src/audio/SDL_audiotypecvt.c
    ${SDL2_DIR}/src/audio/SDL_mixer.c
    ${SDL2_DIR}/src/audio/SDL_wave.c
    ${SDL2_DIR}/src/cpuinfo/SDL_cpuinfo.c
    ${SDL2_DIR}/src/events/SDL_clipboardevents.c
    ${SDL2_DIR}/src/events/SDL_dropevents.c
    ${SDL2_DIR}/src/events/SDL_events.c
    ${SDL2_DIR}/src/events/SDL_gesture.c
    ${SDL2_DIR}/src/events/SDL_keyboard.c
    ${SDL2_DIR}/src/events/SDL_mouse.c
    ${SDL2_DIR}/src/events/SDL_quit.c
    ${SDL2_DIR}/src/events/SDL_touch.c
    ${SDL2_DIR}/src/events/SDL_windowevents.c
    ${SDL2_DIR}/src/file/cocoa/SDL_rwopsbundlesupport.m
    ${SDL2_DIR}/src/file/SDL_rwops.c
    ${SDL2_DIR}/src/haptic/SDL_haptic.c
    ${SDL2_DIR}/src/joystick/SDL_gamecontroller.c
    ${SDL2_DIR}/src/joystick/SDL_joystick.c
    ${SDL2_DIR}/src/loadso/dlopen/SDL_sysloadso.c
    ${SDL2_DIR}/src/power/SDL_power.c
    ${SDL2_DIR}/src/render/SDL_render.c
    ${SDL2_DIR}/src/render/SDL_yuv_mmx.c
    ${SDL2_DIR}/src/render/SDL_yuv_sw.c
    ${SDL2_DIR}/src/render/software/SDL_blendfillrect.c
    ${SDL2_DIR}/src/render/software/SDL_blendline.c
    ${SDL2_DIR}/src/render/software/SDL_blendpoint.c
    ${SDL2_DIR}/src/render/software/SDL_drawline.c
    ${SDL2_DIR}/src/render/software/SDL_drawpoint.c
    ${SDL2_DIR}/src/render/software/SDL_render_sw.c
    ${SDL2_DIR}/src/render/software/SDL_rotate.c
    ${SDL2_DIR}/src/SDL.c
    ${SDL2_DIR}/src/SDL_assert.c
    ${SDL2_DIR}/src/SDL_error.c
    ${SDL2_DIR}/src/SDL_hints.c
    ${SDL2_DIR}/src/SDL_log.c
    ${SDL2_DIR}/src/stdlib/SDL_getenv.c
    ${SDL2_DIR}/src/stdlib/SDL_iconv.c
    ${SDL2_DIR}/src/stdlib/SDL_malloc.c
    ${SDL2_DIR}/src/stdlib/SDL_qsort.c
    ${SDL2_DIR}/src/stdlib/SDL_stdlib.c
    ${SDL2_DIR}/src/stdlib/SDL_string.c
    ${SDL2_DIR}/src/thread/pthread/SDL_syscond.c
    ${SDL2_DIR}/src/thread/pthread/SDL_sysmutex.c
    ${SDL2_DIR}/src/thread/pthread/SDL_syssem.c
    ${SDL2_DIR}/src/thread/pthread/SDL_systhread.c
    ${SDL2_DIR}/src/thread/pthread/SDL_systls.c
    ${SDL2_DIR}/src/thread/SDL_thread.c
    ${SDL2_DIR}/src/timer/SDL_timer.c
    ${SDL2_DIR}/src/timer/unix/SDL_systimer.c
    ${SDL2_DIR}/src/video/dummy/SDL_nullevents.c
    ${SDL2_DIR}/src/video/dummy/SDL_nullframebuffer.c
    ${SDL2_DIR}/src/video/dummy/SDL_nullvideo.c
    ${SDL2_DIR}/src/video/SDL_blit.c
    ${SDL2_DIR}/src/video/SDL_blit_0.c
    ${SDL2_DIR}/src/video/SDL_blit_1.c
    ${SDL2_DIR}/src/video/SDL_blit_A.c
    ${SDL2_DIR}/src/video/SDL_blit_auto.c
    ${SDL2_DIR}/src/video/SDL_blit_copy.c
    ${SDL2_DIR}/src/video/SDL_blit_N.c
    ${SDL2_DIR}/src/video/SDL_blit_slow.c
    ${SDL2_DIR}/src/video/SDL_bmp.c
    ${SDL2_DIR}/src/video/SDL_clipboard.c
    ${SDL2_DIR}/src/video/SDL_egl.c
    ${SDL2_DIR}/src/video/SDL_fillrect.c
    ${SDL2_DIR}/src/video/SDL_pixels.c
    ${SDL2_DIR}/src/video/SDL_rect.c
    ${SDL2_DIR}/src/video/SDL_RLEaccel.c
    ${SDL2_DIR}/src/video/SDL_shape.c
    ${SDL2_DIR}/src/video/SDL_stretch.c
    ${SDL2_DIR}/src/video/SDL_surface.c
    ${SDL2_DIR}/src/video/SDL_video.c)

if (XLI_PLATFORM_ANDROID)

    set(SRC ${SRC}
        android/AFileSystem.cpp
        android/AJniHelper.cpp
        android/AMessageBox.cpp
        android/AShim.cpp
        android/AStream.cpp
        android/AWindow.cpp
        android/ADisplay.cpp
        generic/GenericDialogs.cpp
        posix/PosixFileSystemBase.cpp)

    set(LIBS ${LIBS} log android)

elseif (XLI_PLATFORM_WIN32)

    set(SRC ${SRC}
        generic/GenericBundle.cpp
        win32/Win32Dialogs.cpp
        win32/Win32FileSystem.cpp
        win32/Win32Helpers.cpp
        win32/Win32MessageBox.cpp
        win32/Win32Window.cpp
        win32/Win32Display.cpp
        win32/Win32Helpers.cpp)

elseif (XLI_PLATFORM_LINUX)

    add_definitions(`sdl2-config --cflags`)
    set(LIBS ${LIBS} SDL2 pthread)

    set(SRC ${SRC} ${SDL2_SRC}
        xdg/XdgFileSystem.cpp)

elseif (XLI_PLATFORM_IOS)

    include_directories(${SDL2_DIR}/include)

    set(SRC ${SRC} ${SDL2_SRC} ${SDL2_STATIC_SRC}
        ios/iInit.mm
        ${SDL2_DIR}/src/joystick/iphoneos/SDL_sysjoystick.m
        ${SDL2_DIR}/src/joystick/iphoneos/SDLUIAccelerationDelegate.m
        ${SDL2_DIR}/src/power/uikit/SDL_syspower.m
        ${SDL2_DIR}/src/render/opengles/SDL_render_gles.c
        ${SDL2_DIR}/src/render/opengles2/SDL_render_gles2.c
        ${SDL2_DIR}/src/render/opengles2/SDL_shaders_gles2.c
        ${SDL2_DIR}/src/video/uikit/SDL_uikitappdelegate.m
        ${SDL2_DIR}/src/video/uikit/SDL_uikitevents.m
        ${SDL2_DIR}/src/video/uikit/SDL_uikitmessagebox.m
        ${SDL2_DIR}/src/video/uikit/SDL_uikitmodes.m
        ${SDL2_DIR}/src/video/uikit/SDL_uikitopengles.m
        ${SDL2_DIR}/src/video/uikit/SDL_uikitopenglview.m
        ${SDL2_DIR}/src/video/uikit/SDL_uikitvideo.m
        ${SDL2_DIR}/src/video/uikit/SDL_uikitview.m
        ${SDL2_DIR}/src/video/uikit/SDL_uikitviewcontroller.m
        ${SDL2_DIR}/src/video/uikit/SDL_uikitwindow.m
        sdl2/SDL2Display_iOS.mm
        sdl2/SDL2Window_iOS.mm)

elseif (XLI_PLATFORM_OSX)

    include_directories(${SDL2_DIR}/include)

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lm -liconv -Wl,-framework,OpenGL -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit")

    set(SRC ${SRC} ${SDL2_SRC} ${SDL2_STATIC_SRC}
        ${SDL2_DIR}/src/audio/disk/SDL_diskaudio.c
        ${SDL2_DIR}/src/dynapi/SDL_dynapi.c
        ${SDL2_DIR}/src/filesystem/cocoa/SDL_sysfilesystem.m
        ${SDL2_DIR}/src/haptic/darwin/SDL_syshaptic.c
        ${SDL2_DIR}/src/joystick/darwin/SDL_sysjoystick.c
        ${SDL2_DIR}/src/power/macosx/SDL_syspower.c
        ${SDL2_DIR}/src/render/opengl/SDL_render_gl.c
        ${SDL2_DIR}/src/render/opengl/SDL_shaders_gl.c
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoaclipboard.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoaevents.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoakeyboard.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoamessagebox.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoamodes.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoamouse.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoamousetap.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoaopengl.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoashape.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoavideo.m
        ${SDL2_DIR}/src/video/cocoa/SDL_cocoawindow.m
        ${SDL2_DIR}/src/video/x11/edid-parse.c
        ${SDL2_DIR}/src/video/x11/imKStoUCS.c
        ${SDL2_DIR}/src/video/x11/SDL_x11clipboard.c
        ${SDL2_DIR}/src/video/x11/SDL_x11dyn.c
        ${SDL2_DIR}/src/video/x11/SDL_x11events.c
        ${SDL2_DIR}/src/video/x11/SDL_x11framebuffer.c
        ${SDL2_DIR}/src/video/x11/SDL_x11keyboard.c
        ${SDL2_DIR}/src/video/x11/SDL_x11messagebox.c
        ${SDL2_DIR}/src/video/x11/SDL_x11modes.c
        ${SDL2_DIR}/src/video/x11/SDL_x11mouse.c
        ${SDL2_DIR}/src/video/x11/SDL_x11opengl.c
        ${SDL2_DIR}/src/video/x11/SDL_x11opengles.c
        ${SDL2_DIR}/src/video/x11/SDL_x11shape.c
        ${SDL2_DIR}/src/video/x11/SDL_x11touch.c
        ${SDL2_DIR}/src/video/x11/SDL_x11video.c
        ${SDL2_DIR}/src/video/x11/SDL_x11window.c
        ${SDL2_DIR}/src/video/x11/SDL_x11xinput2.c
        sdl2/SDL2Display_iOS.mm)

endif()

add_library(XliPlatform ${SRC})
target_link_libraries(XliPlatform ${LIBS})
