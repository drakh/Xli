#include <Xli/Exception.h>

namespace Xli
{
	Exception::Exception()
	{
		this->_func = "<unknown>";
		this->_line = 0;
	}

	Exception::Exception(const String& message)
	{
		this->_message = message;
		this->_func = "<unknown>";
		this->_line = 0;
	}

	Exception::Exception(const String& message, const String& func, int line)
	{
		this->_message = message;
		this->_func = func;
		this->_line = line;
	}

	const String& Exception::GetMessage() const
	{
		return _message;
	}

	const String& Exception::GetFunction() const
	{
		return _func;
	}

	int Exception::GetLine() const
	{
		return _line;
	}

	const String& Exception::ToString() const
	{
		return _message;
	}
}
