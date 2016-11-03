#include "stdafx.h"
#include "../Chapter02/2stdalloc.cpp"
#include "../Chapter03/3iterator.h"
#include "4list-node.h"

class alloc{ };

//list的iterator设计
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

	link_type node;//迭代器内部要有一个普通指针，指向list节点。

	//constructor
	__list_iterator(link_type x) :node(x){ }
	__list_iterator(){}
	__list_iterator(const iterator& x) :node(x.node){}

	bool operator==(const self& x){ return node == x.node; }
	bool operator!=(const self& x){ return node != x.node; }
	reference operator*() const{ return (*node).data; }
	pointer operator->() const{ return &(operator*()); }//???????

	//对迭代器累加1，就是前进一个节点
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
	link_type node;//只要一个指针，便可表示整个环状双向链表
protected:
	//配置一个节点，并返回
	link_type get_node(){ return list_node_allocator::allocate(); }
	//释放一个节点
	void put_node(link_type p){ list_node_allocator::deallocate(p); }
	//产生（配置并构造）一个节点，带有元素值
	link_type create_node(const T& x){
		link_type p = get_node();
		construct(&p->data, x);//全局函数，构造对象
		return p;
	}
	//销毁（析构并释放）一个节点
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
		distance(begin(),end(),result);//计算两个迭代器之间的差距
		return result;
	}

	//取头结点的内容（元素值）
	reference  front(){ return *begin(); }
	//取尾节点的内容(元素值)
	reference back(){ return *(--end()); }
};