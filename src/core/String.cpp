#include <Xli/String.h>
#include <Xli/Array.h>
#include <Xli/Exception.h>
#include <Xli/Hash.h>
#include <cctype>
#include <cstdio>
#include <cstring>

namespace Xli
{
	static int GetFloatLength(const char* str)
	{
		int len = (int)strlen(str);

		while (len > 1 && (str[len - 1] == '0' || str[len - 1] == '.')) 
			len--;

		return len;
	}

	void String::initLength(int len)
	{
		data = len < BufSize - 1 ? buf : new char[len + 1];
		data[len] = 0;
		length = len;
	}

	void String::init(const char* str, int len)
	{
		initLength(len);
		memcpy(data, str, len);
	}

	void String::init(int i)
	{
		char buf[32];
		sprintf_s(buf, 32, "%d", i);
		init(buf, (int)strlen(buf));
	}

	void String::init(float f)
	{
		char buf[64];
		sprintf_s(buf, 64, "%f", f);
		init(buf, GetFloatLength(buf));
	}

	void String::init(double d)
	{
		char buf[128];
		sprintf_s(buf, 128, "%f", d);
		init(buf, GetFloatLength(buf));
	}

	void String::init(bool b)
	{
		if (b)
			init("true", 4);
		else
			init("false", 5);
	}

	void String::deinit()
	{
		if (length && data != buf) 
			delete [] data;
	}

	String::String()
	{
		length = 0;
		data = buf;
		buf[0] = 0;
	}

	String::String(const String& str)
	{
		init(str.data, str.length);
	}

	String::String(const char* str)
	{
		init(str, (int)strlen(str));
	}

	String::String(const char* str, int len)
	{
		init(str, len);
	}

	String::String(char c)
	{
		init(&c, 1);
	}

	String::String(int i)
	{
		init(i);
	}

	String::String(float f)
	{
		init(f);
	}

	String::String(double d)
	{
		init(d);
	}

	String::~String()
	{
		deinit();
	}

	char* String::DataCopy()
	{
		char* buf = new char[length + 1];
		buf[length] = 0;
		memcpy(buf, data, length);
		return buf;
	}

	char* String::Data()
	{
		return data;
	}

	const char* String::Data() const
	{
		return data;
	}

	int String::Length() const
	{
		return length;
	}

	UInt32 Hash(const String& str)
	{
		return Xli::Hash((const UInt8*)str.Data(), str.Length());
	}

	char& String::operator [] (int index)
	{
#ifdef XLI_RANGE_CHECK
		if (index >= length)
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

		return data[index];
	}

	const char& String::operator [] (int index) const
	{
#ifdef XLI_RANGE_CHECK
		if (index >= length)
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

		return data[index];
	}

	const char& String::First() const
	{
		return (*this)[0];
	}

	const char& String::Last() const
	{
		return (*this)[length - 1];
	}

	int String::IndexOf(char c, int start) const
	{
		for (int i = start; i < length; i++)
			if (data[i] == c) 
				return i;

		return -1;
	}

	int String::IndexOf(char c) const
	{
		return IndexOf(c, 0);
	}

	int String::LastIndexOf(char c, int start) const
	{
#ifdef XLI_RANGE_CHECK
		if (start >= length)
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

		for (int i = start; i >= 0; i--)
			if (data[i] == c) 
				return i;

		return -1;
	}

	int String::LastIndexOf(char c) const
	{
		return LastIndexOf(c, length - 1);
	}

	String String::Substring(int start, int len) const
	{
#ifdef XLI_RANGE_CHECK
		if (start + len > length || start < 0)
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

		return String(data + start, len);
	}

	String String::Substring(int start) const 
	{ 
		return Substring(start, length - start); 
	}

	String String::Trim() const
	{
		int start = 0;
		int end = length;

		while (start < end) 
			if (isspace((int)(unsigned char)data[start])) 
				start++; 
			else 
				break;

		while (end > start) 
			if (isspace((int)(unsigned char)data[end - 1])) 
				end--; 
			else 
				break;

		return Substring(start, end - start);
	}
		
	String String::Trim(char c) const
	{
		int start = 0;
		int end = length;

		while (start < end) 
			if (data[start] == c) 
				start++; 
			else 
				break;

		while (end > start) 
			if (data[end - 1] == c) 
				end--; 
			else 
				break;

		return Substring(start, end - start);
	}

