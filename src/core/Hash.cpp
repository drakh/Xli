#include <Xli/Hash.h>

namespace Xli
{
	UInt32 Hash(bool b) 
	{ 
		return b ? 1 : 0; 
	}
	
	UInt32 Hash(void* p) 
	{ 
		return (UInt32)(long long)p; 
	}
	
	UInt32 Hash(float f) 
	{ 
		return *(UInt32*)&f; 
	}
	
	UInt32 Hash(double d) 
	{ 
		return ((UInt32*)&d)[0] ^ ((UInt32*)&d)[1]; 
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
		return ((UInt32*)&i)[0] ^ ((UInt32*)&i)[1]; 
	}
	
	UInt32 Hash(UInt64 i) 
	{ 
		return ((UInt32*)&i)[0] ^ ((UInt32*)&i)[1]; 
	}
}
