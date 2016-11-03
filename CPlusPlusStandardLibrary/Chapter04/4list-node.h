#include "stdafx.h"
template<typename T>
struct __list_node{
	typedef void* void_pointer;
	void_pointer prev;//类型为void*,其实可以设置为__list_node<T>*,前驱指针
	void_pointer next;//后继指针
	T data;
};

