set(Sources
	ArrayBuffer.cpp
	Atlas.cpp
	AtlasBuilder.cpp
	BitmapFont.cpp
	BitmapFontBuilder.cpp
	CompressedImage.cpp
	Ktx.cpp
	Mesh.cpp
	Parameter.cpp
	PsgReader.cpp
	PsgWriter.cpp
	Semantic.cpp
	SpriteFont.cpp
	Texture.cpp
	)

add_library(XliGraphics ${Sources})
target_link_libraries(XliGraphics XliCore XliMedia)
