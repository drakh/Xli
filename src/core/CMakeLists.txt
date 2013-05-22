set(Sources
	3rdparty/ConvertUTF.c
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
        generic/GenericAssetFileSystem.cpp
        win32/Win32Cond.cpp
        win32/Win32FileSystem.cpp
        win32/Win32Helpers.cpp
        win32/Win32MessageBox.cpp
        win32/Win32Thread.cpp
        win32/Win32Time.cpp
        win32/Win32Window.cpp
        win32/Win32Display.cpp
        win32/Win32Helpers.cpp
        )

else()

    set(Sources ${Sources}
        generic/GenericDialogs.cpp
        posix/PosixFileSystemBase.cpp
        posix/PosixTime.cpp
        posix/PosixCond.cpp
        posix/PosixMutex.cpp
        posix/PosixThread.cpp
        sdl2/SDL2Window.cpp
        sdl2/SDL2Display.cpp
        sdl2/SDL2GLContext.cpp
        sdl2/SDL2MessageBox.cpp
        )

	if (APPLE)

	    set(Sources ${Sources} 
	    	3rdparty/glew/src/glew.c
	    	cocoa/CocoaFileSystem.mm
	    	)

	    add_definitions(-DGLEW_STATIC -DXLI_HAS_GLEW)
	    include_directories(${PROJECT_SOURCE_DIR}/src/core/3rdparty/glew/include)

	    include_directories(${PROJECT_SOURCE_DIR}/src/core/3rdparty/SDL2/include)
	    link_directories(${PROJECT_SOURCE_DIR}/src/core/3rdparty/SDL2/lib)

	    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lSDL2 -lm -liconv -Wl,-framework,OpenGL -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit")

	else()

		set(Sources ${Sources}
			generic/GenericAssetFileSystem.cpp
			posix/PosixFileSystem.cpp
			)

		add_definitions("`sdl2-config --cflags`" -DXLI_HAS_GLEW)

	endif()

endif()



add_library(Xli ${Sources})

if (UNIX AND NOT APPLE AND NOT WIN32)
    target_link_libraries(Xli SDL2 pthread GL GLEW)
endif()

