#include "stdafx.h"
#include "../Chapter02/2stdalloc.cpp"
#include "../Chapter03/3iterator.h"
#include "4list-node.h"

class alloc{ };

//list��iterator���
template<typename T,typename Ref,typename Ptr>
struct __list_iterator{
	typedef __list_iterator<T, T&, T*> iterator;
	typedef __list_iterator<T, Ref, Ptr> self;

	typedef bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef ptrdiff_t difference_type;

	typedef size_t size_type;
	typedef __list_node<T>* link_type;

	link_type node;//�������ڲ�Ҫ��һ����ָͨ�룬ָ��list�ڵ㡣

	//constructor
	__list_iterator(link_type x) :node(x){ }
	__list_iterator(){}
	__list_iterator(const iterator& x) :node(x.node){}

	bool operator==(const self& x){ return node == x.node; }
	bool operator!=(const self& x){ return node != x.node; }
	reference operator*() const{ return (*node).data; }
	pointer operator->() const{ return &(operator*()); }//???????

	//�Ե������ۼ�1������ǰ��һ���ڵ�
	//prefix
	self& operator++(){
		node = (link_type)((*node).next);
		return *this;
	}
	//postfix
	self operator++(int){
		self temp = *this;
		node = (link_type)((*node).next);
		return temp;
	}

	//prefix
	self& operator--(){
		node = (link_type)((*node).prev);
		return *this;
	}
	//postfix
	self operator--(int){
		self temp = *this;
		node = (link_type)((*node).prev);
		return temp;
	}
};

template<typename T,typename Alloc=alloc>
class list{
protected:
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef size_t size_type;
	typedef __list_node<T> list_node;
	typedef simple_alloc<list_node, Alloc> list_node_allocator;
	//typedef __list_iterator<T, T&, T*> iterator;
public:
	typedef list_node* link_type;
protected:
	link_type node;//ֻҪһ��ָ�룬��ɱ�ʾ������״˫������
protected:
	//����һ���ڵ㣬������
	link_type get_node(){ return list_node_allocator::allocate(); }
	//�ͷ�һ���ڵ�
	void put_node(link_type p){ list_node_allocator::deallocate(p); }
	//���������ò����죩һ���ڵ㣬����Ԫ��ֵ
	link_type create_node(const T& x){
		link_type p = get_node();
		construct(&p->data, x);//ȫ�ֺ������������
		return p;
	}
	//���٣��������ͷţ�һ���ڵ�
	void destroy_node(link_type p){
		destory(&p->data);
		put_node(p);
	}
public:
	iterator begin(){ return (link_type)((*node).next); }
	iterator end(){ return node; }
	bool empty() const{ return node->next == node; }
	size_type size() const{
		size_type result = 0;
		distance(begin(),end(),result);//��������������֮��Ĳ��
		return result;
	}

	//ȡͷ�������ݣ�Ԫ��ֵ��
	reference  front(){ return *begin(); }
	//ȡβ�ڵ������(Ԫ��ֵ)
	reference back(){ return *(--end()); }
};