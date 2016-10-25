#include "stdafx.h"

template<typename T>
class auto_ptr{
private:
	T* pointee;
public:
	explicit auto_ptr(T* p = 0) :pointee(p){ }
	~auto_ptr(){ delete pointee; }

	//copy constructor
	template<typename U>
	auto_ptr(auto_ptr<U>& rhs) : pointee(rhs.release()){ }

	//assignment
	template<typename U>
	auto_ptr<T>& operator=(auto_ptr<U>& rhs){
		if (this != &rhs){
			rhs.release();
		}
		return *this;
	}

	T& operator*() const{ return *pointee; }
	T* operator->() const{ return pointee; }
	T* get() const{ return pointee; }
};