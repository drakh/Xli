set(Sources
	AudioBuffer.cpp
	AudioStream.cpp
	SoundChannel.cpp
	Sound.cpp
	Wav.cpp
	)

if (WIN32)
	
	include_directories(3rdparty/portaudio/include)

endif()

if (APPLE)

	include_directories(/opt/local/include)
	link_directories(/opt/local/lib)

endif()

add_library(XliAudio ${Sources})
target_link_libraries(XliAudio Xli portaudio)
