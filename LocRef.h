#pragma once

template<class T>
class LocRef{
public:
	T* ref;
	LocRef(T* _ref = nullptr){
		ref = _ref;
	}

	~LocRef(){
		if (ref)
			ref->Release();
	}

	operator T*(){
		return ref;
	}

	T** operator&(){
		return &ref;
	}

	T* operator->(){
		return ref;
	}
};
