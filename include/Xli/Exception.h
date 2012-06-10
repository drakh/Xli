#ifndef __XLI_EXCEPTION_H__
#define __XLI_EXCEPTION_H__

#include <Xli/Config.h>
#include <Xli/StringDecl.h>

namespace Xli
{
	class Exception
	{
	public:
		String _message;
		String _func;
		int _line;

		Exception()
		{
			this->_func = "<unknown>";
			this->_line = 0;
		}
		Exception(const String& message)
		{
			this->_message = message;
			this->_func = "<unknown>";
			this->_line = 0;
		}
		Exception(const String& message, const String& func, int line)
		{
			this->_message = message;
			this->_func = func;
			this->_line = line;
		}

		const String& GetMessage() const { return _message; }
		const String& GetFunction() const { return _func; }
		int GetLine() const { return _line; }

		const String& ToString() const { return _message; }
	};
}


#define XLI_DECLARE_EXCEPTION_B(BaseClass, ClassName, Message) \
	class ClassName: public BaseClass \
	{ \
	public: \
		ClassName(const Xli::String& msg) { _message = Xli::String(Message) + ": " + msg; } \
		ClassName() { _message = Message; } \
	}

#define XLI_DECLARE_EXCEPTION(ClassName, Message)	XLI_DECLARE_EXCEPTION_B(Xli::Exception, ClassName, Message)

#define XLI_THROW_CM(ClassName, Message)			do { ClassName e(Message); e._func = XLI_FUNC; e._line = __LINE__; throw e; } while(0)
#define XLI_THROW_C(ClassName)						do { ClassName e; e._func = XLI_FUNC; e._line = __LINE__; throw e; } while(0)
#define XLI_BREAK_THROW_CM(ClassName, Message)		do { XLI_DEBUG_BREAK; ClassName e(Message); e._func = XLI_FUNC; e._line = __LINE__; throw e; } while(0)
#define XLI_BREAK_THROW_C(ClassName)				do { XLI_DEBUG_BREAK; ClassName e; e._func = XLI_FUNC; e._line = __LINE__; throw e; } while(0)

#define XLI_THROW(Message)							XLI_THROW_CM(Xli::Exception, Message)
#define XLI_BREAK_THROW(Message)					XLI_BREAK_THROW_CM(Xli::Exception, Message)


namespace Xli
{
	XLI_DECLARE_EXCEPTION(IndexOutOfBoundsException, "Index out of bounds");
	XLI_DECLARE_EXCEPTION(NullPointerException, "Null pointer access attempt");
	XLI_DECLARE_EXCEPTION(BadDeleteException, "Attemt to delete object with refcount > 1");
	XLI_DECLARE_EXCEPTION(FormatException, "Invalid format");
	XLI_DECLARE_EXCEPTION(ArgumentException, "Illegal argument");
	XLI_DECLARE_EXCEPTION(NotSupportedException, "Not supported");

	class FileException: public Exception {};
	class StreamException: public Exception {};

	XLI_DECLARE_EXCEPTION_B(StreamException, StreamClosedException, "Stream has closed");
	XLI_DECLARE_EXCEPTION_B(StreamException, StreamCantReadException, "Stream not readable");
	XLI_DECLARE_EXCEPTION_B(StreamException, StreamCantWriteException, "Stream not writeable");
	XLI_DECLARE_EXCEPTION_B(StreamException, StreamCantSeekException, "Stream not seekable");
	XLI_DECLARE_EXCEPTION_B(StreamException, EndOfStreamException, "End of Stream reached");
	XLI_DECLARE_EXCEPTION_B(FileException, FileCantOpenException, "Couldn't open file");
	XLI_DECLARE_EXCEPTION_B(FileException, FileNotFoundException, "File not found");
}

#define XLI_THROW_INDEX_OUT_OF_BOUNDS		XLI_BREAK_THROW_C(Xli::IndexOutOfBoundsException)
#define XLI_THROW_NULL_POINTER				XLI_BREAK_THROW_C(Xli::NullPointerException)
#define XLI_THROW_BAD_DELETE				XLI_BREAK_THROW_C(Xli::BadDeleteException)
#define XLI_THROW_INVALID_FORMAT(msg)		XLI_THROW_CM(Xli::FormatException, msg)
#define XLI_THROW_INVALID_ARGUMENT(msg)		XLI_THROW_CM(Xli::ArgumentException, msg)
#define XLI_THROW_NOT_SUPPORTED(msg)		XLI_THROW_CM(Xli::NotSupportedException, msg)

#define XLI_THROW_STREAM_CLOSED				XLI_THROW_C(Xli::StreamClosedException)
#define XLI_THROW_STREAM_CANT_READ			XLI_THROW_C(Xli::StreamCantReadException)
#define XLI_THROW_STREAM_CANT_WRITE			XLI_THROW_C(Xli::StreamCantWriteException)
#define XLI_THROW_STREAM_CANT_SEEK			XLI_THROW_C(Xli::StreamCantSeekException)
#define XLI_THROW_END_OF_STREAM				XLI_THROW_C(Xli::EndOfStreamException)
#define XLI_THROW_CANT_OPEN_FILE(msg)		XLI_THROW_CM(Xli::FileCantOpenException, msg)
#define XLI_THROW_FILE_NOT_FOUND(msg)		XLI_THROW_CM(Xli::FileNotFoundException, msg)

#define XLI_IGNORE_EXCEPTION(x)				do { try { x; } catch (const Exception&) {} } while(0)
#define XLI_WARN_ON_EXCEPTION(x)			do { try { x; } catch (const Xli::Exception& e) { Xli::ErrorPrintLine((Xli::String)"WARNING: " + XLI_FUNC + "(" + __LINE__ + "): " + e.ToString()); } } while (0)

#endif
