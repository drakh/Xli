#ifndef __XLI_BINARY_WRITER_H__
#define __XLI_BINARY_WRITER_H__

#include <Xli/Stream.h>
#include <Xli/Matrix4.h>

namespace Xli
{
	class BinaryWriter: public StreamWriter
	{
	public:
		BinaryWriter(Stream* stream);
		virtual ~BinaryWriter();

		void Write(const void* data, int elmSize, int elmCount);
		void WriteCStr(const char* str);

		template <typename T> void Write(const T& elm)
		{
			stream->WriteSafe((const void*)&elm, sizeof(T), 1);
		}

		template <typename T> void Write(const Vector2t<T>& vec) { stream->WriteSafe(vec.Data(), sizeof(T), 2); }
		template <typename T> void Write(const Vector3t<T>& vec) { stream->WriteSafe(vec.Data(), sizeof(T), 3); }
		template <typename T> void Write(const Vector4t<T>& vec) { stream->WriteSafe(vec.Data(), sizeof(T), 4); }
		template <typename T> void Write(const Matrix4t<T>& vec) { stream->WriteSafe(vec.Data(), sizeof(T), 16); }

		void Write7BitEncodedInt(int value);
		void WriteUtf8StringData(const Utf16String& str);
		
		/// .NET compatible string writer
		void WriteEncodedUtf16String(const Utf16String& str);

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

		template <typename T, uint S> void WriteStringtData(const Stringt<T, S>& str)
		{
			UInt32 l = (UInt32)str.Length();
			if (l) stream->WriteSafe((const void*)str.Data(), sizeof(T), (int)l);
		}

		template <typename T, int S> void WriteArrayData(const Array<T, S>& array)
		{
			UInt32 l = array.Length();
			if (l) stream->WriteSafe((const void*)array.Data(), sizeof(T), (int)l);
		}

		void WriteBufferData(DataAccessor* buf)
		{
			UInt32 l = buf->GetSizeInBytes();
			if (l) stream->WriteSafe(buf->GetData(), 1, buf->GetSizeInBytes());
		}

		inline void WriteCharStringData(const CharString& str) { WriteStringtData(str); }
		inline void WriteUtf16StringData(const Utf16String& str) { WriteStringtData(str); }
		inline void WriteUtf32StringData(const Utf32String& str) { WriteStringtData(str); }
	};
}

#endif
