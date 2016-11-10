#include "stdafx.h"
#include "4deque-iterator.h"

class alloc{ };

//��__deque_buf_size()��BufsizeĬ��ֵΪ0��Ψһ������Ϊ������ĳЩ�������ڴ�������ʽ(constant expression)ʱ�ĳ���
//ȱʡʹ��allocΪ������
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
	//Ԫ�ص�ָ���ָ��(pointer of pointer of T)
	typedef pointer* map_pointer;
	
protected://data members
	map_pointer map;//ָ��map,map�ǿ������ռ䣬���ڵ�ÿ��Ԫ�ض���һ��ָ��(��Ϊ�ڵ�)��ָ��һ�黺����
	size_type map_size;//map�ڿ����ɶ���ָ��

	iterator start;//���ֵ�һ���ڵ�
	iterator finish;//�������һ���ڵ�

public://Basic accessors
	iterator begin(){ return start; }
	iterator end(){ return finish; }

	reference operator[](size_type n){
		return start[difference_type(n)];//����__deque_iterator<>::operator[]
	}
	reference front(){ return *start; }//����__deque_iterator<>::operator*
	reference end(){
		iterator temp = finish;
		--temp;//����__deque_iterator<>::operator--
		return *temp;//����__deque_iterator<>::operator*
		//�������кβ���Ϊ�� return *(finish-1);
		//��Ϊ_deque_iterator<>û��Ϊ��finish-1�����������
	}

	//���������������;��,����ֵ��Ϻ��﷨
	size_type size() const{
		return finish - start;;//����__deque_iterator<>::operator-
	}

	size_type max_size() const{
		return size_type(-1);
	}

	bool empty() const{
		return finish == start;
	}
};