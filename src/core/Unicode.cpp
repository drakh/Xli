#include <Xli/Unicode.h>
#include "3rdparty/ConvertUTF.h"

namespace Xli
{
	static const ConversionFlags Flags = strictConversion;
	static const bool ErrorCheck = true;
	
	bool Unicode::IsLegalUtf8(const CharString& str)
	{
		const UTF8* src_start = (const UTF8*)str.Data();
		const UTF8* src_end = src_start + str.Length();

		return isLegalUTF8Sequence(src_start, src_end) != 0;
	}

	Utf16String Unicode::Utf8To16(const CharString& str)
	{
		const UTF8* src_start = (const UTF8*)str.Data();
		const UTF8* src_end = src_start + str.Length();

		Utf16String tmp = Utf16String::Create(str.Length());
		UTF16* dst_start = (UTF16*)tmp.Data();
		UTF16* dst_end = dst_start + tmp.Length();

		ConversionResult res = ConvertUTF8toUTF16(&src_start, src_end, &dst_start, dst_end, Flags);

		if (res != conversionOK && ErrorCheck)
		{
			XLI_THROW("Unicode conversion failed");
		}

		return tmp.SubString(0, dst_start - (UTF16*)tmp.Data());
	}

	Utf32String Utf8To32(const CharString& str)
	{
		const UTF8* src_start = (const UTF8*)str.Data();
		const UTF8* src_end = src_start + str.Length();

		Utf32String tmp = Utf32String::Create(str.Length());
		UTF32* dst_start = (UTF32*)tmp.Data();
		UTF32* dst_end = dst_start + tmp.Length();

		ConversionResult res = ConvertUTF8toUTF32(&src_start, src_end, &dst_start, dst_end, Flags);

		if (res != conversionOK && ErrorCheck)
		{
			XLI_THROW("Unicode conversion failed");
		}

		return tmp.SubString(0, dst_start - (UTF32*)tmp.Data());
	}

	CharString Unicode::Utf16To8(const Utf16String& str)
	{
		const UTF16* src_start = (const UTF16*)str.Data();
		const UTF16* src_end = src_start + str.Length();

		CharString tmp = CharString::Create(str.Length()*2);
		UTF8* dst_start = (UTF8*)tmp.Data();
		UTF8* dst_end = dst_start + tmp.Length();

		ConversionResult res = ConvertUTF16toUTF8(&src_start, src_end, &dst_start, dst_end, Flags);

		if (res != conversionOK && ErrorCheck)
		{
			XLI_THROW("Unicode conversion failed");
		}

		return tmp.SubString(0, dst_start - (UTF8*)tmp.Data());
	}

	Utf32String Unicode::Utf16To32(const Utf16String& str)
	{
		const UTF16* src_start = (const UTF16*)str.Data();
		const UTF16* src_end = src_start + str.Length();

		Utf32String tmp = Utf32String::Create(str.Length());
		UTF32* dst_start = (UTF32*)tmp.Data();
		UTF32* dst_end = dst_start + tmp.Length();

		ConversionResult res = ConvertUTF16toUTF32(&src_start, src_end, &dst_start, dst_end, Flags);

		if (res != conversionOK && ErrorCheck)
		{
			XLI_THROW("Unicode conversion failed");
		}

		return tmp.SubString(0, dst_start - (UTF32*)tmp.Data());
	}

	CharString Unicode::Utf32To8(const Utf32String& str)
	{
		const UTF32* src_start = (const UTF32*)str.Data();
		const UTF32* src_end = src_start + str.Length();

		CharString tmp = CharString::Create(str.Length()*4);
		UTF8* dst_start = (UTF8*)tmp.Data();
		UTF8* dst_end = dst_start + tmp.Length();

		ConversionResult res = ConvertUTF32toUTF8(&src_start, src_end, &dst_start, dst_end, Flags);

		if (res != conversionOK && ErrorCheck)
		{
			XLI_THROW("Unicode conversion failed");
		}

		return tmp.SubString(0, dst_start - (UTF8*)tmp.Data());
	}

	Utf16String Unicode::Utf32To16(const Utf32String& str)
	{
		const UTF32* src_start = (const UTF32*)str.Data();
		const UTF32* src_end = src_start + str.Length();

		Utf16String tmp = Utf16String::Create(str.Length()*2);
		UTF16* dst_start = (UTF16*)tmp.Data();
		UTF16* dst_end = dst_start + tmp.Length();

		ConversionResult res = ConvertUTF32toUTF16(&src_start, src_end, &dst_start, dst_end, Flags);

		if (res != conversionOK && ErrorCheck)
		{
			XLI_THROW("Unicode conversion failed");
		}

		return tmp.SubString(0, dst_start - (UTF16*)tmp.Data());
	}
}
