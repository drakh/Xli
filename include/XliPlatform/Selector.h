#ifndef __XLI_SELECTOR_H__
#define __XLI_SELECTOR_H__

#include <Xli/String.h>
#include <Xli/HashMap.h>

namespace Xli
{
	class Selector
	{
		static int initCount;
		static UInt32 enumerator;
		static void* mutex;
		static HashMap<String, UInt32>* selectors;
		static HashMap<const char*, UInt32>* selectorsCstr;
		static Array<String>* selectorsReverse;
		static UInt32 getSelectorFromSymbol(const char* symbol);
		static UInt32 getSelectorFromSymbol(const String& symbol);
		static String getSymbolFromSelector(UInt32 selector);
		UInt32 handle;
	public:
		/**
			For thread safety, call this before spawning any threads.
			Automatically called when the first Selector is constructed if not called before.
			Counts the number of times it is called, inited until number of calls to Init() equals number of calls to CleanUp().
		*/
		static void Init();

		/**
			If you want to delete the selector hash table and mutex, call this when done using selectors.
			Counts the number of times it is called, clean up is not actually done until number of calls to CleanUp equals number of calls to Init().
		*/
		static void CleanUp();

		Selector()
		{
			handle = ~0;
		}
		explicit Selector(UInt32 handle)
		{
			this->handle = handle;
		}
		Selector(const char* symbol)
		{
			handle = getSelectorFromSymbol(symbol);
		}
		Selector(const String& symbol)
		{
			handle = getSelectorFromSymbol(symbol);
		}
		Selector(const Selector& sel)
		{
			this->handle = sel.handle;
		}
		Selector operator = (const Selector& sel)
		{
			this->handle = sel.handle;
			return *this;
		}
		bool IsNull() const { return handle == (UInt32)~0; }
		UInt32 GetHandle() const { return handle; }
		UInt32 Hash() const { return handle; }
		String GetSymbol() const { return getSymbolFromSelector(handle); }

		bool operator == (const Selector& s) const { return handle == s.handle; }
		bool operator != (const Selector& s) const { return handle != s.handle; }

		operator String () const { return GetSymbol(); }
	};
}

#endif
