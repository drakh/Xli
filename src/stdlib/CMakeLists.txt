set(Sources
	Binary.cpp
	BinaryReader.cpp
	BinaryWriter.cpp
	Buffer.cpp
	BufferStream.cpp
	Console.cpp
	DataType.cpp
	DateTime.cpp
	Directory.cpp
	File.cpp
	FileSystem.cpp
	FormattedTextWriter.cpp
	Path.cpp
	Random.cpp
	Stream.cpp
	StringTools.cpp
	Text.cpp
	TextReader.cpp
	TextWriter.cpp
	TextParser.cpp
	Unicode.cpp
	Value.cpp
	3rdparty/ConvertUTF.c
	)

add_library(XliCore ${Sources})
