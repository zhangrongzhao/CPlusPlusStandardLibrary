#include "stdafx.h"
#include "4deque-iterator.h"

class alloc{ };

//见__deque_buf_size()。Bufsize默认值为0的唯一理由是为了闪避某些编译器在处理常数算式(constant expression)时的臭虫
//缺省使用alloc为配置器
template<typename T,typename Alloc=alloc,size_t BufSize=0>
class deque{
public://Basic types
	typedef T value_type;
	typedef value_type& reference;
	typedef value_type* pointer;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;

public://iterators
	typedef __deque_iterator<T, T&, T*, BufSize> iterator;

protected://internal typedefs
	//元素的指针的指针(pointer of pointer of T)
	typedef pointer* map_pointer;
	
protected://data members
	map_pointer map;//指向map,map是块连续空间，其内的每个元素都是一个指针(称为节点)，指向一块缓冲区
	size_type map_size;//map内可容纳多少指针

	iterator start;//表现第一个节点
	iterator finish;//表现最后一个节点

public://Basic accessors
	iterator begin(){ return start; }
	iterator end(){ return finish; }

	reference operator[](size_type n){
		return start[difference_type(n)];//调用__deque_iterator<>::operator[]
	}
	reference front(){ return *start; }//调用__deque_iterator<>::operator*
	reference end(){
		iterator temp = finish;
		--temp;//调用__deque_iterator<>::operator--
		return *temp;//调用__deque_iterator<>::operator*
		//以上三行何不改为： return *(finish-1);
		//因为_deque_iterator<>没有为（finish-1）定义运算符
	}

	//下行最后有两个‘;’,虽奇怪但合乎语法
	size_type size() const{
		return finish - start;;//调用__deque_iterator<>::operator-
	}

	size_type max_size() const{
		return size_type(-1);
	}

	bool empty() const{
		return finish == start;
	}
};