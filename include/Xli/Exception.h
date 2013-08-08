#ifndef __XLI_EXCEPTION_H__
#define __XLI_EXCEPTION_H__

#include <Xli/Config.h>
#include <Xli/String.h>

namespace Xli
{
	/**
		\ingroup Xli
	*/
	class Exception
	{
	public:
		String _message;
		String _func;
		int _line;

		Exception();
		Exception(const String& message);
		Exception(const String& message, const String& func, int line);

		const String& GetMessage() const;
		const String& GetFunction() const;
		int GetLine() const;

		const String& ToString() const;
	};
}

/**
	\addtogroup Xli
	@{
*/

#define XLI_DECLARE_EXCEPTION_SUB(BaseClass, ClassName, Message) \
	class ClassName: public BaseClass \
	{ \
	public: \
		ClassName(const Xli::String& msg) { _message = ::Xli::String(Message) + ": " + msg; } \
		ClassName() { _message = Message; } \
	}

#define XLI_DECLARE_EXCEPTION(ClassName, Message)	XLI_DECLARE_EXCEPTION_SUB(Xli::Exception, ClassName, Message)

#define XLI_THROW_EX(ClassName, Message)			do { ClassName e(Message); e._func = XLI_FUNCTION; e._line = XLI_LINE; throw e; } while(0)
#define XLI_THROW_E(ClassName)						do { ClassName e; e._func = XLI_FUNCTION; e._line = XLI_LINE; throw e; } while(0)
#define XLI_BREAK_THROW_EX(ClassName, Message)		do { XLI_DEBUG_BREAK; ClassName e(Message); e._func = XLI_FUNCTION; e._line = XLI_LINE; throw e; } while(0)
#define XLI_BREAK_THROW_E(ClassName)				do { XLI_DEBUG_BREAK; ClassName e; e._func = XLI_FUNCTION; e._line = XLI_LINE; throw e; } while(0)

#define XLI_THROW(Message)							XLI_THROW_EX(::Xli::Exception, Message)
#define XLI_BREAK_THROW(Message)					XLI_BREAK_THROW_EX(::Xli::Exception, Message)

/** @} */

namespace Xli
{
	/** \ingroup Xli */
	XLI_DECLARE_EXCEPTION(IndexOutOfBoundsException, "Index out of bounds");
	/** \ingroup Xli */
	XLI_DECLARE_EXCEPTION(NullPointerException, "Null pointer access attempt");
	/** \ingroup Xli */
	XLI_DECLARE_EXCEPTION(BadDeleteException, "Attemt to delete object with refcount > 1");
	/** \ingroup Xli */
	XLI_DECLARE_EXCEPTION(FormatException, "Invalid format");
	/** \ingroup Xli */
	XLI_DECLARE_EXCEPTION(ArgumentException, "Illegal argument");
	/** \ingroup Xli */
	XLI_DECLARE_EXCEPTION(NotSupportedException, "Not supported");

	/** \ingroup XliCoreIO */
	class FileException: public Exception {};
	/** \ingroup XliCoreIO */
	class StreamException: public Exception {};

	/** \ingroup XliCoreIO */
	XLI_DECLARE_EXCEPTION_SUB(StreamException, StreamClosedException, "Stream has closed");
	/** \ingroup XliCoreIO */
	XLI_DECLARE_EXCEPTION_SUB(StreamException, StreamCantReadException, "Stream not readable");
	/** \ingroup XliCoreIO */
	XLI_DECLARE_EXCEPTION_SUB(StreamException, StreamCantWriteException, "Stream not writeable");
	/** \ingroup XliCoreIO */
	XLI_DECLARE_EXCEPTION_SUB(StreamException, StreamCantSeekException, "Stream not seekable");
	/** \ingroup XliCoreIO */
	XLI_DECLARE_EXCEPTION_SUB(StreamException, EndOfStreamException, "End of Stream reached");
	/** \ingroup XliCoreIO */
	XLI_DECLARE_EXCEPTION_SUB(FileException, FileCantOpenException, "Couldn't open file");
	/** \ingroup XliCoreIO */
	XLI_DECLARE_EXCEPTION_SUB(FileException, FileNotFoundException, "File not found");
}

/**
	\addtogroup Xli
	@{
*/

#define XLI_THROW_INDEX_OUT_OF_BOUNDS		XLI_BREAK_THROW_E(::Xli::IndexOutOfBoundsException)
#define XLI_THROW_NULL_POINTER				XLI_BREAK_THROW_E(::Xli::NullPointerException)
#define XLI_THROW_BAD_DELETE				XLI_BREAK_THROW_E(::Xli::BadDeleteException)
#define XLI_THROW_INVALID_FORMAT(msg)		XLI_THROW_EX(::Xli::FormatException, msg)
#define XLI_THROW_INVALID_ARGUMENT(msg)		XLI_THROW_EX(::Xli::ArgumentException, msg)
#define XLI_THROW_NOT_SUPPORTED(msg)		XLI_THROW_EX(::Xli::NotSupportedException, msg)

#define XLI_THROW_STREAM_CLOSED				XLI_THROW_E(::Xli::StreamClosedException)
#define XLI_THROW_STREAM_CANT_READ			XLI_THROW_E(::Xli::StreamCantReadException)
#define XLI_THROW_STREAM_CANT_WRITE			XLI_THROW_E(::Xli::StreamCantWriteException)
#define XLI_THROW_STREAM_CANT_SEEK			XLI_THROW_E(::Xli::StreamCantSeekException)
#define XLI_THROW_END_OF_STREAM				XLI_THROW_E(::Xli::EndOfStreamException)
#define XLI_THROW_CANT_OPEN_FILE(msg)		XLI_THROW_EX(::Xli::FileCantOpenException, msg)
#define XLI_THROW_FILE_NOT_FOUND(msg)		XLI_THROW_EX(::Xli::FileNotFoundException, msg)

#define XLI_IGNORE_EXCEPTION(x)				do { try { x; } catch (const ::Xli::Exception&) { } } while(0)
#define XLI_WARN_ON_EXCEPTION(x)			do { try { x; } catch (const ::Xli::Exception& e) { ::Xli::ErrorPrintLine((::Xli::String)"WARNING: " + XLI_FUNCTION + "(" + XLI_LINE + "): " + e.ToString()); } } while (0)

/** @} */

#endif
