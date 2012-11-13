#ifndef __XLI_STRING_DECL_H__
#define __XLI_STRING_DECL_H__

#include <Xli/BaseTypes.h>

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

namespace Xli
{
	typedef Stringt<char> CharString;
	typedef Stringt<Utf16Char> Utf16String;
	typedef Stringt<Utf32Char> Utf32String;
    
//#ifdef XLI_UNICODE
	typedef Utf16String String;
	typedef Utf16Char StringChar;
//#else
//	typedef CharString String;
//	typedef char StringChar;
//#endif

	class StringTools
	{
	public:
		template <typename T> static void Copy(T* dst, const T* src, uint len)
		{
			memcpy(dst, src, len * sizeof(T));
		}
		template <typename TDst, typename TSrc> static void Copy(TDst* dst, const TSrc* src, uint len)
		{
			if (sizeof(TDst) >= sizeof(TSrc))
			{
				while (len--) *dst++ = (TDst)*src++;
			}
			else
			{
				const uint u = (uint)(((UInt64)1 << (sizeof(TSrc) * 8)) - 1);
				
				while (len--)
				{
					*dst++ = (uint)*src > u? (TDst)u: (TDst)*src;
					src++;
				}
			}
		}
		template <typename T> static uint GetLength(const T* str)
		{
			uint len = 0;
			while ((*str++) != 0) len++;
			return len;
		}

		static int HexToInt(const char* s);
		static int ToInt(const char* s);
		static float ToFloat(const char* s);
		static double ToDouble(const char* s);
		static bool ToBool(const char* s);
		static CharString Format(const char* format, va_list argList);
		static uint GetFloatLength(const char* str);
	};

