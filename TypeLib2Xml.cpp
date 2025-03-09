#include "LocRef.h"
#include "ResultXML.h"
#include "SysStr.h"
#include <tchar.h>

ITypeInfo* currentType;


TCHAR* GetTypeName(TYPEDESC* typeDesc){
	TCHAR* result;
	switch (typeDesc->vt){
		case VT_EMPTY:
			result = TEXT("VT_EMPTY");
			break;
		case VT_NULL:
			result = TEXT("VT_NULL");
			break;
		case VT_I2:
			result = TEXT("VT_I2");
			break;
		case VT_I4:
			result = TEXT("VT_I4");
			break;
		case VT_R4:
			result = TEXT("VT_R4");
			break;
		case VT_R8:
			result = TEXT("VT_R8");
			break;
		case VT_CY:
			result = TEXT("VT_CY");
			break;
		case VT_DATE:
			result = TEXT("VT_DATE");
			break;
		case VT_BSTR:
			result = TEXT("VT_BSTR");
			break;
		case VT_DISPATCH:
			result = TEXT("VT_DISPATCH");
			break;
		case VT_ERROR:
			result = TEXT("VT_ERROR");
			break;
		case VT_BOOL:
			result = TEXT("VT_BOOL");
			break;
		case VT_VARIANT:
			result = TEXT("VT_VARIANT");
			break;
		case VT_UNKNOWN:
			result = TEXT("VT_UNKNOWN");
			break;
		case VT_DECIMAL:
			result = TEXT("VT_DECIMAL");
			break;
		case VT_I1:
			result = TEXT("VT_I1");
			break;
		case VT_UI1:
			result = TEXT("VT_UI1");
			break;
		case VT_UI2:
			result = TEXT("VT_UI2");
			break;
		case VT_UI4:
			result = TEXT("VT_UI4");
			break;
		case VT_I8:
			result = TEXT("VT_I8");
			break;
		case VT_UI8:
			result = TEXT("VT_UI8");
			break;
		case VT_INT:
			result = TEXT("VT_INT");
			break;
		case VT_UINT:
			result = TEXT("VT_UINT");
			break;
		case VT_VOID:
			result = TEXT("VT_VOID");
			break;
		case VT_HRESULT:
			result = TEXT("VT_HRESULT");
			break;
		case VT_PTR:
			result = TEXT("VT_PTR");
			break;
		case VT_SAFEARRAY:
			result = TEXT("VT_SAFEARRAY");
			break;
		case VT_CARRAY:
			result = TEXT("VT_CARRAY");
			break;
		case VT_USERDEFINED:
			result = TEXT("VT_USERDEFINED");
			break;
		case VT_LPSTR:
			result = TEXT("VT_LPSTR");
			break;
		case VT_LPWSTR:
			result = TEXT("VT_LPWSTR");
			break;
		case VT_RECORD:
			result = TEXT("VT_RECORD");
			break;
		case VT_INT_PTR:
			result = TEXT("VT_INT_PTR");
			break;
		case VT_UINT_PTR:
			result = TEXT("VT_UINT_PTR");
			break;
		case VT_FILETIME:
			result = TEXT("VT_FILETIME");
			break;
		case VT_BLOB:
			result = TEXT("VT_BLOB");
			break;
		case VT_STREAM:
			result = TEXT("VT_STREAM");
			break;
		case VT_STORAGE:
			result = TEXT("VT_STORAGE");
			break;
		case VT_STREAMED_OBJECT:
			result = TEXT("VT_STREAMED_OBJECT");
			break;
		case VT_STORED_OBJECT:
			result = TEXT("VT_STORED_OBJECT");
			break;
		case VT_BLOB_OBJECT:
			result = TEXT("VT_BLOB_OBJECT");
			break;
		case VT_CF:
			result = TEXT("VT_CF");
			break;
		case VT_CLSID:
			result = TEXT("VT_CLSID");
			break;
		case VT_VERSIONED_STREAM:
			result = TEXT("VT_VERSIONED_STREAM");
			break;
		case VT_BSTR_BLOB:
			result = TEXT("VT_BSTR_BLOB");
			break;
		case VT_VECTOR:
			result = TEXT("VT_VECTOR");
			break;
		case VT_ARRAY:
			result = TEXT("VT_ARRAY");
			break;
		case VT_BYREF:
			result = TEXT("VT_BYREF");
			break;
		case VT_RESERVED:
			result = TEXT("VT_RESERVED");
			break;
		case VT_ILLEGAL:
			result = TEXT("VT_ILLEGAL");
			break;
	}

	return result;
}

