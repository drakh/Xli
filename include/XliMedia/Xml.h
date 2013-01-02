#ifndef __XLI_MEDIA_XML_H__
#define __XLI_MEDIA_XML_H__

#include <Xli/Object.h>
#include <Xli/String.h>
#include <Xli/File.h>
#include <Xli/HashMap.h>
#include <Xli/TextReader.h>

namespace Xli
{
	class XmlElement;
	class XmlText;

	/**
		\ingroup Markup
	*/
	class XmlNode: public Object
	{
	public:
		enum Type
		{
			TypeElement,
			TypeText
		};

		virtual Type GetType() = 0;

		virtual XmlElement* ToElement() { return 0; } 
		virtual XmlText* ToText() { return 0; }
	};

	/**
		\ingroup Markup
	*/
	class XmlText: public XmlNode
	{
	public:
		virtual Type GetType() { return TypeText; }
		virtual XmlText* ToText() { return this; }

		String Value;
	};

	/**
		\ingroup Markup
	*/
	class XmlElement: public XmlNode
	{
	public:
		virtual Type GetType() { return TypeElement; }
		virtual XmlElement* ToElement() { return this; }

		/**
			If the first child is a Text object, then the value of that Text object is returned.
			Else an Exception is thrown.
		*/
		String GetValue();

		/**
			Returns the first child Element with this name or throws an Exception if none could be found.
		*/
		XmlElement* GetElement(const String& name);

		/**
			If the specified attribute exists, then the value is returned.
			Else an Exception is thrown.
		*/
		String GetAttribute(const String& name);

		String Name;
		HashMap<String, String> Attributes;
		Array<Managed<XmlNode> > Children;
	};

	/**
		\ingroup Markup
	*/
	class Xml
	{
	public:
		static XmlElement* Parse(const CharString& code);

		inline static XmlElement* Load(Stream* stream)
		{
			return Parse(TextReader(stream).ReadAllRaw());
		}

		inline static XmlElement* Load(const String& fileName)
		{
			File f(fileName, FileModeRead);
			return Load(&f);
		}
	};
}


#endif
