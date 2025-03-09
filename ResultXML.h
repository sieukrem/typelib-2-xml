#pragma once

#include <MsXml2.h>
#include "Xml.h"

class ResultXML
{
public:
	Xml             m_DocElem;

	IXMLDOMElement* CreateElement(TCHAR* name);
	IXMLDOMElement* CreateElement(TCHAR* name, TCHAR* value);
	void			AppendElement(IXMLDOMElement* el, TCHAR* name, TCHAR* value);

	IXMLDOMNodeList* ParseXML(BSTR xml);

	static void  SetAttribute(IXMLDOMElement* elem, TCHAR* name, TCHAR* value);

	IXMLDOMDocument* m_XmlDoc;
	ResultXML(void);
	~ResultXML(void);
	void Save(TCHAR* file);
};

