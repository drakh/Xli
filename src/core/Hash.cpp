#include <Xli/Hash.h>

namespace Xli
{
	UInt32 Hash(const UInt8* data, int size)
	{
		// djb2
		UInt32 hash = 5381;
		for (int i = 0; i < size; i++)
		{
			hash = ((hash << 5) + hash) ^ (UInt32)data[i];
		}
		return hash;
	}

	UInt32 Hash(bool b) 
	{
		return b ? 1 : 0; 
	}
	
	UInt32 Hash(const void* p) 
	{ 
		return Hash((UInt8*)&p, sizeof(void*));
	}
	
	UInt32 Hash(float f) 
	{ 
		return *(UInt32*)&f; 
	}
	
	UInt32 Hash(double d) 
	{ 
        UInt32 hash = 27;
        hash = (13 * hash) + ((UInt32*)&d)[0];
        hash = (13 * hash) + ((UInt32*)&d)[1];
        return hash;
	}

	UInt32 Hash(Int8 i) 
	{ 
		return (UInt32)i; 
	}
	
	UInt32 Hash(UInt8 i) 
	{ 
		return (UInt32)i; 
	}
	
	UInt32 Hash(Int16 i) 
	{ 
		return (UInt32)i; 
	}
	
	UInt32 Hash(UInt16 i) 
	{ 
		return (UInt32)i; 
	}
	
	UInt32 Hash(Int32 i) 
	{ 
		return (UInt32)i; 
	}
	
	UInt32 Hash(UInt32 i) 
	{ 
		return i; 
	}
	
	UInt32 Hash(Int64 i) 
	{
        UInt32 hash = 27;
        hash = (13 * hash) + ((UInt32*)&i)[0];
        hash = (13 * hash) + ((UInt32*)&i)[1];
        return hash;
	}
	
	UInt32 Hash(UInt64 i) 
	{ 
        UInt32 hash = 27;
        hash = (13 * hash) + ((UInt32*)&i)[0];
        hash = (13 * hash) + ((UInt32*)&i)[1];
        return hash;
	}
}
