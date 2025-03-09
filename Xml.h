#pragma once

#include <MsXml2.h>

struct Xml{
	IXMLDOMElement* elem;

	Xml(IXMLDOMElement* _elem = NULL){
		elem = _elem;
	}
	Xml(Xml& src){
		elem = src.elem;
		if (elem)
			elem->AddRef();
	}

	IXMLDOMElement* operator->(){
		return elem;
	}

	IXMLDOMElement** operator&(){
		return &elem;
	}

	~Xml(){
		if (elem)
			elem->Release();
	}

	void insertBefore(IXMLDOMNode* newChild);

	IXMLDOMDocument* doc();

	Xml Elem(TCHAR* name);
	Xml& Text(TCHAR* text);
	Xml& TextAsXml(TCHAR* xml);
	Xml& Attr(TCHAR* name, TCHAR* value);
};