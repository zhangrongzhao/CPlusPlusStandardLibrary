#include "stdafx.h"
#include <cstdlib>
#include "../Chapter03/3iterator.h"

inline size_t __deque_buf_size(size_t n,size_t sz){
	return n != 0 ? n : (sz<512?size_t(512/sz):size_t(1));
}

template<typename T,typename Ref,typename Ptr,size_t BufSize>
struct __deque_iterator{//δ�̳�std::iterator
	typedef __deque_iterator<T, T&, T*, BufSize> iterator;
	typedef __deque_iterator<T, const T&, const T*, BufSize> const_iterator;
	static size_t buffer_size(){ return __deque_buf_size(BufSize,sizeof(T)); }

	//δ�̳�std::iterator,��������׫д�����Ҫ�ĵ�������Ӧ����
	typedef T value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;
	
	typedef size_t size_type;
	typedef T** map_pointer;

	typedef __deque_iterator self;
	
	//����������������
	T* cur;//�˵�������ָ֮�������еĵ�ǰԪ��
	T* first;//�˵�������ָ֮��������ͷ
	T* last;//�˵�������ָ֮��������β
	map_pointer node;//�ܿ�����

protected:
	void set_node(map_pointer new_node){
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size);
	}
public:
	//���¶����������
	reference operator*() const{ return *cur; }
	pointer operator->() const{ return &(operator*()); }//????????????????????????
	difference_type operator-(const self& x) const{
		return difference_type(buffer_size())*(node-x.node-1)+(cur-first)+(x.last-x.cur);
	}

	//prefix:++i
	self& operator++(){
		++cur;//�л�����һ��Ԫ��
		if (cur==last){//����Ѿ��ﵽ��������β��
			set_node(node+1);//�л�����һ���ڵ㣬���л�����һ����������Ԫ��
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
		if (cur==first){//����Ѵﻺ������ͷ��
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

	//����ʵ�������ȡ��������������Ծn������
	self& operator+=(difference_type n){
		difference_type  offset = n + (cur - first);//����ڵ�ǰ��������ʼλ�õ�ƫ����
		if (offset >= 0 && offset < difference_type(buffer_size)){
            //Ŀ��λ����ͬһ��������
			cur += n;
		}else{
			//Ŀ��λ�ò���ͬһ��������
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
			//�л�����ȷ�Ľڵ㣨�༴��������
			set_node(node + node_offset);
			//�л�����ȷ��Ԫ��
			cur = first + (offset-node_offset*difference_type(buffer_size()));
		}
		return *this;
	}

	self operator+(difference_type n) const{
		self temp = *this;
		return temp += n;//����operator+=
	}

	self& operator-=(difference_type n){
		return *this += -n;//����operator+=
	}

	self operator-(difference_type n) const{
		self temp = *this;
		return temp -= n;//����operator-=
	}

	//����ʵ�������ȡ������������ֱ����Ծn�����롣
	reference operator[](difference_type n) const{
		return *(*this + n);//1.����operator+;2.����operator*
	}

	bool operator==(const self& x) const{
		return cur == x.cur;
	}

	bool operator!=(const self& x) const{
		return !(*this == x);//����operator==
	}

	bool operator<(const self& x) const{
		return node == x.node ? (cur < x.cur) : (node < x.node);
	}

};