	template <typename T, uint TBufSize> class Stringt
	{
		T* data;
		T buf[TBufSize];
		uint length;

		void initLength(uint len)
		{
			if (len < TBufSize-1) data = buf;
			else data = new T[len+1];
			data[len] = 0;
			length = len;
		}
		template <typename TStr> inline void init(const TStr* str, const uint& len)
		{
			initLength(len);
			StringTools::Copy(data, str, len);
		}
		inline void init(int i)
		{
			char buf[32];
			sprintf_s(buf, 32, "%d", i);
			init(buf, StringTools::GetLength(buf));
		}
		inline void init(float f)
		{
			char buf[64];
			sprintf_s(buf, 64, "%f", f);
			init(buf, StringTools::GetFloatLength(buf));
		}
		inline void init(double d)
		{
			char buf[128];
			sprintf_s(buf, 128, "%f", d);
			init(buf, StringTools::GetFloatLength(buf));
		}
		inline void deinit()
		{
			if (length) if (data != buf) delete [] data;
		}

	public:
		Stringt()
		{
			length = 0;
			data = buf;
			buf[0] = 0;
		}
		Stringt(const Stringt& str)
		{
			init(str.data, str.length);
		}
		template <typename TStr> Stringt(const TStr* str)
		{
			init(str, StringTools::GetLength(str));
		}
		template <typename TStr> Stringt(const TStr* str, int len)
		{
			init(str, len);
		}
		template <typename TStr, uint TBuf> Stringt(const Stringt<TStr, TBuf>& str)
		{
			init(str.Data(), str.Length());
		}
		Stringt(char c)
		{
			init(&c, 1);
		}
		Stringt(Utf16Char c)
		{
			init(&c, 1);
		}
		Stringt(int i)
		{
			init(i);
		}
		Stringt(float f)
		{
			init(f);
		}
		Stringt(double d)
		{
			init(d);
		}
		~Stringt()
		{
			deinit();
		}

		T* DataCopy()
		{
			T* buf = new T[length+1];
			buf[length] = 0;
			StringTools::Copy(buf, data, length);
			return buf;
		}

		T* Data()
		{
			return data;
		}
		const T* Data() const
		{
			return data;
		}

		int Length() const
		{
			return length;
		}

		UInt32 Hash() const
		{
			// djb2 algorithm
			UInt32 hash = 5381;
			for (uint i = 0; i < length; i++)
			{
				hash = ((hash << 5) + hash) ^ (UInt32)data[i];
			}
			return hash;
		}

		T& Get(int index);

		T& operator [] (int index)
		{
			return Get(index);
		}

		const T& Get(int index) const;

		const T& operator [] (int index) const
		{
			return Get(index);
		}
		const T& First() const
		{
			return Get(0);
		}
		const T& Last() const
		{
			return Get(length-1);
		}

		int IndexOf(T c, int start) const
		{
			for (uint i = start; i < length; i++)
			{
				if (data[i] == c) return i;
			}
			return -1;
		}
		int IndexOf(T c) const
		{
			return IndexOf(c, 0);
		}

		int LastIndexOf(T c, int start) const;

		int LastIndexOf(T c) const
		{
			return LastIndexOf(c, length-1);
		}

		Stringt SubString(int start, int len) const;
		Stringt SubString(int start) const { return SubString(start, (int)length - start); }

		Stringt Trim() const
		{
			int start = 0;
			int end = length;

			while (start < end) if (isspace(data[start])) start++; else break;
			while (end > start) if (isspace(data[end-1])) end--; else break;

			return SubString(start, end-start);
		}
		Stringt Trim(T c) const
		{
			int start = 0;
			int end = length;

			while (start < end) if (data[start] == c) start++; else break;
			while (end > start) if (data[end-1] == c) end--; else break;

			return SubString(start, end-start);
		}

		Stringt ToLower() const
		{
			Stringt r(data, length);
			for (uint i = 0; i < length; i++) r.data[i] = tolower(r.data[i]);
			return r;
		}
		Stringt ToUpper() const
		{
			Stringt r(data, length);
			for (uint i = 0; i < length; i++) r.data[i] = toupper(r.data[i]);
			return r;
		}

		void Split(T c, Array<Stringt>& parts) const;
		static Stringt Join(T c, const Array<Stringt>& list);

		Stringt Head(T c) const
		{
			for (uint i = 0; i < length; i++)
			{
				if (data[i] == c) return SubString(0, i);
			}
			return Stringt();
		}
		Stringt Tail(T c) const
		{
			for (uint i = 0; i < length; i++)
			{
				if (data[i] == c) return SubString(i+1, length-i-1);
			}
			return *this;
		}

		bool StartsWith(const Stringt& str) const
		{
			if (str.length > length) return false;
			for (uint i = 0; i < str.length; i++)
			{
				if (str.data[i] != data[i]) return false;
			}
			return true;
		}
		bool EndsWith(const Stringt& str) const
		{
			if (str.length > length) return false;
			for (uint i = length-str.length; i < length; i++)
			{
				if (str.data[i-length+str.length] != data[i]) return false;
			}
			return true;
		}


		static Stringt FromInt(int i)
		{
			return i;
		}
		static Stringt FromFloat(float f)
		{
			return f;
		}
		static Stringt FromDouble(double d)
		{
			return d;
		}
		static Stringt FromChar(char c)
		{
			return c;
		}
		static Stringt FromChar(Utf16Char c)
		{
			return c;
		}
		static Stringt Create(uint length)
		{
			Stringt r;
			r.initLength(length);
			return r;
		}
		static Stringt HexFromInt(int h)
		{
			char buf[32];
			sprintf_s(buf, 32, "0x%x", h);
			return buf;
		}
		static Stringt FromBool(bool b)
		{
			return b? "true": "false";
		}
		static Stringt Format(const char* format, ...)
		{
			va_list argList;
			va_start(argList, format);
			Stringt s = StringTools::Format(format, argList);
			va_end(argList);
			return s;
		}

		inline int HexToInt() const
		{
			CharString ascii(*this);
			return Xli::StringTools::HexToInt(ascii.Data());
		}
		inline int ToInt() const
		{
			CharString ascii(*this);
			return Xli::StringTools::ToInt(ascii.Data());
		}
		inline float ToFloat() const
		{
			CharString ascii(*this);
			return Xli::StringTools::ToFloat(ascii.Data());
		}
		inline double ToDouble() const
		{
			CharString ascii(*this);
			return Xli::StringTools::ToDouble(ascii.Data());
		}
		bool ToBool() const
		{
			CharString ascii(*this);
			return Xli::StringTools::ToBool(ascii.Data());
		}

		template <typename TStr> bool Equals(const TStr* str, unsigned int len) const
		{
			if (length != len) return false;

			for (uint i = 0; i < length; i++)
			{
				if ((int)data[i] != (int)str[i]) return false;
			}

			return true;
		}
		template <typename TStr> inline bool Equals(const TStr* str) const
		{
			return Equals(str, StringTools::GetLength(str));
		}
		inline bool Equals(const Stringt& str) const
		{
			return Equals(str.data, str.length);
		}
		template <typename TStr, uint TBuf> inline bool Equals(const Stringt<TStr, TBuf>& str) const
		{
			return Equals(str.Data(), str.Length());
		}


		template <typename TStr> int CompareTo(const TStr* str, unsigned int len) const
		{
			unsigned int l = length;
			if (len < l) l = len;

			for (unsigned int i = 0; i < l; i++)
			{
				if ((int)data[i] == (int)str[i]) continue;
				else if ((int)data[i] < (int)str[i]) return -1;
				else return 1;
			}

			if (length == len) return 0;
			else if (length < len) return -1;
			return 1;
		}
		template <typename TStr> inline int CompareTo(const TStr* str) const
		{
			return CompareTo(str, StringTools::GetLength(str));
		}
		inline int CompareTo(const Stringt& str) const
		{
			return CompareTo(str.data, str.length);
		}
		template <typename TStr, uint TBuf> inline int CompareTo(const Stringt<TStr, TBuf>& str) const
		{
			return CompareTo(str.Data(), str.Length());
		}


		template <typename TStr> Stringt Add(const TStr* str, int len) const
		{
			Stringt r;
			r.initLength(length+len);
			StringTools::Copy(r.data, data, length);
			StringTools::Copy(r.data+length, str, len);
			return r;
		}
		template <typename TStr> inline Stringt Add(const TStr* str) const
		{
			return Add(str, StringTools::GetLength(str));
		}
		inline Stringt Add(const Stringt& str) const
		{
			return Add(str.data, str.length);
		}
		template <typename TStr, uint TBuf> inline Stringt Add(const Stringt<TStr, TBuf>& str) const
		{
			return Add(str.Data(), str.Length());
		}

		inline bool operator == (const Stringt& str) const
		{
			return Equals(str);
		}
		template <typename TStr> inline bool operator == (const TStr* str) const
		{
			return Equals(str);
		}
		template <typename TStr, uint TBuf> inline bool operator == (const Stringt<TStr, TBuf>& str) const
		{
			return Equals(str);
		}

		inline bool operator != (const Stringt& str) const
		{
			return !Equals(str);
		}
		template <typename TStr> inline bool operator != (const TStr* str) const
		{
			return !Equals(str);
		}
		template <typename TStr, uint TBuf> inline bool operator != (const Stringt<TStr, TBuf>& str) const
		{
			return !Equals(str);
		}

		inline Stringt operator + (const Stringt& str) const
		{
			return Add(str);
		}
		template <typename TStr> inline Stringt operator + (const TStr* str) const
		{
			return Add(str);
		}
		template <typename TStr, uint TBuf> inline Stringt operator + (const Stringt<TStr, TBuf>& str) const
		{
			return Add(str);
		}

		inline bool operator < (const Stringt& str) const
		{
			return CompareTo(str) < 0;
		}
		template <typename TStr> inline bool operator < (const TStr* str) const
		{
			return CompareTo(str) < 0;
		}
		template <typename TStr, uint TBuf> inline bool operator < (const Stringt<TStr, TBuf>& str) const
		{
			return CompareTo(str) < 0;
		}

		inline bool operator <= (const Stringt& str) const
		{
			return CompareTo(str) <= 0;
		}
		template <typename TStr> inline bool operator <= (const TStr* str) const
		{
			return CompareTo(str) <= 0;
		}
		template <typename TStr, uint TBuf> inline bool operator <= (const Stringt<TStr, TBuf>& str) const
		{
			return CompareTo(str) <= 0;
		}

		inline bool operator > (const Stringt& str) const
		{
			return CompareTo(str) > 0;
		}
		template <typename TStr> inline bool operator > (const TStr* str) const
		{
			return CompareTo(str) > 0;
		}
		template <typename TStr, uint TBuf> inline bool operator > (const Stringt<TStr, TBuf>& str) const
		{
			return CompareTo(str) > 0;
		}

		inline bool operator >= (const Stringt& str) const
		{
			return CompareTo(str) >= 0;
		}
		template <typename TStr> inline bool operator >= (const TStr* str) const
		{
			return CompareTo(str) >= 0;
		}
		template <typename TStr, uint TBuf> inline bool operator >= (const Stringt<TStr, TBuf>& str) const
		{
			return CompareTo(str) >= 0;
		}


		Stringt& operator = (const Stringt& str)
		{
			if (this == &str) return *this;
			deinit();
			init(str.data, str.length);
			return *this;
		}
		template <typename TStr, uint TBuf> Stringt& operator = (const Stringt<TStr, TBuf>& str)
		{
			if (this == (Stringt*)&str) return *this;
			deinit();
			init(str.Data(), str.Length());
			return *this;
		}
		template <typename TStr> Stringt& operator = (const TStr* str)
		{
			if (this == (Stringt*)&str) return *this;
			deinit();
			init(str, StringTools::GetLength(str));
			return *this;
		}
		Stringt& operator = (char c)
		{
			deinit();
			init(c);
			return *this;
		}
		Stringt& operator = (Utf16Char c)
		{
			deinit();
			init(c);
			return *this;
		}
		Stringt& operator = (int i)
		{
			deinit();
			init(i);
			return *this;
		}
		Stringt& operator = (float f)
		{
			deinit();
			init(f);
			return *this;
		}
		Stringt& operator = (double d)
		{
			deinit();
			init(d);
			return *this;
		}
	};
}

template<typename TStr, uint TBuf> Xli::Stringt<TStr, TBuf> operator + (const char* a, const Xli::Stringt<TStr, TBuf>& b)
{
	uint len = Xli::StringTools::GetLength(a);
	Xli::Stringt<TStr, TBuf> r = Xli::Stringt<TStr, TBuf>::Create(len + b.Length());
	Xli::StringTools::Copy(r.Data(), a, len);
	Xli::StringTools::Copy(r.Data() + len, b.Data(), b.Length());
	return r;
}


#endif
