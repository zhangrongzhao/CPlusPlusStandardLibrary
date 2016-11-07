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
	typedef __list_iterator<T, T&, T*> iterator;
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
	void empty_initialize(){//链表空节点头尾均指向自身且不设定任何元素，表示空链表
		node = get_node();
		node->prev = node;
		node->next = node;
	}

	//在迭代器position所指节点处插入一个节点
	iterator insert(iterator position, const T& x){
		link_type temp = create_node(x);//产生一个新节点，设其内容为x
		//调整双向指针，使temp插入进去
		temp->prev = (position.node)->prev;
		temp->next = position.node;
		(link_type(temp->prev))->next = temp;
		position.node->prev = temp;
		return temp;
	}
	//移除迭代器position所指结点,返回迭代器指向下一个节点
	iterator erase(iterator position){
		link_type prev_node = (link_type)(position.node->prev);
		link_type next_node = (link_type)(position.node->next);
		prev_node->prev = next_node;
		next_node->next = prev_node;
		destroy_node(position.node);
		return iterator(next_node);
	}

	//将[first,last)内的所有元素移动到position之前
	void transfer(iterator position, iterator first, iterator last);
public:
	//constructor
	//产生一个空链表
	list(){ empty_initialize(); }

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
	//插入新元素于尾端,作为尾节点
	void push_back(const T& x){ insert(end(),x); }
	//插入一个节点，作为头节点
	void push_front(const T& x){ insert(begin(),x); }

	//移除头结点
	void pop_front(){ erase(begin()); }
	//移除尾节点
	void pop_back(){ 
		iterator temp = end();
		erase(--temp); 
	}
	//清除所有节点（整个链表）
	void clear();

	//将数值为value之所有元素移除
	void remove(const T& value);

	//移除数值相同的连续元素，注意：只有连续而相同的连续元素，才会被移除剩下一个
	void unique();

	//将x结合于position所指位置之前，x必须不同于*this
	void splice(iterator position,list& x);
	//将i所指元素结合于position所指元素之前，position和i指向同一个list
	void splice(iterator position,list&,iterator i);
	//将[first,last)内的所有元素结合于position所指位置之前，position和[first,last)可指向同一个list，但position不能位于[first,last)之内
	void splice(iterator position,list& x,iterator first,iterator last);

	void merge(list<T,Alloc>& x);
	//将*this的内容逆向重置
	void reverse();
	//不能使用STL的sort()算法，必须使用自己的sort()的member function.因为STL算法的sort()只接受RandomAccessIterator，本函数采用quick sort.
	void sort();
};

template<typename T,typename Alloc>
void list<T, Alloc>::clear(){
	link_type current = node->next();//获取头节点
	while (current!=node){
		link_type temp = current;
		current = current->next;
		destroy_node(temp);
	}
	//恢复node为原始状态
	node->next = node;
	node->prev = node;
}

template<typename T,typename Alloc>
void list<T, Alloc>::remove(const T& value){
	iterator first = begin();
	iterator last = end();
	while (first!=last){
		iterator next = first;
		++next;
		if (*first == value){ erase(first); }//找到后清除
		first = next;
	}
}

template<typename T,typename Alloc>
void list<T, Alloc>::unique(){
	iterator first = begin();
	iterator last = end();
	if (first == last) return;//空链表，什么都不必做
	iterator next = first;
	while (++next!=last){//遍历每一个节点
		if (*first == *next){//如果在此区段内有相同的元素
			erase(next);//移除
		}else{
			first = next;//调整指针
		}
		next = first;//修正区段范围
	};
}

template<typename T,typename Alloc>
void list<T, Alloc>::transfer(iterator position,iterator first,iterator last){
	if (position != last){
		link_type nodeFirst = first.node;
		link_type nodeLast = last.node;
		link_type nodePosition = position.node;

		link_type nodePreFirst = (link_type)((*(first.node)).prev);
		link_type nodePreLast = (link_type)((*(last.node)).prev);
		link_type nodePrePosition = (link_type)((*(position.node)).prev);

		//两个双向链表的顺向指针调整
		nodePreLast->next = nodePosition;
		nodePrePosition->next = nodeFirst;
		nodePreFirst->next = nodeLast;

		//两个双向链表的逆向指针调整
		nodePosition->prev = nodePreLast;
		nodeFirst->prev = nodePrePosition;
		nodeLast->prev = nodePreFirst;
	}
}

template<typename T,typename Alloc>
void list<T, Alloc>::splice(iterator position,list& x){
	if (!x.empty()){
		transfer(position,x.begin(),x.end());
	}
}

template<typename T,typename Alloc>
void list<T, Alloc>::splice(iterator position,list&,iterator i){
	iterator j = i;
	++j;
	if (position == i || position == j){
		return;
	}
	transfer(position,i,j);
}

template<typename T,typename Alloc>
void list<T, Alloc>::splice(iterator position,list&,iterator first,iterator last){
	if (first != last){
		transfer(position,first,last);
	}
}

template<typename T,typename Alloc>
void list<T, Alloc>::merge(list<T,Alloc>& x){
	iterator first1 = begin();
	iterator last1 = end();
	iterator first2 = x.begin();
	iterator last2 = x.end();

	//注意：前提是，两个lists都已经过递增排序
	while (first1 != last1 && first2 != last2){
		if (*first2<*first1){
			iterator next = first2;
			transfer(first1,first2,++next);
			first2 = next;
		}else{
			++first1;
		}

		if (first2!=last2){
			transfer(last1,first2,last2);
		}
	}
}

template<typename T,typename Alloc>
void list<T, Alloc>::reverse(){
	//以下判断，如果是空链表，或仅有一个元素，就不进行任何操作
	//可以使用size()==0或者size()==1来判断，虽然可以，但是速度比较慢
	if (node->next==node||(link_type(node->next))->next==node){
		return;
	}
	iterator first = begin();
	while (first!=end())
	{
		iterator old = first;
		++first;
		transfer(begin(),old,first);
	}
}

//不能使用STL的sort()算法，必须使用自己的sort()的member function.
//因为STL算法的sort()只接受RandomAccessIterator，
//本函数采用quick sort.快排算法（基本思路：合并有序链表）
template<typename T,typename Alloc>
void list<T, Alloc>::sort(){
	//以下判断，如果是空链表，或仅有一个元素，就不进行任何操作
	//可以使用size()==0或者size()==1判断，虽然可以，但是速度较慢
	if (node->next == node || ((link_type)(node->next))->next == node){ return; }
	//一些新的lists作为数据缓冲区
	list<T, Alloc> carry;
	list<T, Alloc> counter[64];
	int fill = 0;
	while (!empty()){
		carry.splice(carry.begin(),*this,begin());//从原链表中获取一个节点
		int i = 0;
		while (i < fill && !counter[i].empty()){
			counter[i].merge(carry); // 合并carry链表到counter[i]
			carry.swap(counter[i++]);// 交换counter[i]到carry.
		}
		carry.swap(counter[i]);//交换carry到counter
		if (i == fill)
			++fill;
	}
	for (int i = 1; i < fill;++i){//合并counter中的fill个链表
		counter[i].merge(counter[i-1]);
	}
	swap(counter[fill-1]);//和当前链表交换，获取最终的排序后的链表
}