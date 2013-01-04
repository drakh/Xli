#ifndef __XLI_BINARY_WRITER_H__
#define __XLI_BINARY_WRITER_H__

#include <Xli/Stream.h>
#include <Xli/Matrix4.h>

namespace Xli
{
	/**
		\ingroup IO
	*/
	class BinaryWriter: public StreamWriter
	{
	public:
		BinaryWriter(Stream* stream);
		virtual ~BinaryWriter();

		template <typename T> void Write(const T& elm)
		{
			stream->WriteSafe((const void*)&elm, sizeof(T), 1);
		}

		template <typename T> void Write(const Vector2t<T>& vec) { stream->WriteSafe(vec.Data(), sizeof(T), 2); }
		template <typename T> void Write(const Vector3t<T>& vec) { stream->WriteSafe(vec.Data(), sizeof(T), 3); }
		template <typename T> void Write(const Vector4t<T>& vec) { stream->WriteSafe(vec.Data(), sizeof(T), 4); }

		inline void WriteFloat(const float& elm) { Write<float>(elm); }
		inline void WriteDouble(const double& elm) { Write<double>(elm); }
		inline void WriteInt64(const Int64& elm) { Write<Int64>(elm); }
		inline void WriteInt32(const Int32& elm) { Write<Int32>(elm); }
		inline void WriteInt16(const Int16& elm) { Write<Int16>(elm); }
		inline void WriteInt8(const Int8& elm) { Write<Int8>(elm); }
		inline void WriteUInt64(const UInt64& elm) { Write<UInt64>(elm); }
		inline void WriteUInt32(const UInt32& elm) { Write<UInt32>(elm); }
		inline void WriteUInt16(const UInt16& elm) { Write<UInt16>(elm); }
		inline void WriteUInt8(const UInt8& elm) { Write<UInt8>(elm); }
		inline void WriteBool(const bool& elm) { Write<UInt8>(elm? 1: 0); }

		void Write(const void* data, int elmSize, int elmCount);
		void WriteCStr(const char* str);

		void Write7BitEncodedInt(int value);
		
		/// .NET compatible string writer
		void WriteString(const String& str);
	};
}

#endif
