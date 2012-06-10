#include <XliPlatform/Selector.h>
#include <XliPlatform/Mutex.h>

namespace Xli
{
	int Selector::initCount = 0;
	UInt32 Selector::enumerator = 0;
	void* Selector::mutex = 0;
	HashMap<String, UInt32>* Selector::selectors = 0;
	HashMap<const char*, UInt32>* Selector::selectorsCstr = 0;
	Array<String>* Selector::selectorsReverse = 0;

	UInt32 Selector::getSelectorFromSymbol(const char* symbol)
	{
		if (!mutex) Init();
		LockMutex(mutex);
		
		UInt32 h;
		if (selectorsCstr->Get(symbol, h))
		{
			UnlockMutex(mutex);
			return h;
		}
		
		String s(symbol);
		if (selectors->Get(s, h))
		{
			UnlockMutex(mutex);
			return h;
		}
	
		h = enumerator++;
		selectors->Add(s, h);
		selectorsCstr->Add(symbol, h);
		selectorsReverse->Add(s);

		UnlockMutex(mutex);
		return h;
	}

	UInt32 Selector::getSelectorFromSymbol(const String& symbol)
	{
		if (!mutex) Init();
		LockMutex(mutex);
		
		UInt32 h;
		if (selectors->Get(symbol, h))
		{
			UnlockMutex(mutex);
			return h;
		}
	
		h = enumerator++;
		selectors->Add(symbol, h);
		selectorsReverse->Add(symbol);

		UnlockMutex(mutex);
		return h;
	}

	String Selector::getSymbolFromSelector(UInt32 selector)
	{
		if (!mutex) Init();
		LockMutex(mutex);

		if ((int)selector < selectorsReverse->Length())
		{
			UnlockMutex(mutex);
			return selectorsReverse->Get(selector);
		}

		UnlockMutex(mutex);
		XLI_THROW("Invalid selector");
	}

	void Selector::Init()
	{
		if (!initCount)
		{
			mutex = CreateMutex();
			selectors = new HashMap<String, UInt32>();
			selectorsCstr = new HashMap<const char*, UInt32>();
			selectorsReverse = new Array<String>();
			enumerator = 0;
		}
		initCount++;
		
	}

	void Selector::CleanUp()
	{
		if (!initCount) XLI_THROW("CleanUp() called before/without Init()");
		initCount--;
		if (!initCount)
		{
			DeleteMutex(mutex);
			delete selectors;
			delete selectorsCstr;
			delete selectorsReverse;
		}
	}
}
