#include "stdafx.h"
#include <cstdlib>
#include "../Chapter03/3iterator.h"

inline size_t __deque_buf_size(size_t n,size_t sz){
	return n != 0 ? n : (sz<512?size_t(512/sz):size_t(1));
}

template<typename T,typename Ref,typename Ptr,size_t BufSize>
struct __deque_iterator{//未继承std::iterator
	typedef __deque_iterator<T, T&, T*, BufSize> iterator;
	typedef __deque_iterator<T, const T&, const T*, BufSize> const_iterator;
	static size_t buffer_size(){ return __deque_buf_size(BufSize,sizeof(T)); }

	//未继承std::iterator,所以自行撰写五个必要的迭代器相应类型
	typedef T value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;
	
	typedef size_t size_type;
	typedef T** map_pointer;

	typedef __deque_iterator self;
	
	//保持与容器的连结
	T* cur;//此迭代器所指之缓冲区中的当前元素
	T* first;//此迭代器所指之缓冲区的头
	T* last;//此迭代器所指之缓冲区的尾
	map_pointer node;//管控中心

protected:
	void set_node(map_pointer new_node){
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size);
	}
public:
	//以下对运算符重载
	reference operator*() const{ return *cur; }
	pointer operator->() const{ return &(operator*()); }//????????????????????????
	difference_type operator-(const self& x) const{
		return difference_type(buffer_size())*(node-x.node-1)+(cur-first)+(x.last-x.cur);
	}

	//prefix:++i
	self& operator++(){
		++cur;//切换至下一个元素
		if (cur==last){//如果已经达到缓冲区的尾端
			set_node(node+1);//切换到下一个节点，即切换到下一个缓冲区的元素
			cur = first;
		}
		return *this;
	}

	//postfix:i++
	self operator++(int){
		self temp = *this;
		++*this;
		return temp;
	}

	//prefix:--i
	self& operator--(){
		if (cur==first){//如果已达缓冲区的头端
			set_node(node-1);
			cur = last;
		}
		--cur;
		return *this;
	}

	//postfix:i--
	self operator--(int){
		self temp = *this;
		--*this;
		return temp;
	}

	//以下实现随机存取，迭代器可以跳跃n个距离
	self& operator+=(difference_type n){
		difference_type  offset = n + (cur - first);//相对于当前缓冲区起始位置的偏移量
		if (offset >= 0 && offset < difference_type(buffer_size)){
            //目标位置在同一缓冲区内
			cur += n;
		}else{
			//目标位置不在同一缓冲区内
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
			//切换至正确的节点（亦即缓冲区）
			set_node(node + node_offset);
			//切换至正确的元素
			cur = first + (offset-node_offset*difference_type(buffer_size()));
		}
		return *this;
	}

	self operator+(difference_type n) const{
		self temp = *this;
		return temp += n;//调用operator+=
	}

	self& operator-=(difference_type n){
		return *this += -n;//调用operator+=
	}

	self operator-(difference_type n) const{
		self temp = *this;
		return temp -= n;//调用operator-=
	}

	//以下实现随机存取，迭代器可以直接跳跃n个距离。
	reference operator[](difference_type n) const{
		return *(*this + n);//1.调用operator+;2.调用operator*
	}

	bool operator==(const self& x) const{
		return cur == x.cur;
	}

	bool operator!=(const self& x) const{
		return !(*this == x);//调用operator==
	}

	bool operator<(const self& x) const{
		return node == x.node ? (cur < x.cur) : (node < x.node);
	}

};