	String String::ToLower() const
	{
		String r(data, length);

		for (int i = 0; i < length; i++) 
			r.data[i] = (char)(unsigned char)tolower((int)(unsigned char)r.data[i]);

		return r;
	}

	String String::ToUpper() const
	{
		String r(data, length);

		for (int i = 0; i < length; i++) 
			r.data[i] = (char)(unsigned char)toupper((int)(unsigned char)r.data[i]);

		return r;
	}

	void String::Split(char c, Array<String>& parts) const
	{
		int start = 0;

		for (int i = 0; i < length; i++)
		{
			if (data[i] == c)
			{
				parts.Add(Substring(start, i - start));
				start = i+1;
			}
		}

		if (start < length)
		{
			parts.Add(Substring(start, length - start));
		}
	}

	String String::Join(char c, const Array<String>& list)
	{
		String r;
		r.length = -1;

		for (int i = 0; i < list.Length(); i++) 
			r.length += list[i].Length() + 1;

		if (r.length == -1) 
			return String();

		r.data = new char[r.length + 1];
		int p = 0;

		for (int i = 0; i < list.Length(); i++)
		{
			memcpy(r.data + p, list[i].Data(), list[i].Length());
			p += list[i].Length();
			r.data[p++] = c;
		}

		r.data[r.length] = 0;
		return r;
	}

	bool String::StartsWith(const String& str) const
	{
		if (str.length > length) 
			return false;

		for (int i = 0; i < str.length; i++)
			if (str.data[i] != data[i]) 
				return false;

		return true;
	}

	bool String::EndsWith(const String& str) const
	{
		if (str.length > length)
			return false;

		for (int i = length - str.length; i < length; i++)
			if (str.data[i - length + str.length] != data[i]) 
				return false;

		return true;
	}

	String String::Create(int length)
	{
		String r;
		r.initLength(length);
		return r;
	}

	String String::HexFromInt(int h)
	{
		char buf[32];
		sprintf_s(buf, 32, "0x%x", h);
		return buf;
	}

	String String::FromBool(bool b)
	{
		String r;
		r.init(b);
		return r;
	}

	String String::Format(const char* format, va_list argList)
	{
		// TODO: Verify code

		const int BufferSize = 2048;

		char buf[BufferSize];
		int i = vsnprintf_s(buf, BufferSize, _TRUNCATE, format, argList);

		if (i >= BufferSize || i < 0) 
			XLI_THROW_INVALID_FORMAT("Result too long");

		return String(buf, i);
	}

	String String::Format(const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		String result = Format(format, argList);
		va_end(argList);
		return result;
	}

	int String::HexToInt() const
	{
		int i;
		if (sscanf_s(data, "%x", &i) == 1) return i;
		if (sscanf_s(data, "%X", &i) == 1) return i;
		XLI_THROW_INVALID_FORMAT("Unable to convert string(hex) to integer");
	}

	int String::ToInt() const
	{
		int i;
		if (sscanf_s(data, "%d", &i) == 1) return i;
		XLI_THROW_INVALID_FORMAT("Unable to convert string to int");
	}

	float String::ToFloat() const
	{
		float i;
		if (sscanf_s(data, "%f", &i) == 1) return i;
		if (sscanf_s(data, "%e", &i) == 1) return i;
		if (sscanf_s(data, "%E", &i) == 1) return i;
		XLI_THROW_INVALID_FORMAT("Unable to convert string to float");
	}

	double String::ToDouble() const
	{
		double i;
		if (sscanf_s(data, "%lf", &i) == 1) return i;
		if (sscanf_s(data, "%le", &i) == 1) return i;
		if (sscanf_s(data, "%lE", &i) == 1) return i;
		XLI_THROW_INVALID_FORMAT("Unable to convert string to double");
	}

	bool String::ToBool() const
	{
		if (!strcmp(data, "true")) return true;
		if (!strcmp(data, "false")) return false;
		if (!strcmp(data, "TRUE")) return true;
		if (!strcmp(data, "FALSE")) return false;
		if (!strcmp(data, "True")) return true;
		if (!strcmp(data, "False")) return false;
		XLI_THROW_INVALID_FORMAT("Unable to convert string to bool");
	}

	bool String::Equals(const char* str, int len) const
	{
		if (length != len)
			return false;

		for (int i = 0; i < length; i++)
			if (data[i] != str[i]) 
				return false;

		return true;
	}

