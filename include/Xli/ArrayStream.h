#ifndef __XLI_ARRAY_STREAM_H__
#define __XLI_ARRAY_STREAM_H__

#include <Xli/Stream.h>

namespace Xli
{
	/**
		\ingroup IO
	*/
	class ArrayStream: public Stream
	{
	protected:
		Array<UInt8> buf;

	public:
		ArrayStream(int initCapacity = 1024);

		virtual bool CanWrite() const;
		virtual int Write(const void* src, int elmSize, int elmCount);

		virtual int GetLength() const;

		const UInt8* GetData() const;
		Buffer* CopyData() const;

		void Clear();
	};
}

#endif
