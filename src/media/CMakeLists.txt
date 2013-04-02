set(Sources
	Bitmap.cpp
	CompressedImage.cpp
	FontFace.cpp
	Format.cpp
	FormattedTextWriter.cpp
	Gzip.cpp
	ImageReader.cpp
	ImageWriter.cpp
	JpegReader.cpp
	JpegWriter.cpp
	Json.cpp
	Ktx.cpp	
	PngReader.cpp
	PngWriter.cpp
	Texture.cpp
	Value.cpp
	Xml.cpp
	Zip.cpp
	)

set(JsonSrc
	3rdparty/json_parser/JSON_parser.c
	)

set(TinyXmlInc 3rdparty/tinyxml)

set(TinyXmlSrc
	3rdparty/tinyxml/tinystr.cpp
	3rdparty/tinyxml/tinyxml.cpp
	3rdparty/tinyxml/tinyxmlerror.cpp
	3rdparty/tinyxml/tinyxmlparser.cpp
	)

set(UnzipInc 3rdparty/unzip)
	
set(UnzipSrc
	3rdparty/unzip/ioapi.c
	3rdparty/unzip/unzip.c
	)

set(ZlibInc 3rdparty/zlib)

set(ZlibSrc
	3rdparty/zlib/adler32.c
	3rdparty/zlib/compress.c
	3rdparty/zlib/crc32.c
	3rdparty/zlib/deflate.c
	3rdparty/zlib/gzio.c
	3rdparty/zlib/infback.c
	3rdparty/zlib/inffast.c
	3rdparty/zlib/inflate.c
	3rdparty/zlib/inftrees.c
	3rdparty/zlib/trees.c
	3rdparty/zlib/uncompr.c
	3rdparty/zlib/zutil.c
	)

set(JpegInc 3rdparty/jpeg)

set(JpegSrc
	3rdparty/jpeg/jcapimin.c
	3rdparty/jpeg/jcapistd.c
	3rdparty/jpeg/jccoefct.c
	3rdparty/jpeg/jccolor.c
	3rdparty/jpeg/jcdctmgr.c
	3rdparty/jpeg/jchuff.c
	3rdparty/jpeg/jcinit.c
	3rdparty/jpeg/jcmainct.c
	3rdparty/jpeg/jcmarker.c
	3rdparty/jpeg/jcmaster.c
	3rdparty/jpeg/jcomapi.c
	3rdparty/jpeg/jcparam.c
	3rdparty/jpeg/jcphuff.c
	3rdparty/jpeg/jcprepct.c
	3rdparty/jpeg/jcsample.c
	3rdparty/jpeg/jctrans.c
	3rdparty/jpeg/jdapimin.c
	3rdparty/jpeg/jdapistd.c
	3rdparty/jpeg/jdatadst.c
	3rdparty/jpeg/jdatasrc.c
	3rdparty/jpeg/jdcoefct.c
	3rdparty/jpeg/jdcolor.c
	3rdparty/jpeg/jddctmgr.c
	3rdparty/jpeg/jdhuff.c
	3rdparty/jpeg/jdinput.c
	3rdparty/jpeg/jdmainct.c
	3rdparty/jpeg/jdmarker.c
	3rdparty/jpeg/jdmaster.c
	3rdparty/jpeg/jdmerge.c
	3rdparty/jpeg/jdphuff.c
	3rdparty/jpeg/jdpostct.c
	3rdparty/jpeg/jdsample.c
	3rdparty/jpeg/jdtrans.c
	3rdparty/jpeg/jerror.c
	3rdparty/jpeg/jfdctflt.c
	3rdparty/jpeg/jfdctfst.c
	3rdparty/jpeg/jfdctint.c
	3rdparty/jpeg/jidctflt.c
	3rdparty/jpeg/jidctfst.c
	3rdparty/jpeg/jidctint.c
	3rdparty/jpeg/jidctred.c
	3rdparty/jpeg/jmemmgr.c
	3rdparty/jpeg/jmemnobs.c
	3rdparty/jpeg/jquant1.c
	3rdparty/jpeg/jquant2.c
	3rdparty/jpeg/jutils.c
	)

set(PngInc 3rdparty/png)

