set(Sources
	SoundPortAudio.cpp
	WavStream.cpp
	)

if (WIN32)
	
	include_directories(3rdparty/portaudio/include)

endif()

if (APPLE)

	include_directories(/opt/local/include)
	link_directories(/opt/local/lib)

endif()

add_library(XliSound ${Sources})
target_link_libraries(XliSound Xli XliPlatform portaudio)
