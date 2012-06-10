#ifndef __XLI_BINARY_READER_H__
#define __XLI_BINARY_READER_H__

#include <Xli/Stream.h>
#include <Xli/Matrix4.h>

namespace Xli
{
	class BinaryReader: public StreamReader
	{
	public:
		BinaryReader(Stream* stream);
		virtual ~BinaryReader();

		void Read(void* dst, int elmSize, int elmCount);
		CharString ReadCStr(int len);

		template <typename T> void Read(T& elm)
		{
			stream->ReadSafe((void*)&elm, sizeof(T), 1);
		}

		template <typename T> void Read(Vector2t<T>& vec) { stream->ReadSafe(vec.Data(), sizeof(T), 2); }
		template <typename T> void Read(Vector3t<T>& vec) { stream->ReadSafe(vec.Data(), sizeof(T), 3); }
		template <typename T> void Read(Vector4t<T>& vec) { stream->ReadSafe(vec.Data(), sizeof(T), 4); }
		template <typename T> void Read(Matrix4t<T>& vec) { stream->ReadSafe(vec.Data(), sizeof(T), 16); }

		template <typename T> T Read()
		{
			T elm;
			Read(elm);
			return elm;
		}

		template <typename T, uint S> void ReadStringtData(Stringt<T, S>& str, int length)
		{
			str = Stringt<T, S>::Create(length);
			if (length) stream->ReadSafe((void*)str.Data(), sizeof(T), length);
		}

		template <typename T, int S> void ReadArrayData(Array<T, S>& a, int length)
		{
			a.Resize(length);
			if (length) stream->ReadSafe((void*)a.Data(), sizeof(T), length);
		}

		Buffer* ReadBuffer();
		Buffer* ReadAll();

    	int Read7BitEncodedInt();
		Utf16String ReadUtf8String(int len);

		/// .NET compatible string reader
		Utf16String ReadEncodedUtf16String() { return ReadUtf8String(Read7BitEncodedInt()); }

		inline float ReadFloat() { return Read<float>(); }
		inline double ReadDouble() { return Read<double>(); }
		inline Int64 ReadInt64() { return Read<Int64>(); }
		inline Int32 ReadInt32() { return Read<Int32>(); }
		inline Int16 ReadInt16() { return Read<Int16>(); }
		inline Int8 ReadInt8() { return Read<Int8>(); }
		inline UInt64 ReadUInt64() { return Read<UInt64>(); }
		inline UInt32 ReadUInt32() { return Read<UInt32>(); }
		inline UInt16 ReadUInt16() { return Read<UInt16>(); }
		inline UInt8 ReadUInt8() { return Read<UInt8>(); }
		inline bool ReadBool() { return Read<UInt8>() != 0; }

		template <typename T> Vector2t<T> ReadVector2t() { Vector2t<T> vec; Read(vec); return vec; }
		template <typename T> Vector3t<T> ReadVector3t() { Vector3t<T> vec; Read(vec); return vec; }
		template <typename T> Vector4t<T> ReadVector4t() { Vector4t<T> vec; Read(vec); return vec; }

		inline CharString ReadCharString(int length) { CharString str; ReadStringtData(str, length); return str; }
		inline Utf16String ReadUtf16String(int length) { Utf16String str; ReadStringtData(str, length); return str; }
		inline Utf32String ReadUtf32String(int length) { Utf32String str; ReadStringtData(str, length); return str; }
	};
}

#endif