set(PngSrc
	3rdparty/png/png.c
	3rdparty/png/pngerror.c
	3rdparty/png/pngget.c
	3rdparty/png/pngmem.c
	3rdparty/png/pngpread.c
	3rdparty/png/pngread.c
	3rdparty/png/pngrio.c
	3rdparty/png/pngrtran.c
	3rdparty/png/pngrutil.c
	3rdparty/png/pngset.c
	3rdparty/png/pngtrans.c
	3rdparty/png/pngwio.c
	3rdparty/png/pngwrite.c
	3rdparty/png/pngwtran.c
	3rdparty/png/pngwutil.c
	)

set(FreetypeInc 3rdparty/freetype/include)

set(FreetypeSrc
	3rdparty/freetype/src/autofit/autofit.c
	3rdparty/freetype/src/bdf/bdf.c
	3rdparty/freetype/src/cff/cff.c
	3rdparty/freetype/src/base/ftapi.c
	3rdparty/freetype/src/base/ftbase.c
	3rdparty/freetype/src/base/ftbdf.c
	3rdparty/freetype/src/base/ftbitmap.c
	3rdparty/freetype/src/cache/ftcache.c
	3rdparty/freetype/src/base/ftdebug.c
	3rdparty/freetype/src/base/ftgasp.c
	3rdparty/freetype/src/base/ftglyph.c
	3rdparty/freetype/src/base/ftgxval.c
	3rdparty/freetype/src/gzip/ftgzip.c
	3rdparty/freetype/src/base/ftinit.c
	3rdparty/freetype/src/base/ftlcdfil.c
	3rdparty/freetype/src/lzw/ftlzw.c
	3rdparty/freetype/src/base/ftmm.c
	3rdparty/freetype/src/base/ftotval.c
	3rdparty/freetype/src/base/ftpatent.c
	3rdparty/freetype/src/base/ftpfr.c
	3rdparty/freetype/src/base/ftstroke.c
	3rdparty/freetype/src/base/ftsynth.c
	3rdparty/freetype/src/base/ftsystem.c
	3rdparty/freetype/src/base/fttype1.c
	3rdparty/freetype/src/base/ftwinfnt.c
	3rdparty/freetype/src/base/ftxf86.c
	3rdparty/freetype/src/gxvalid/gxvalid.c
	3rdparty/freetype/src/otvalid/otvalid.c
	3rdparty/freetype/src/pcf/pcf.c
	3rdparty/freetype/src/pfr/pfr.c
	3rdparty/freetype/src/pfr/pfrload.c
	3rdparty/freetype/src/pfr/pfrgload.c
	3rdparty/freetype/src/pfr/pfrobjs.c
	3rdparty/freetype/src/pfr/pfrsbit.c
	3rdparty/freetype/src/psaux/psaux.c
	3rdparty/freetype/src/pshinter/pshinter.c
	3rdparty/freetype/src/psnames/psnames.c
	3rdparty/freetype/src/raster/raster.c
	3rdparty/freetype/src/sfnt/sfnt.c
	3rdparty/freetype/src/smooth/smooth.c
	3rdparty/freetype/src/type42/t42drivr.c
	3rdparty/freetype/src/type42/t42objs.c
	3rdparty/freetype/src/type42/t42parse.c
	3rdparty/freetype/src/truetype/truetype.c
	3rdparty/freetype/src/type1/type1.c
	3rdparty/freetype/src/cid/type1cid.c
	3rdparty/freetype/src/winfonts/winfnt.c
	)

include_directories(${TinyXmlInc} ${UnzipInc})
set(Sources ${Sources} ${JsonSrc} ${TinyXmlSrc} ${UnzipSrc})

if (WIN32)

	add_definitions(-DFT2_BUILD_LIBRARY)
	include_directories(${ZlibInc} ${JpegInc} ${PngInc} ${FreetypeInc})
	set(Sources ${Sources} ${ZlibSrc} ${JpegSrc} ${PngSrc} ${FreetypeSrc})

elseif (APPLE)

	add_definitions(-DFT2_BUILD_LIBRARY)
	include_directories(${JpegInc} ${PngInc} ${FreetypeInc})
	set(Sources ${Sources} ${JpegSrc} ${PngSrc} ${FreetypeSrc})
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-framework,CoreFoundation -Wl,-framework,CoreServices -Wl,-framework,ApplicationServices")

elseif (UNIX)

	include_directories(/usr/include/freetype2)

endif()

add_library(XliMedia ${Sources})
target_link_libraries(XliMedia Xli)

if (APPLE)
	target_link_libraries(XliMedia z)
elseif (UNIX)
	target_link_libraries(XliMedia freetype jpeg png z)
endif()
