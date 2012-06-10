#include <XliGraphics/Mesh.h>
#include <XliGraphics/Semantic.h>

namespace Xli
{
	namespace Psg
	{
		uint Mesh::GetIndex(uint index)
		{
			switch (Indices->GetStride())
			{
			case 1: return (uint)*(UInt8*)Indices->GetElementPtr(index);
			case 2: return (uint)*(UInt16*)Indices->GetElementPtr(index);
			case 4: return (uint)*(UInt32*)Indices->GetElementPtr(index);
			}

			XLI_THROW("Unsupported index size: " + (String)(int)Indices->GetStride());
		}

		Vector3 Mesh::GetPosition(uint index)
		{
			for (int i = 0; i < Attributes.Length(); i++)
			{
				if (Attributes[i].Semantic == Semantic::Position)
				{
					switch (Layout[i].Format)
					{
					case FormatRGB_32_32_32_Float:
						return *(Vector3*)(VertexBuffers[Layout[i].Buffer]->GetElementPtr(index) + Layout[i].Offset);
					}

					XLI_THROW("Position has unsupported format: " + (String)FormatInfo::ToString(Layout[i].Format));
				}
			}

			XLI_THROW("Position attribute not found");
		}
	}
}
