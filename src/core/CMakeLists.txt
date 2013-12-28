set(Sources
	3rdparty/ConvertUTF.c
	Application.cpp
	ArrayStream.cpp
	BinaryReader.cpp
	BinaryWriter.cpp
	Buffer.cpp
	BufferStream.cpp
	Console.cpp
	DataType.cpp
	DateTime.cpp
	Exception.cpp
	File.cpp
	FileSystem.cpp
	Hash.cpp
	Matrix3.cpp
	Matrix4.cpp
	MessageBox.cpp
	NativeFileSystem.cpp
	Object.cpp
	Path.cpp
	Quaternion.cpp
	Random.cpp
	Stream.cpp
	StringBuilder.cpp
	String.cpp
	Text.cpp
	TextReader.cpp
	TextStream.cpp 
	TextWriter.cpp
	Thread.cpp
	ToString.cpp
	Unicode.cpp
	Utf16String.cpp
	Window.cpp
	)

if (WIN32)

    SET(Sources ${Sources}
        win32/Win32Cond.cpp
	win32/Win32Dialogs.cpp
        win32/Win32FileSystem.cpp
        win32/Win32Helpers.cpp
        win32/Win32MessageBox.cpp
        win32/Win32Thread.cpp
        win32/Win32Time.cpp
        win32/Win32Window.cpp
        win32/Win32Display.cpp
        win32/Win32Helpers.cpp
        )

elseif (UNIX)

    set(Sources ${Sources}
        generic/GenericDialogs.cpp
        posix/PosixFileSystemBase.cpp
        posix/PosixTime.cpp
        posix/PosixCond.cpp
        posix/PosixMutex.cpp
        posix/PosixThread.cpp
        sdl2/SDL2Window.cpp
        sdl2/SDL2Display.cpp
        sdl2/SDL2MessageBox.cpp
        )

	if (APPLE)

		set(SDL2_DIR "${PROJECT_SOURCE_DIR}/src/core/sdl2/3rdparty/SDL2")

	    set(Sources ${Sources} 
	    	cocoa/CocoaFileSystem.mm
			${SDL2_DIR}/src/atomic/SDL_atomic.c
			${SDL2_DIR}/src/atomic/SDL_spinlock.c
			${SDL2_DIR}/src/audio/coreaudio/SDL_coreaudio.c
			${SDL2_DIR}/src/audio/disk/SDL_diskaudio.c
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
			${SDL2_DIR}/src/filesystem/cocoa/SDL_sysfilesystem.m
			${SDL2_DIR}/src/haptic/darwin/SDL_syshaptic.c
			${SDL2_DIR}/src/haptic/SDL_haptic.c
			${SDL2_DIR}/src/joystick/darwin/SDL_sysjoystick.c
			${SDL2_DIR}/src/joystick/SDL_gamecontroller.c
			${SDL2_DIR}/src/joystick/SDL_joystick.c
			${SDL2_DIR}/src/loadso/dlopen/SDL_sysloadso.c
			${SDL2_DIR}/src/power/macosx/SDL_syspower.c
			${SDL2_DIR}/src/power/SDL_power.c
			${SDL2_DIR}/src/render/opengl/SDL_render_gl.c
			${SDL2_DIR}/src/render/opengl/SDL_shaders_gl.c
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
			${SDL2_DIR}/src/video/SDL_video.c
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
	    	)

	    include_directories(${SDL2_DIR}/include)
	    #link_directories(${SDL2_DIR}/lib)

	    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lm -liconv -Wl,-framework,OpenGL -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit")

	else()

		set(Sources ${Sources}
			generic/GenericAssetFileSystem.cpp
			xdg/XdgFileSystem.cpp
			)

		add_definitions(`sdl2-config --cflags`)

	endif()

endif()

add_library(Xli ${Sources})


if (UNIX AND NOT APPLE AND NOT WIN32)
    target_link_libraries(Xli SDL2 pthread)
endif()
