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
	map_pointer map;//指向map,map是块连续空间，其内的每个元素都是一个指针(称为节点)，指向一块缓冲区
	size_type map_size;//map内可容纳多少指针

};