void GetTypeDesc(Xml& parent, TYPEDESC* typeDesc){
	TCHAR* name;

	switch (typeDesc->vt){
	case VT_PTR:
	case VT_SAFEARRAY:	
		GetTypeDesc(parent, typeDesc->lptdesc);
		break;
	case VT_CARRAY:

		GetTypeDesc(parent, & typeDesc->lpadesc->tdescElem );
		break;
	case VT_USERDEFINED:{
		LocRef<ITypeInfo> udt;

		currentType->GetRefTypeInfo(typeDesc->hreftype, &udt);

		udt->GetDocumentation(-1, &name, NULL, NULL, NULL);

		parent.Attr(TEXT("typedesc"), name);
		break;
	}
	default:

		name = GetTypeName(typeDesc);

		parent.Attr(TEXT("typedesc"), name);
	}

}

void GetParams(BSTR param, Xml& eFunction, TCHAR* kind, ELEMDESC* elemDesc ){
	Xml eParam = eFunction.Elem(TEXT("param"));

	eParam.Attr(TEXT("name"), param);

	eParam.Attr(TEXT("kind"), kind);

	GetTypeDesc(eParam, &elemDesc->tdesc);
	
	USHORT paramFlags = elemDesc->paramdesc.wParamFlags;
	if (paramFlags & PARAMFLAG_FIN)
		eParam.Elem(TEXT("in"));
	if (paramFlags & PARAMFLAG_FOUT)
		eParam.Elem(TEXT("out"));
	if (paramFlags & PARAMFLAG_FLCID)
		eParam.Elem(TEXT("lcid"));
	if (paramFlags & PARAMFLAG_FOPT)
		eParam.Elem(TEXT("opt"));
	if (paramFlags & PARAMFLAG_FRETVAL)
		eParam.Elem(TEXT("retval"));
	if (paramFlags & PARAMFLAG_NONE)
		eParam.Elem(TEXT("none"));
	if (paramFlags & PARAMFLAG_FHASDEFAULT)
		eParam.Elem(TEXT("hasdefault"));
	if (paramFlags & PARAMFLAG_FHASCUSTDATA)
		eParam.Elem(TEXT("hascustdata"));
}

void GetFunction(ITypeInfo* type, Xml& eType,  UINT fi){
	FUNCDESC* pfd;
	type->GetFuncDesc(fi, &pfd);
	Xml eFunction = eType.Elem(TEXT("function"));

	SysStr tName, tDoc;

	type->GetDocumentation(pfd->memid, &tName, &tDoc, NULL, NULL);
	
	if (tName.str)
		eFunction.Attr(TEXT("name"), tName);
	if (tDoc){
		eFunction.Elem(TEXT("documentation")).TextAsXml(tDoc);
	}

	TCHAR* invkind;
	switch (pfd->invkind){
		case INVOKE_FUNC:
			invkind = TEXT("func");
			break;
		case INVOKE_PROPERTYGET:
			invkind = TEXT("propertyget");
			break;
		case INVOKE_PROPERTYPUT:
			invkind = TEXT("propertyput");
			break;
		case INVOKE_PROPERTYPUTREF:
			invkind = TEXT("propertyputref");
			break;
	}
	eFunction.Attr(TEXT("invoke_kind"), invkind);

	if (pfd->cParams){
		BSTR* names = new BSTR[pfd->cParams + 1];
		UINT	cnames = 0;
		HRESULT res = type->GetNames(pfd->memid, names, pfd->cParams+1, &cnames);
		if (res == S_OK)
			for(UINT i=1; i< cnames; i++){
				
				if (pfd->cParamsOpt == 0) // there are no optionals
					GetParams(names[i], eFunction,TEXT("required"), &pfd->lprgelemdescParam[i-1]);
				else if (pfd->cParamsOpt !=-1 && i- (pfd->cParams - pfd->cParamsOpt)>=0) // all last cParamsOpt params
					GetParams(names[i], eFunction,TEXT("optional"), &pfd->lprgelemdescParam[i-1]);
				else if (i+1==cnames) // the last param
					GetParams(names[i], eFunction, TEXT("array_of_optional"), &pfd->lprgelemdescParam[i-1]);
			}

		delete[] names;
	}

	// type of returned value

	Xml retVal = eFunction.Elem(TEXT("return"));
	GetTypeDesc(retVal, &pfd->elemdescFunc.tdesc);

	type->ReleaseFuncDesc(pfd);
}

