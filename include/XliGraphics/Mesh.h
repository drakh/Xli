#ifndef __CRUNCH_PSG_MESH_H__
#define __CRUNCH_PSG_MESH_H__

#include "ArrayBuffer.h"
#include "PrimitiveType.h"
#include <XliMedia/Format.h>
#include <Xli/Vector4.h>
#include <Xli/String.h>
#include <Xli/Managed.h>

namespace Xli
{
	namespace Psg
	{
		class DecoderInfo: public Object
		{
		public:
			enum Type
			{
				TypeNone = 0,
				TypeScaleBias = 1
			};
			virtual Type GetType() = 0;
		};

		class DecoderInfoNone: public DecoderInfo
		{
		public:
			virtual Type GetType() { return TypeNone; }
		};

		class DecoderInfoScaleBias: public DecoderInfo
		{
		public:
			Vector4 Scale, Bias;
			DecoderInfoScaleBias(): Scale(0,0,0,0), Bias(0,0,0,0) {}
			DecoderInfoScaleBias(const Vector4& scale, const Vector4& bias): Scale(scale), Bias(bias) {}
			virtual Type GetType() { return TypeScaleBias; }
		};

		struct AttributeInfo
		{
			String Semantic;
			uint ComponentCount;
			Managed<DecoderInfo> Decoder;
		};

		struct VertexElement
		{
			Xli::Format Format;
			uint Buffer;
			uint Offset;
		};

		class Mesh: public Object
		{
		public:
			Array<AttributeInfo> Attributes;
			Array<VertexElement> Layout;
			Array<Managed<ArrayBuffer> > VertexBuffers;
			Managed<ArrayBuffer> Indices;
			PrimitiveType Type;
			uint VertexCount;
			uint IndexCount;

			uint GetIndex(uint index);
			Vector3 GetPosition(uint index);
		};
	}
}

#endif
