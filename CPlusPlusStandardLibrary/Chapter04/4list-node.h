#include "stdafx.h"
template<typename T>
struct __list_node{
	typedef void* void_pointer;
	void_pointer prev;//����Ϊvoid*,��ʵ��������Ϊ__list_node<T>*,ǰ��ָ��
	void_pointer next;//���ָ��
	T data;
};