void GetVariable(ITypeInfo* type, Xml& eType,  UINT vi){
	VARDESC* pvd;
	type->GetVarDesc(vi, &pvd);
	Xml eVar = eType.Elem(TEXT("variable"));

	SysStr tName, tDoc;

	type->GetDocumentation(pvd->memid, &tName, &tDoc, NULL, NULL);
	
	if (tName.str)
		eVar.Attr(TEXT("name"), tName);
	if (tDoc){
		eVar.Elem(TEXT("documentation")).TextAsXml(tDoc);
	}

	TCHAR* varkind;
	switch (pvd->varkind){
		case VAR_CONST:       varkind = TEXT("const");          break;
		case VAR_DISPATCH:    varkind = TEXT("dispatch");       break;
		case VAR_PERINSTANCE: varkind = TEXT("perinstance");    break;
		case VAR_STATIC:      varkind = TEXT("propertyputref"); break;
	}

	eVar.Attr(TEXT("kind"), varkind);

	if (pvd->lpvarValue){
		if (VariantChangeType(pvd->lpvarValue, pvd->lpvarValue, VARIANT_ALPHABOOL, VT_BSTR) == S_OK)
			eVar.Attr(TEXT("value"), pvd->lpvarValue->bstrVal);
	}

	type->ReleaseVarDesc(pvd);
}


void AddImplements(Xml& eType, ITypeInfo* type, unsigned int index){
	HREFTYPE refType;
	type->GetRefTypeOfImplType(index, &refType);

	LocRef<ITypeInfo> refTypeInfo;
	type->GetRefTypeInfo(refType, &refTypeInfo);

	SysStr refName;
	refTypeInfo->GetDocumentation(-1, &refName, NULL,NULL,NULL);

	Xml implType = eType.Elem(TEXT("impl"));
	implType.Attr(TEXT("name"), refName);
}

void GetType(Xml& docElem, ITypeInfo* type){
	TYPEATTR* ptya;
	type->GetTypeAttr(&ptya);

	currentType = type;
	
	TCHAR* kind;

	switch(ptya->typekind){
		case TKIND_ENUM	     : kind = TEXT("enum"); break;
		case TKIND_RECORD    : kind = TEXT("record"); break;
		case TKIND_MODULE    : kind = TEXT("module"); break;
		case TKIND_INTERFACE : kind = TEXT("interface"); break;
		case TKIND_DISPATCH  : kind = TEXT("dispatch"); break;
		case TKIND_COCLASS   : kind = TEXT("coclass"); break;
		case TKIND_ALIAS     : kind = TEXT("alias"); break;
		case TKIND_UNION     : kind = TEXT("union"); break;
		case TKIND_MAX       : kind = TEXT("max"); break;
	}

	Xml eType = docElem.Elem(TEXT("type"));
	eType.Attr(TEXT("kind"), kind);

	for(unsigned int i=0; i<ptya->cImplTypes; i++){
		AddImplements(eType, type, i);
	}

	SysStr tName,tDoc;
	type->GetDocumentation(-1, &tName, &tDoc, NULL, NULL);
	
	if (tName)
		eType.Attr(TEXT("name"), tName);
	if (tDoc){
		eType.Elem(TEXT("documentation")).TextAsXml(tDoc);
	}



	for(UINT fi=0; fi<ptya->cFuncs; fi++){
		GetFunction(type, eType, fi);
	}

	for(UINT vi=0; vi<ptya->cVars; vi++){
		GetVariable(type, eType, vi);
	}

	type->ReleaseTypeAttr(ptya);
}

int _tmain(int argc, _TCHAR* argv[])
{

	HRESULT res = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	
	if (argc != 3){
		fwprintf(stdout, TEXT("Expect two parameters!\nUsage:\n\t%s <file with tlb> <output xml file>\n"),argv[0]);
		return 0;
	}
	
	ResultXML xml;
	LocRef<ITypeLib> typeLib;

	res = LoadTypeLib(argv[1], &typeLib);
	if (res != S_OK){
		fwprintf(stderr, TEXT("Can't load typelib from %s\n"), argv[1]);
		return 1;
	}

	IXMLDOMProcessingInstruction* pi;
	xml.m_XmlDoc->createProcessingInstruction(TEXT("xml-stylesheet"),TEXT("type='text/xsl' href='static/typelib2xml.xsl'"),&pi);

	xml.m_DocElem.insertBefore(pi);

	UINT ti = typeLib->GetTypeInfoCount();
	SysStr name, docu;
	typeLib->GetDocumentation(-1, &name, &docu, NULL, NULL);

	xml.m_DocElem.Attr(TEXT("name"), name);
	xml.m_DocElem.Elem(TEXT("documentation")).TextAsXml(docu);


	for(UINT i=0; i<ti; i++){
		LocRef<ITypeInfo> type;
		typeLib->GetTypeInfo(i, &type);

		GetType(xml.m_DocElem, type);
	}

	xml.Save(argv[2]);

	return 0;
}

