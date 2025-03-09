#include "LocRef.h"
#include "SysStr.h"
#include "ResultXML.h"

IXMLDOMElement* ResultXML::CreateElement(TCHAR* name){
	IXMLDOMElement* elem;
	
	if (FAILED(m_XmlDoc->createElement(SysStr(name), &elem))){
		throw "Can not create XML Element ";
	}

	return elem;
}

IXMLDOMElement* ResultXML::CreateElement(TCHAR* name, TCHAR* value){

	IXMLDOMElement* elem = CreateElement(name);

	if (FAILED(elem->put_text(SysStr(value)))){
		throw "Can not insert Text to the node";
	}

	return elem;
}


void ResultXML::AppendElement(IXMLDOMElement* el, TCHAR* name, TCHAR* value){
	LocRef<IXMLDOMElement> elem = CreateElement(name);

	LocRef<IXMLDOMNodeList> list = ParseXML(value);

	if (list){
		IXMLDOMNode* node = NULL;

		HRESULT hr = list->reset();
		long len;
		list->get_length(&len);
		while((hr=list->nextNode(&node)) == S_OK){
			LocRef<IXMLDOMNode> clone;
			
			node->cloneNode(-1, &clone);
			
			elem->appendChild(clone, NULL);

			node->Release();
		}
	}else{
		if (FAILED(elem->put_text(SysStr(value)))){
			throw "Can not insert Text to the node";
		}
	}

	el->appendChild(elem, NULL);
}

void ResultXML::SetAttribute(IXMLDOMElement* elem, TCHAR* name, TCHAR* value){
	VARIANT vValue;

	vValue.vt		= VT_BSTR;
	vValue.bstrVal	= SysAllocString(value);

	elem->setAttribute(SysStr(name), vValue);

	VariantClear(&vValue);
}

IXMLDOMNodeList* ResultXML::ParseXML(BSTR xml){
	LocRef<IXMLDOMDocument> xmlDoc;

	// create an instance of xmldocument
	HRESULT res = CoCreateInstance(
		__uuidof(DOMDocument), 
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IXMLDOMDocument),
		(void **)&xmlDoc
	);

	if (FAILED(res)){
		throw "Creating of XMLDocument is failed";
	}

	UINT len = SysStringLen(xml);
	BSTR newStr = SysAllocStringLen(TEXT("<d>"),len + 3 + 4);
	BSTR newXML = newStr;

	size_t xmlBytes = len * sizeof(OLECHAR);
	newStr += 3;

	memcpy(newStr, xml,  xmlBytes);
	
	newStr += len;

	memcpy(newStr, TEXT("</d>"),  4*sizeof(OLECHAR));

	res = xmlDoc->loadXML(newXML, NULL);

	SysFreeString(newXML);

	if (FAILED(res)){
		throw "Creating of XMLDocument is failed";
	}

	IXMLDOMNodeList* nodes;
	
	xmlDoc->selectNodes(SysStr(TEXT("/d/*|/d/node()")), &nodes);

	long len1;
	nodes->get_length(&len1);

	return nodes;
}

ResultXML::ResultXML(void)
{
	m_XmlDoc = NULL;

	// create an instance of xmldocument
	HRESULT res = CoCreateInstance(
		__uuidof(DOMDocument), 
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IXMLDOMDocument),
		(void **)&m_XmlDoc
	);

	if (FAILED(res)){
		throw "Creating of XMLDocument is failed";
	}

	m_DocElem.elem = CreateElement(TEXT("TypeLibrary"));
	res = m_XmlDoc->appendChild(m_DocElem.elem, NULL);

	if (FAILED(res)){
		throw "Creating of XMLDocument is failed";
	}
}


ResultXML::~ResultXML(void)
{
	if (m_XmlDoc){
		m_XmlDoc->Release();
	}
}

void ResultXML::Save(TCHAR* file){
	VARIANT vFile;
	vFile.bstrVal	= SysAllocString(file);
	vFile.vt		= VT_BSTR;

	m_XmlDoc->save(vFile);

	VariantClear(&vFile);
}