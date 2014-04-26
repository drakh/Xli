#ifndef __XLI_STRING_DECL_H__
#define __XLI_STRING_DECL_H__

#include <Xli/IntTypes.h>

#ifdef XLI_PLATFORM_ANDROID 
# include <stdarg.h>
#else
# include <cstdarg>
#endif

namespace Xli
{
    /**
        \ingroup XliCoreText
    */
    class String
    {
        friend class Unicode;
        static const int BufSize = 8;

        char* data;
        char buf[BufSize];
        int length;

        void initLength(int len);
        void init(const char* str, int len);
        void init(int i);
        void init(float f);
        void init(double d);
        void init(bool b);
        void deinit();

    public:
        String();
        String(const String& copy);
        String(const char* str);
        String(const char* str, int len);
        String(char c);
        String(int i);
        String(float f);
        String(double d);
        ~String();

        char* DataCopy();

        char* DataPtr();
        const char* DataPtr() const;

        int Length() const;

        char& operator [] (int index);
        const char& operator [] (int index) const;

        const char& First() const;
        const char& Last() const;

        int IndexOf(char c, int start) const;
        int IndexOf(char c) const;

        int LastIndexOf(char c, int start) const;
        int LastIndexOf(char c) const;

        String Substring(int start, int len) const;
        String Substring(int start) const ;

        String Trim() const;
        String Trim(char c) const;

        String ToLower() const;
        String ToUpper() const;

        void Split(char c, Array<String>& parts) const;
        static String Join(char c, const Array<String>& list);

        bool StartsWith(const String& str) const;
        bool EndsWith(const String& str) const;

        static String Create(int length);
        static String HexFromInt(int h);
        static String FromBool(bool b);
        
        static String Format(const char* format, va_list argList);
        static String Format(const char* format, ...);

        int HexToInt() const;
        int ToInt() const;
        float ToFloat() const;
        double ToDouble() const;
        bool ToBool() const;

        bool Equals(const char* str, int len) const;
        bool Equals(const char* str) const;
        bool Equals(const String& str) const;

        int CompareTo(const char* str, int len) const;
        int CompareTo(const char* str) const;
        int CompareTo(const String& str) const;

        String Add(const char* str, int len) const;
        String Add(const char* str) const;
        String Add(const String& str) const;

        void Append(const char* str, int len);
        void Append(const char* str);
        void Append(const String& str);
        
        bool operator == (const String& str) const;
        bool operator == (const char* str) const;

        bool operator != (const String& str) const;
        bool operator != (const char* str) const;

        String operator + (const String& str) const;
        String operator + (const char* str) const;

        bool operator < (const String& str) const;
        bool operator < (const char* str) const;

        bool operator <= (const String& str) const;
        bool operator <= (const char* str) const;

        bool operator > (const String& str) const;
        bool operator > (const char* str) const;

        bool operator >= (const String& str) const;
        bool operator >= (const char* str) const;

        String& operator += (const String& str);
        String& operator += (const char* str);
        
        String& operator = (const String& str);
        String& operator = (const char* str);
        String& operator = (char c);
        String& operator = (int i);
        String& operator = (float f);
        String& operator = (double d);
    };
}

/**
    \addtogroup XliCoreText
    @{
*/

Xli::String operator + (const char* a, const Xli::String& b);

/** @} */

#endif
