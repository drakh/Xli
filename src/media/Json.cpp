#include <Xli/Unicode.h>
#include <Xli/Console.h>
#include <XliMedia/Json.h>
#include <XliMedia/FormattedTextWriter.h>
#include "3rdparty/json_parser/JSON_parser.h"

namespace Xli
{
	class JsonContext
	{
	public:
		Array<Value> Values;
		Array<String> Keys;
		Value Result;
		bool PreserveOrder;

		JsonContext(bool preserveOrder): PreserveOrder(preserveOrder)
		{
		}

		void AddValue(Value v)
		{
			if (!Values.Length() && Result.IsUndefined()) Result = v;
			else if (Values.Last().IsArray()) Values.Last()->Append(v);
			else if (Values.Last().IsObject()) Values.Last()->Insert(Keys.RemoveLast(), v);
			else XLI_THROW("JSON parser failed");
		}
	};

	static int JsonCallback(void* ctx, int type, const JSON_value* value)
	{
		JsonContext* c = (JsonContext*)ctx;

		switch(type) {
		case JSON_T_ARRAY_BEGIN:    
			c->Values.Add(new ArrayValue());
			break;
		case JSON_T_ARRAY_END:
			c->AddValue(c->Values.RemoveLast());
			break;
	    case JSON_T_OBJECT_BEGIN:
			c->PreserveOrder ? c->Values.Add(new OrderedObjectValue()) : c->Values.Add(new ObjectValue());
			break;
		case JSON_T_OBJECT_END:
			c->AddValue(c->Values.RemoveLast());
			break;
		case JSON_T_INTEGER:
			c->AddValue(new IntegerValue(value->vu.integer_value));
			break;
		case JSON_T_FLOAT:
			c->AddValue(new DoubleValue(value->vu.float_value));
			break;
		case JSON_T_NULL:
			c->AddValue(new Undefined());
			break;
		case JSON_T_TRUE:
			c->AddValue(new BoolValue(true));
			break;
		case JSON_T_FALSE:
			c->AddValue(new BoolValue(false));
			break;
		case JSON_T_KEY:
			c->Keys.Add(Unicode::Utf8To16(CharString(value->vu.str.value, value->vu.str.length)));
			break;   
		case JSON_T_STRING:
			c->AddValue(new StringValue(Unicode::Utf8To16(CharString(value->vu.str.value, value->vu.str.length))));
			break;
		default:
			XLI_THROW("JSON parser failed");
			break;
		}
    
		return 1;
	}

	Value Json::Parse(const CharString& code, bool preserveOrder)
	{
		JSON_config config;

		struct JSON_parser_struct* jc = NULL;
    
		init_JSON_config(&config);
    
		config.depth                  = 19;
		config.callback               = &JsonCallback;
		config.callback_ctx           = new JsonContext(preserveOrder);
		config.allow_comments         = 1;
		config.handle_floats_manually = 0;
    
		jc = new_JSON_parser(&config);

		int ln = 1;
    
		for (int i = 0; i < code.Length(); i++)
		{
			int next_char = code[i];

			if (!JSON_parser_char(jc, next_char))
			{
				delete_JSON_parser(jc);
				XLI_THROW("JSON Syntax error on line " + (String)ln);
			}

			if (next_char == '\n') ln++;
		}

		if (!JSON_parser_done(jc))
		{
			delete_JSON_parser(jc);
			XLI_THROW("JSON Syntax error on line " + (String)ln);
		}

		JsonContext* c = (JsonContext*)config.callback_ctx;
		Value val = c->Result;

		delete c;
		delete_JSON_parser(jc);

		return val;
	}

	CharString Json::FormatString(const String& str)
	{
		Array<char> w;

		w.Add('"');
		CharString k = Unicode::Utf16To8(str);

		for (int i = 0; i < k.Length(); i++)
		{
			if (k[i] == '"') w.Add("\\\"", 2);
			else if (k[i] == '\n') w.Add("\\n", 2);
			else if (k[i] == '\r') w.Add("\\r", 2);
			else if (k[i] == '\t') w.Add("\\t", 2);
			else if (k[i] == '\0') w.Add("\\0", 2);
			else if (k[i] == '\\') w.Add("\\\\", 2);
			else w.Add(k[i]);
		}

		w.Add('"');
		return w.Data();
	}

	class JsonWriter: public FormattedTextWriter
	{
	public:
		Array<bool> Array;
		bool ArrayEndSkipLine;

		JsonWriter(Stream* stream): FormattedTextWriter(stream), ArrayEndSkipLine(false)
		{
		}

		void WriteRecursive(const Value& value)
		{
			switch (value.GetType())
			{
			case ValueTypeString:
				Write(Json::FormatString(value.ToString()));
				break;

			case ValueTypeUndefined:
			case ValueTypeInteger:
			case ValueTypeInt64:
			case ValueTypeDouble:
			case ValueTypeBool:
				Write(value.ToString());
				break;

			case ValueTypeArray:
				{
					Write("[");
					Array.Add(true);
					ArrayEndSkipLine = false;

					if (value.Count() > 0)
					{
						for (int i = 0; i < value.Count(); i++)
						{
							if (i > 0) Write(", ");
							WriteRecursive(value.GetValue(i));
						}
					}

					Array.RemoveLast();

					if (ArrayEndSkipLine)
					{
						EndLine();
						BeginLine();
						ArrayEndSkipLine = false;
					}

					Write("]");
				}
				break;

			case ValueTypeOrderedObject:
			case ValueTypeObject:
				{
					if (Array.Last())
					{
						EndLine();
						PushIndent();
						BeginLine();
					}

					Write("{");
					Array.Add(false);

					if (value.Count() > 0)
					{
						EndLine();
						PushIndent();
						BeginLine();

						bool first = true;

						for (int it = value.Begin(); it != value.End(); it = value.Next(it))
						{
							const Value& key = value.GetKey(it);
							const Value& val = value.GetValue(it);

							if (!first)
							{
								Write(",");
								EndLine();
								BeginLine();
							}

							first = false;

							Write(Json::FormatString(key.ToString()));
							Write(": ");
							WriteRecursive(val);
						}

						EndLine();
						PopIndent();
						BeginLine();
					}

					Array.Add();
					Write("}");

					if (Array.Last())
					{
						PopIndent();
						ArrayEndSkipLine = true;
					}
				}
				break;

			default:
				ErrorPrintLine("JSON WARNING: Unsupported value type: " + (String)Value::TypeToString(value.GetType()));
				break;
			}
		}

		void WriteJson(const Value& value)
		{
			BeginLine();
			Array.Add(false);
			WriteRecursive(value);
			Array.RemoveLast();
			EndLine();
		}
	};

	void Json::Save(Stream* stream, const Value& value)
	{
		JsonWriter(stream).WriteJson(value);
	}

	CharString Json::ToStringRaw(const Value& value)
	{
		TextStream ts;
		Save(&ts, value);
		return ts.GetTextRaw();
	}
}
