#include "stdafx.h"

class alloc{ };

template<typename T,typename Alloc=alloc,size_t BufSize=0>
class deque{
public://Basic types
	typedef T value_type;
	typedef value_type* pointer;
protected://internal typedefs
	typedef pointer* map_pointer;
	typedef size_t size_type;
protected:
	map_pointer map;//ָ��map,map�ǿ������ռ䣬���ڵ�ÿ��Ԫ�ض���һ��ָ��(��Ϊ�ڵ�)��ָ��һ�黺����
	size_type map_size;//map�ڿ����ɶ���ָ��

};