	bool String::Equals(const char* str) const
	{
		return Equals(str, (int)strlen(str));
	}

	bool String::Equals(const String& str) const
	{
		return Equals(str.data, str.length);
	}

	int String::CompareTo(const char* str, int len) const
	{
		int l = length;
		if (len < l) l = len;

		for (int i = 0; i < l; i++)
		{
			if (data[i] == str[i]) 
				continue;
			else if (data[i] < str[i]) 
				return -1;

			return 1;
		}

		if (length == len) 
			return 0;
		else if (length < len) 
			return -1;
		
		return 1;
	}

	int String::CompareTo(const char* str) const
	{
		return CompareTo(str, (int)strlen(str));
	}

	int String::CompareTo(const String& str) const
	{
		return CompareTo(str.data, str.length);
	}

	String String::Add(const char* str, int len) const
	{
		String r;
		r.initLength(length + len);
		memcpy(r.data, data, length);
		memcpy(r.data + length, str, len);
		return r;
	}

	String String::Add(const char* str) const
	{
		return Add(str, (int)strlen(str));
	}

	String String::Add(const String& str) const
	{
		return Add(str.data, str.length);
	}

    void String::Append(const char* str, int len)
    {
	if (!len) return;
        String temp = Add(str, len);

	if (temp.data != temp.buf)
    	{	
		// Hack: 
		// 1. Swap data with temp 
		// 2. Let temp free old data when it goes out of scope, as long as old data does not point to buf which is stack allocated
		char* newData = temp.data;
		temp.data = data != buf ? data : temp.buf;
		data = newData;
		length = temp.length;
    	}
	else
    	{
		// If hack fails we must reallocate the data
		deinit();
		init(temp.data, temp.length);
	}		
    }
    
    void String::Append(const char* str)
    {
        Append(str, (int)strlen(str));
    }
    
    void String::Append(const String& str)
    {
        Append(str.data, str.length);
    }
    
	bool String::operator == (const String& str) const
	{
		return Equals(str);
	}

	bool String::operator == (const char* str) const
	{
		return Equals(str);
	}

	bool String::operator != (const String& str) const
	{
		return !Equals(str);
	}

	bool String::operator != (const char* str) const
	{
		return !Equals(str);
	}

	String String::operator + (const String& str) const
	{
		return Add(str);
	}

	String String::operator + (const char* str) const
	{
		return Add(str);
	}

	bool String::operator < (const String& str) const
	{
		return CompareTo(str) < 0;
	}

	bool String::operator < (const char* str) const
	{
		return CompareTo(str) < 0;
	}

	bool String::operator <= (const String& str) const
	{
		return CompareTo(str) <= 0;
	}

	bool String::operator <= (const char* str) const
	{
		return CompareTo(str) <= 0;
	}

	bool String::operator > (const String& str) const
	{
		return CompareTo(str) > 0;
	}

	bool String::operator > (const char* str) const
	{
		return CompareTo(str) > 0;
	}

	bool String::operator >= (const String& str) const
	{
		return CompareTo(str) >= 0;
	}

	bool String::operator >= (const char* str) const
	{
		return CompareTo(str) >= 0;
	}

    String& String::operator += (const String& str)
    {
        Append(str);
        return *this;
    }
    
    String& String::operator += (const char* str)
    {
        Append(str);
        return *this;
    }
    
	String& String::operator = (const String& str)
	{
		if (this == &str) return *this;
		deinit();
		init(str.data, str.length);
		return *this;
	}

	String& String::operator = (const char* str)
	{
		if (this->data == str) return *this;
		deinit();
		init(str, (int)strlen(str));
		return *this;
	}

	String& String::operator = (char c)
	{
		deinit();
		init(c);
		return *this;
	}

	String& String::operator = (int i)
	{
		deinit();
		init(i);
		return *this;
	}

	String& String::operator = (float f)
	{
		deinit();
		init(f);
		return *this;
	}

	String& String::operator = (double d)
	{
		deinit();
		init(d);
		return *this;
	}
}

Xli::String operator + (const char* a, const Xli::String& b)
{
	int len = (int)strlen(a);
	Xli::String r = Xli::String::Create(len + b.Length());
	memcpy(r.Data(), a, len);
	memcpy(r.Data() + len, b.Data(), b.Length());
	return r;
}
