#include <Xli/DataType.h>
#include <Xli/Exception.h>

namespace Xli
{
	const char* DataTypeInfo::ToString(DataType dt)
	{
		switch (dt)
		{
		case DataTypeHalf: return "Half";
		case DataTypeFloat: return "Float";
		case DataTypeDouble: return "Double";
		case DataTypeUInt64: return "UInt64";
		case DataTypeUInt32: return "UInt32";
		case DataTypeUInt24: return "UInt24";
		case DataTypeUInt16: return "UInt16";
		case DataTypeUInt8: return "UInt8";
		case DataTypeInt64: return "Int64";
		case DataTypeInt32: return "Int32";
		case DataTypeInt24: return "Int24";
		case DataTypeInt16: return "Int16";
		case DataTypeInt8: return "Int8";
		default: return "<Unknown DataType>";
		}
	}

	int DataTypeInfo::SizeOf(DataType dt)
	{
		switch (dt)
		{
		case DataTypeHalf: return 2;
		case DataTypeDouble: return 8;
		case DataTypeFloat: return 4;
		case DataTypeUInt64: return 4;
		case DataTypeUInt32: return 4;
		case DataTypeUInt24: return 3;
		case DataTypeUInt16: return 2;
		case DataTypeUInt8: return 1;
		case DataTypeInt64: return 8;
		case DataTypeInt32: return 4;
		case DataTypeInt24: return 3;
		case DataTypeInt16: return 2;
		case DataTypeInt8: return 1;
		default: XLI_THROW("Unknown size for " + (String)DataTypeInfo::ToString(dt));
		}
	}

	bool DataTypeInfo::IsSigned(DataType dt)
	{
		switch (dt)
		{
		case DataTypeHalf:
		case DataTypeDouble:
		case DataTypeFloat:
		case DataTypeInt64:
		case DataTypeInt32:
		case DataTypeInt24:
		case DataTypeInt16:
		case DataTypeInt8:
			return true;

		case DataTypeUInt64:
		case DataTypeUInt32:
		case DataTypeUInt24:
		case DataTypeUInt16:
		case DataTypeUInt8:
			return false;

		default: XLI_THROW("Unknown sign for " + (String)DataTypeInfo::ToString(dt));
		}
	}
}
