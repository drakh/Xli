#include <Xli/Console.h>
#include <XliMedia/Xml.h>
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
        if (Attributes.TryGetValue(name, r)) return r;
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
        elm->Name = root->Value();

        for (TiXmlAttribute* attr = root->FirstAttribute(); attr; attr = attr->Next())
        {
            elm->Attributes.Add(attr->Name(), attr->Value());
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
                    Array<char> strb;
                    
                    while (*cstr)
                    {
                        // UTF8 non-breaking space
                        if (*cstr == (char)0xA0) 
                        { 
                            strb.Add((char)0xC2); 
                            strb.Add((char)0xA0); 
                        }
                        else 
                        {
                            strb.Add(*cstr);
                        }

                        cstr++;
                    }

                    text->Value = strb.DataPtr();
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

    XmlElement* Xml::Parse(const String& code)
    {
        TiXmlDocument doc;
        doc.SetCondenseWhiteSpace(false); // TiXml has buggy whitespace condensing

        if (!doc.Parse(code.DataPtr()))
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
