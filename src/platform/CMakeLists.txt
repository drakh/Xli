set(Sources
	MessageBox.cpp
	NativeFileSystem.cpp
	Pipe.cpp
	Profiling.cpp
	Selector.cpp
	Thread.cpp
	TimedTrigger.cpp
	Window.cpp
	)

if (WIN32)

	if (XLI_SDL2)

		SET(Sources ${Sources}
			sdl2/SDL2Cond.cpp
			sdl2/SDL2Mutex.cpp
			sdl2/SDL2Thread.cpp
			sdl2/SDL2Time.cpp
			sdl2/SDL2Window.cpp
			sdl2/SDL2Display.cpp
			sdl2/SDL2FileSystem.cpp
			win32/Win32FileSystem.cpp
			win32/Win32Helpers.cpp
			win32/Win32MessageBox.cpp
			)

	else()

		SET(Sources ${Sources}
			generic/GenericAssetFileSystem.cpp
			win32/Win32Cond.cpp
			win32/Win32FileSystem.cpp
			win32/Win32Helpers.cpp
			win32/Win32MessageBox.cpp
			win32/Win32Mutex.cpp
			win32/Win32Socket.cpp
			win32/Win32Thread.cpp
			win32/Win32Time.cpp
			win32/Win32Window.cpp
			win32/Win32Display.cpp
			win32/Win32Helpers.cpp
			)

	endif()

elseif (APPLE)

    set(Sources ${Sources}
		generic/GenericDialogs.cpp
		generic/GenericMessageBox.cpp
		generic/GenericAssetFileSystem.cpp
		posix/PosixEnvironment.cpp
		posix/PosixPipe.cpp
		posix/PosixSocket.cpp
		posix/PosixTime.cpp
		sdl2/SDL2Window.cpp
		sdl2/SDL2Display.cpp
		sdl2/SDL2FileSystem.cpp
		sdl2/SDL2Cond.cpp
		sdl2/SDL2Mutex.cpp
		sdl2/SDL2Thread.cpp
		)

	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lSDL2 -lm -liconv -Wl,-framework,OpenGL -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit")

elseif (UNIX)

	set(Sources ${Sources}
		generic/GenericAssetFileSystem.cpp
		generic/GenericDialogs.cpp
		generic/GenericMessageBox.cpp
		posix/PosixEnvironment.cpp
		posix/PosixFileSystem.cpp
		posix/PosixPipe.cpp
		posix/PosixSocket.cpp
		posix/PosixTime.cpp
		sdl2/SDL2Window.cpp
		sdl2/SDL2Display.cpp
		sdl2/SDL2Cond.cpp
		sdl2/SDL2Mutex.cpp
		sdl2/SDL2Thread.cpp
		)

endif()

add_library(XliPlatform ${Sources})
target_link_libraries(XliPlatform XliCore)

if (UNIX AND NOT APPLE)
	target_link_libraries(XliPlatform SDL2)
endif()
