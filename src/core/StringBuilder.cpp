#include <Xli/StringBuilder.h>
#include <cstring>

namespace Xli
{
	StringBuilder::StringBuilder(int cap) 
	{ 
		buffer.Reserve(cap);
	}

	StringBuilder& StringBuilder::operator += (const String& text)
	{
		Append(text);
		return *this;
	}

	StringBuilder& StringBuilder::operator += (const char* str)
	{
		Append(str);
		return *this;
	}

	StringBuilder& StringBuilder::operator += (char c)
	{
		Append(c);
		return *this;
	}

	int StringBuilder::GetLength() const
	{ 
		return buffer.Length(); 
	}

	void StringBuilder::Append(const char* str)
	{
		buffer.Add(str, (int)strlen(str));
	}

	void StringBuilder::Append(const String& text)
	{
		buffer.Add(text.Data(), text.Length());
	}

	void StringBuilder::Append(char c)
	{
		buffer.Add(c);
	}

	void StringBuilder::AppendLine(const char* str)
	{
		Append(str);
		buffer.Add('\n');
	}

	void StringBuilder::AppendLine(const String& text)
	{
		Append(text);
		buffer.Add('\n');
	}

	void StringBuilder::AppendLine()
	{
		buffer.Add('\n');
	}

	void StringBuilder::AppendFormat(const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		Append(String::Format(format, argList));
		va_end(argList);
	}

	void StringBuilder::Clear()
	{
		buffer.Clear();
	}

	String StringBuilder::GetString() const
	{
		return String(buffer.Data(), buffer.Length());
	}

	StringBuilder::operator String() const
	{
		return String(buffer.Data(), buffer.Length());
	}
}
