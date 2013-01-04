#ifndef __XLI_UTF16_STRING_H__
#define __XLI_UTF16_STRING_H__

#include <Xli/String.h>

namespace Xli
{
	/**
		\addtogroup Core
		@{
	*/

#ifdef WIN32
	typedef wchar_t Utf16;
#else
	typedef UInt16 Utf16;
#endif

	int Utf16StrLen(const Utf16* str);

	/** @} */

	/**
		\ingroup Core
	*/
	class Utf16String
	{
		friend class Unicode;
		
		Utf16* data;
		int length;

	public:
		Utf16String();
		Utf16String(const Utf16String& copy);
		Utf16String(const Utf16* str, int len);
		Utf16String(const Utf16* str);
		~Utf16String();

		static Utf16String Create(int len);

		int Length() const;

		Utf16& operator[] (int index);
		const Utf16& operator[] (int index) const;

		Utf16* Data();
		const Utf16* Data() const;

		Utf16String& operator = (const Utf16String& copy);
	};
}

#endif
