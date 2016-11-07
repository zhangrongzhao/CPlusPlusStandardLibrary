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
	typedef __list_iterator<T, T&, T*> iterator;
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
	void empty_initialize(){//����սڵ�ͷβ��ָ�������Ҳ��趨�κ�Ԫ�أ���ʾ������
		node = get_node();
		node->prev = node;
		node->next = node;
	}

	//�ڵ�����position��ָ�ڵ㴦����һ���ڵ�
	iterator insert(iterator position, const T& x){
		link_type temp = create_node(x);//����һ���½ڵ㣬��������Ϊx
		//����˫��ָ�룬ʹtemp�����ȥ
		temp->prev = (position.node)->prev;
		temp->next = position.node;
		(link_type(temp->prev))->next = temp;
		position.node->prev = temp;
		return temp;
	}
	//�Ƴ�������position��ָ���,���ص�����ָ����һ���ڵ�
	iterator erase(iterator position){
		link_type prev_node = (link_type)(position.node->prev);
		link_type next_node = (link_type)(position.node->next);
		prev_node->prev = next_node;
		next_node->next = prev_node;
		destroy_node(position.node);
		return iterator(next_node);
	}

	//��[first,last)�ڵ�����Ԫ���ƶ���position֮ǰ
	void transfer(iterator position, iterator first, iterator last);
public:
	//constructor
	//����һ��������
	list(){ empty_initialize(); }

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
	//������Ԫ����β��,��Ϊβ�ڵ�
	void push_back(const T& x){ insert(end(),x); }
	//����һ���ڵ㣬��Ϊͷ�ڵ�
	void push_front(const T& x){ insert(begin(),x); }

	//�Ƴ�ͷ���
	void pop_front(){ erase(begin()); }
	//�Ƴ�β�ڵ�
	void pop_back(){ 
		iterator temp = end();
		erase(--temp); 
	}
	//������нڵ㣨��������
	void clear();

	//����ֵΪvalue֮����Ԫ���Ƴ�
	void remove(const T& value);

	//�Ƴ���ֵ��ͬ������Ԫ�أ�ע�⣺ֻ����������ͬ������Ԫ�أ��Żᱻ�Ƴ�ʣ��һ��
	void unique();

	//��x�����position��ָλ��֮ǰ��x���벻ͬ��*this
	void splice(iterator position,list& x);
	//��i��ָԪ�ؽ����position��ָԪ��֮ǰ��position��iָ��ͬһ��list
	void splice(iterator position,list&,iterator i);
	//��[first,last)�ڵ�����Ԫ�ؽ����position��ָλ��֮ǰ��position��[first,last)��ָ��ͬһ��list����position����λ��[first,last)֮��
	void splice(iterator position,list& x,iterator first,iterator last);

	void merge(list<T,Alloc>& x);
	//��*this��������������
	void reverse();
	//����ʹ��STL��sort()�㷨������ʹ���Լ���sort()��member function.��ΪSTL�㷨��sort()ֻ����RandomAccessIterator������������quick sort.
	void sort();
};

template<typename T,typename Alloc>
void list<T, Alloc>::clear(){
	link_type current = node->next();//��ȡͷ�ڵ�
	while (current!=node){
		link_type temp = current;
		current = current->next;
		destroy_node(temp);
	}
	//�ָ�nodeΪԭʼ״̬
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
		if (*first == value){ erase(first); }//�ҵ������
		first = next;
	}
}

template<typename T,typename Alloc>
void list<T, Alloc>::unique(){
	iterator first = begin();
	iterator last = end();
	if (first == last) return;//������ʲô��������
	iterator next = first;
	while (++next!=last){//����ÿһ���ڵ�
		if (*first == *next){//����ڴ�����������ͬ��Ԫ��
			erase(next);//�Ƴ�
		}else{
			first = next;//����ָ��
		}
		next = first;//�������η�Χ
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

		//����˫�������˳��ָ�����
		nodePreLast->next = nodePosition;
		nodePrePosition->next = nodeFirst;
		nodePreFirst->next = nodeLast;

		//����˫�����������ָ�����
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

	//ע�⣺ǰ���ǣ�����lists���Ѿ�����������
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
	//�����жϣ�����ǿ����������һ��Ԫ�أ��Ͳ������κβ���
	//����ʹ��size()==0����size()==1���жϣ���Ȼ���ԣ������ٶȱȽ���
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

//����ʹ��STL��sort()�㷨������ʹ���Լ���sort()��member function.
//��ΪSTL�㷨��sort()ֻ����RandomAccessIterator��
//����������quick sort.�����㷨������˼·���ϲ���������
template<typename T,typename Alloc>
void list<T, Alloc>::sort(){
	//�����жϣ�����ǿ����������һ��Ԫ�أ��Ͳ������κβ���
	//����ʹ��size()==0����size()==1�жϣ���Ȼ���ԣ������ٶȽ���
	if (node->next == node || ((link_type)(node->next))->next == node){ return; }
	//һЩ�µ�lists��Ϊ���ݻ�����
	list<T, Alloc> carry;
	list<T, Alloc> counter[64];
	int fill = 0;
	while (!empty()){
		carry.splice(carry.begin(),*this,begin());//��ԭ�����л�ȡһ���ڵ�
		int i = 0;
		while (i < fill && !counter[i].empty()){
			counter[i].merge(carry); // �ϲ�carry����counter[i]
			carry.swap(counter[i++]);// ����counter[i]��carry.
		}
		carry.swap(counter[i]);//����carry��counter
		if (i == fill)
			++fill;
	}
	for (int i = 1; i < fill;++i){//�ϲ�counter�е�fill������
		counter[i].merge(counter[i-1]);
	}
	swap(counter[fill-1]);//�͵�ǰ����������ȡ���յ�����������
}