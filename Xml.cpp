#include "Xml.h"
#include "LocRef.h"
#include "SysStr.h"

void Xml::insertBefore(IXMLDOMNode* newChild) {
	VARIANT b;
	b.vt = VT_DISPATCH;
	b.pdispVal = elem;

	LocRef<IXMLDOMDocument> doc(doc());
	doc->insertBefore(newChild, b, NULL);
}

IXMLDOMDocument* Xml::doc(){
	IXMLDOMDocument* res;
	elem->get_ownerDocument(&res);
	return res;
}

Xml Xml::Elem(TCHAR* name){
	LocRef<IXMLDOMDocument> doc(doc()); 

	Xml result;
	doc->createElement(SysStr(name), &result);

	elem->appendChild(result.elem, NULL);

	return result;
}

Xml& Xml::Text(TCHAR* text){

	elem->put_text(SysStr(text));

	return *this;
}

Xml& Xml::TextAsXml(TCHAR* xml){

	SysStr newXml(TEXT("<d>"));
	newXml = newXml + xml;
	newXml = newXml + TEXT("</d>");

	LocRef<IXMLDOMDocument> xmlDoc;

	// create an instance of xmldocument
	HRESULT res = CoCreateInstance(
		__uuidof(DOMDocument), 
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IXMLDOMDocument),
		(void **)&xmlDoc
	);

	res = xmlDoc->loadXML(newXml, NULL);

	if (FAILED(res)){
		throw "Creating of XMLDocument is failed";
	}

	IXMLDOMNodeList* list;
	
	xmlDoc->selectNodes(SysStr(TEXT("/d/*|/d/node()")), &list);

	if (list){
		IXMLDOMNode* node = NULL;

		HRESULT hr = list->reset();
		while(hr=list->nextNode(&node) == S_OK){
			LocRef<IXMLDOMNode> clone;
			
			node->cloneNode(-1, &clone);
			
			elem->appendChild(clone, NULL);

			node->Release();
		}
	}else{
		if (FAILED(elem->put_text(SysStr(xml)))){
			throw "Can not insert Text to the node";
		}
	}

	return *this;
}


Xml& Xml::Attr(TCHAR* name, TCHAR* value){
	VARIANT vValue;

	vValue.vt		= VT_BSTR;
	vValue.bstrVal	= SysAllocString(value);

	elem->setAttribute(SysStr(name), vValue);

	VariantClear(&vValue);

	return *this;
}
