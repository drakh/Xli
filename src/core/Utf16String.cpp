#include <Xli/Utf16String.h>
#include <Xli/Exception.h>
#include <cstring>

namespace Xli
{
	int Utf16StrLen(const Utf16* str)
	{
		int len = 0;
		
		while (*str++) 
			len++;
		
		return len;
	}

	Utf16String::Utf16String()
	{
		data = 0;
		length = 0;
	}

	Utf16String::Utf16String(const Utf16String& copy)
	{
		this->length = copy.length;
		this->data = new Utf16[copy.length + 1];
		this->data[copy.length] = 0;
		memcpy(this->data, copy.data, copy.length * 2);
	}

	Utf16String& Utf16String::operator = (const Utf16String& copy)
	{
		if (this->data != 0) delete [] this->data;
		this->length = copy.length;
		this->data = new Utf16[copy.length + 1];
		this->data[copy.length] = 0;
		memcpy(this->data, copy.data, copy.length * 2);
		return *this;
	}

	Utf16String::Utf16String(const Utf16* str, int len)
	{
		this->length = len;
		this->data = new Utf16[len + 1];
		this->data[len] = 0;
		memcpy(this->data, str, len * 2);
	}

	Utf16String::Utf16String(const Utf16* str)
	{
		this->length = Utf16StrLen(str);
		this->data = new Utf16[this->length + 1];
		this->data[this->length] = 0;
		memcpy(this->data, str, this->length * 2);
	}

	Utf16String::~Utf16String()
	{
		if (data != 0) delete [] data;
	}

	Utf16String Utf16String::Create(int len)
	{
		Utf16String r;
		r.length = len;
		r.data = new Utf16[len + 1];
		r.data[len] = 0;
		return r;
	}
	
	int Utf16String::Length() const
	{
		return length;
	}

	Utf16& Utf16String::operator[] (int index)
	{
		return data[index];
	}
	
	const Utf16& Utf16String::operator[] (int index) const
	{
		return data[index];
	}

	Utf16* Utf16String::Data()
	{
		return data;
	}
	
	const Utf16* Utf16String::Data() const
	{
		return data;
	}

	Utf16String Utf16String::operator + (const Utf16String& str) const
	{
		Utf16String r = Utf16String::Create(length + str.length);
		memcpy(r.data, data, length * sizeof(Utf16));
		memcpy(r.data + length, str.data, str.length * sizeof(Utf16));
		return r;
	}
}
