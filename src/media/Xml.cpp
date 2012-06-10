#include <Xli/Console.h>
#include <Xli/Unicode.h>
#include <XliMedia/Xml.h>
#include <Xli/StringBuilder.h>
#include <tinyxml.h>

namespace Xli
{
	String XmlElement::GetValue()
	{
		if (Children.Length() > 0 && Children[0]->ToText()) return Children[0]->ToText()->Value;
		XLI_THROW("Value not found");
	}

	String XmlElement::GetAttribute(const String& name)
	{
		String r;
		if (Attributes.Get(name, r)) return r;
		XLI_THROW("Attribute not found: " + name);
	}

	XmlElement* XmlElement::GetElement(const String& name)
	{
		for (int i = 0; i < Children.Length(); i++)
		{
			if (Children[i]->ToElement() && Children[i]->ToElement()->Name == name)
			{
				return Children[i]->ToElement();
			}
		}
		XLI_THROW("Element not found: " + name);
	}

	static XmlElement* ToXmlElement(TiXmlElement* root)
	{
		XmlElement* elm = new XmlElement();
		elm->Name = Unicode::Utf8To16(root->Value());

		for (TiXmlAttribute* attr = root->FirstAttribute(); attr; attr = attr->Next())
		{
			elm->Attributes.Add(Unicode::Utf8To16(attr->Name()), Unicode::Utf8To16(attr->Value()));
		}

		for (TiXmlNode* node = root->FirstChild(); node; node = node->NextSibling())
		{
			switch (node->Type())
			{
			case TiXmlNode::TEXT:
				{
					TiXmlText* textNode = node->ToText();
					XmlText* text = new XmlText();
					const char* cstr = textNode->Value();
					CharStringBuilder strb;
					while (*cstr)
					{
						if (*cstr == (char)0xA0) { strb.AppendChar((char)0xC2); strb.AppendChar((char)0xA0); } // UTF8 non-breaking space
						else strb.AppendChar(*cstr);
						cstr++;
					}

					text->Value = Unicode::Utf8To16(strb.GetString());
					elm->Children.Add(text);
				}
				break;

			case TiXmlNode::ELEMENT:
				elm->Children.Add(ToXmlElement(node->ToElement()));
				break;

			default:
				break;
			};
		}

		return elm;
	}

	XmlElement* Xml::Parse(const CharString& code)
	{
		TiXmlDocument doc;
		doc.SetCondenseWhiteSpace(false); // TiXml has buggy whitespace condensing

		if (!doc.Parse(code.Data()))
		{
			//XLI_THROW("Couldn't parse Xml");
			ErrorPrintLine("XML WARNING: Errors found in Xml document. May not be displayed correctly.");
		}

		if (!doc.RootElement())
		{
			XLI_THROW("Couldn't parse Xml");
		}

		return ToXmlElement(doc.RootElement());
	}
}
