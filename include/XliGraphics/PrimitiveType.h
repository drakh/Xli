#ifndef __XLI_GRAPHICS_PRIMITIVE_TYPE_H__
#define __XLI_GRAPHICS_PRIMITIVE_TYPE_H__

namespace Xli
{
	enum PrimitiveType
	{
		PrimitiveTypeTriangles = 1,
		PrimitiveTypeLines = 2,
		PrimitiveTypePoints = 3,
		PrimitiveTypeTriangleStrip = 4,
		PrimitiveTypeLineStrip = 5,
		PrimitiveTypeLinesWithAdjacency = 6,
		PrimitiveTypeLineStripWithAdjacency = 7,
		PrimitiveTypeTrianglesWithAdjacency = 8,
		PrimitiveTypeTriangleStripWithAdjacency = 9,
	};
}

#endif
