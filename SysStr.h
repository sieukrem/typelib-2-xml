#pragma once

#include <WTypes.h>
#include <OleAuto.h>
#include <strsafe.h>

class SysStr{
public:
	BSTR str;

	SysStr(TCHAR* value = NULL){
		if (value)
			str = SysAllocString(value);
		else
			str = NULL;
	}

	~SysStr(){
		if (str != NULL)
			SysFreeString(str);
	}

	BSTR* operator&(){
		return &str;
	}

	UINT Length(){
		if (str == NULL)
			return 0;

		return SysStringLen(str);
	}

	SysStr& operator+(TCHAR* value){
		size_t len1 = Length();
		size_t len2;

		if(FAILED(StringCchLength(value,STRSAFE_MAX_CCH, &len2)))
			throw "Count of characters cannot be determined";

		BSTR newStr = SysAllocStringLen(str, len1 + len2);
		SysFreeString(str);
		str = newStr;

		memcpy(str + len1, value, (len2 + 1)*sizeof(OLECHAR));
		return *this;
	}


	operator BSTR(){
		return str;
	}
};