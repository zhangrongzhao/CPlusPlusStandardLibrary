#include "stdafx.h"
#include "../Chapter02/2simple_alloc.h"
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

protected://Internal typedefs
	//ר��֮�ռ���������ÿ������һ��Ԫ�ش�С
	typedef simple_alloc<value_type, Alloc> data_allocator;

	//ר��֮�ռ���������ÿ������һ��ָ���С
	typedef simple_alloc<pointer, Alloc> map_allocator;

protected:
	//������������ź�deque�Ľṹ������Ԫ�صĳ�ֵ�趨�׵�
	void fill_initialize(size_type n,const value_type& value);
	//���������ź�deque�Ľṹ
	void create_map_and_nodes(size_type num_elements);
	//ֻ�е�finish.cur==finish.last-1ʱ�Żᱻ����
	//Ҳ����˵�����һ��������ֻʣһ������Ԫ�ؿռ�ʱ���Żᱻ����
	void push_back_aux(const value_type& value);
	//ֻ�е�start.cur==start.firstʱ�Żᱻ����
	//Ҳ����˵��һ������û�б��ÿռ�ʱ���Żᱻ����
	void push_front_aux(const value_type& value);

	void reallocate_map(size_type nodes_to_add, bool add_at_front);
	void reserve_map_at_back(size_type nodes_to_add=1)
	{
		if ((nodes_to_add+1)>(map_size-(finish.node-map)))
		{
			//���mapβ�˵Ľڵ㱸�ÿռ䲻�㣬
			//�����������������»�һ��map(����һ������ģ�����ԭ���ģ��ͷ�ԭ����)
			reallocate_map(nodes_to_add,false);
		}
	}
	void reserve_map_at_front(size_type nodes_to_add=1)
	{
		if (nodes_to_add>(start.node-map))
		{
			//���mapǰ�˵Ľڵ㱸�ÿռ䲻�㣬
			//�����������������»�һ��map(����һ������ģ�����ԭ���ģ��ͷ�ԭ����)
			reallocate_map(nodes_to_add,true);
		}
	}
	//ֻ�е�finish.cur==finish.firstʱ�Żᱻ���ã��ͷŻ�����
	void pop_back_aux();
	//ֻ�е�start.cur==start.last-1ʱ�Żᱻ����
	void pop_front_aux();
	//��deque���м�����һ��Ԫ��
	iterator insert_aux(iterator position,const value_type& x);
public:
	deque(int n, const value_type& value) :start(), finish(), map(0), map_size(0)
	{
		fill_initialize(n,value);
	}
	//push_* and pop_*
	void push_back(const value_type& t){
		if (finish.cur!=finish.last-1){
		    //��󻺳��������������ϵ�Ԫ�ر��ÿռ�
			construct(finish.cur,t);//ֱ���ڱ��ÿռ��Ϲ���Ԫ��
			++finish.cur;//������󻺳�����ʹ��״̬
		}else{//���Ԫ��ֻʣһ��Ԫ�ر��ÿռ�
			push_back_aux(t);
		}
	}
	void push_front(const value_type& t){
		if (start.cur!=start.first){//��һ���������б��ÿռ�
			construct(start.cur-1,t);//ֱ���ڱ��ÿռ��Ϲ���Ԫ��
			--start.cur;//������һ��������ʹ��״̬
		}
		else//��һ���������ޱ��ÿռ�
		{
			push_front_aux(t);
		}
	}

	void pop_back(){
		if (finish.cur!=finish.first){
		    //���һ����������һ�������Ԫ��
			--finish.cur;//����ָ�룬�൱���ų������һ��Ԫ��
			destory(finish.cur);//�����Ԫ������
		}
		else
		{
			//��󻺳���û���κ�Ԫ��,�ͷŻ�����
			pop_back_aux();
		}
	}
	void pop_front(){
		if (start.cur!=start.last-1){
		    //��һ����������һ�����߶��Ԫ��
			destory(start.cur);//����һԪ������
			++start.cur;//����ָ���ƶ�����һ��Ԫ�أ��൱���ų���һ��Ԫ��
		}
		else
		{
			//��һ������������һ��Ԫ��
			pop_front_aux();
		}
	}

	void clear();
	//���pos��ָ��Ԫ�ء�posΪ�����
	iterator erase(iterator pos);
	//���[first,last�������ڵ�����Ԫ��
	iterator erase(iterator first,iterator last);
	//��position������һ��Ԫ�أ�ֵΪx
	iterator insert(iterator position,const value_type& x);
};

//������������ź�deque�Ľṹ������Ԫ�صĳ�ֵ�趨�׵�
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::fill_initialize(size_type n,const value_type& value){
	create_map_and_nodes(n);//��deque�Ľṹ�����������ź�
	map_pointer cur;
	__STL_TRY
	{
		//Ϊÿ���ڵ�Ļ������趨��ֵ
		for (cur = start.node; cur < finish.node;++cur)
		{
			//
			uninitialized_fill(*cur,*cur+buffer_size(),value);
		}
		//���һ���ڵ���趨���в�ͬ����Ϊβ�˿����б��ÿռ䣬�������ֵ��
		uninitialized_fill(finish.first,finish.cur,value);
	}
	catch (...){
	   ...
	}
}

template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements){
	//��Ҫ�ڵ���=(Ԫ�ظ���/ÿ�������������ɵ�Ԫ�ظ���)+1
	//����պ������������һ���ڵ�
	size_type num_nodes = num_elements / buffer_size() + 1;

	//һ��mapҪ�������ڵ㡣����8��������ǡ�����ڵ���+2��
	//ǰ���Ԥ��һ��������ʱ����
	map_size = max(initial_map_size(),num_nodes+2);
	//���ó�һ��������map_size���ڵ㡱
	map = map_allocator::allocate(map_size);

	//������nstart��nfinishָ��map��ӵ��֮ȫ���ڵ������������
	//�����������룬��ʹͷβ���˵���������һ����ÿ���ڵ��Ӧһ��������
	map_pointer nstart = map + (map_size - num_nodes) / 2;
	map_pointer nfinish = nstart + num_nodes - 1;

	map_pointer  cur;
	__STL_TRY
	{
		//Ϊmap�ڵ�ÿ�����ýڵ����û����������л���������������deque�Ŀ��ÿռ䣨���һ������������������ԣ��
		for (cur = nstart; cur < nfinish;++cur)
		{
			*cur = allocate_node();
		}
	}
    catch (...)
	{
		//commit or rollback:����ȫ���ɹ�����һ��Ҳ����
	}

	//Ϊdeque�ڵ�����������start��end�趨��ȷ������
	start.set_node(nstart);
	finish.set_node(nfinish);
	start.cur = start.first;//first,cur����public
	finish.cur = finish.first + num_elements%buffer_size();
	//����պ�������������һ���ڵ�
	//��ʱ����curָ���������һ���ڵ㣨����Ӧ������������ʼ��
}

//ֻ�е�finish.cur==finish.last-1ʱ�Żᱻ����
//Ҳ����˵�����һ��������ֻʣһ������Ԫ�ؿռ�ʱ���Żᱻ����
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::push_back_aux(const value_type& value){
	value_type t_copy = t;
	reserve_map_at_back();//������ĳ�����������ػ�һ��map
	*(finish.node + 1) = allocate_node();//����һ���½ڵ㣨��������
	__STL_TRY{
		construct(finish.cur,t_copy);//��Ա��Ԫ������
		finish.set_node(finish.node+1);//�ı�finish������ָ���½ڵ�
		finish.cur = finish.first;//�趨finish��״̬
	}
	__STL_UNWIND(deallocate_node(*(finish.node+1)));
}

//ֻ�е�start.cur==start.firstʱ�Żᱻ����
//Ҳ����˵��һ������û�б��ÿռ�ʱ���Żᱻ����
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::push_front_aux(const value_type& value){
	value_type t_copy = t;
	reserve_map_at_front();//������ĳ������������ػ�һ��map
	*(start.node - 1) = allocate_node();//����һ���½ڵ㣨��������
	__STL_TRY {
		start.set_node(start.node-1);//�ı�start������������ָ���µĽڵ�
		start.cur = start.last - 1;//�趨start��״̬
		construct(start.cur,t_copy);
	}
	catch (...){
	    //"commit or call back":����ȫ���ɹ�����һ������
		start.set_node(start.node+1);//start�������˻�ԭ����һ������
		start.cur = start.first;//��������ǰָ������Ϊ��һ��������firstλ��
		deallocate_node(*(start.node-1));//�ͷ����õ��½ڵ�(�»�����)
		throw;
	}
}

template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add,bool add_at_front)
{
	size_type old_num_nodes = finish.node - start.node + 1;
	size_type new_num_nodes = old_num_nodes + nodes_to_add;

	map_pointer new_nstart;
	if (map_size>2*new_num_nodes)
	{
		new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front?nodes_to_add:0);
		if (new_nstart<start.node)
		{
			copy(start.node,finish.node+1,new_nstart);
		}
		else
		{
			copy_backward(start.node,finish.node+1,new_nstart+old_num_nodes);
		}
	}
	else
	{
		size_type new_map_size = map_size + max(map_size,nodes_to_add) + 2;
		//����һ���ڴ棬׼������mapʹ��
		map_pointer new_map = map_allocator::allocate(new_map_size);
		new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + add_at_front ? nodes_to_add : 0;
		//��ԭmap����copy����
		copy(start.node,finish.node+1,new_nstart);
		//�ͷ�ԭmap
		map_allocator::deallocate(map,map_size);
		//�趨�µ�map����ʼ��ַ���С
		map = new_map;
		map_size = new_map_size;
	}
	//�����趨������start��finish
	start.set_node(new_nstart);
	finish.set_node(new_nstart+old_num_nodes-1);
}

//ֻ�е�finish.cur==finish.firstʱ�Żᱻ���ã��ͷŻ�����
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::pop_back_aux(){
	deallocate_node(finish.first);//�ͷ����һ��������
	finish.set_node(finish.node-1);//����finish��״̬����ָ����һ�������������һ��Ԫ��
	finish.cur = finish.last-1;
	destory(finish.cur);//����Ԫ������
}

template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::pop_front_aux(){
	destory(start.cur);//���������ĵ�һ����Ҳ�����һ����Ψһһ����Ԫ������
	deallocate_node(start.first);//�ͷŵ�һ������
	start.set_node(start.node+1);//����start״̬��ʹָ����һ���������ĵ�һ��Ԫ��
	start.cur = start.first;
}

//ע�⣺������Ҫ����һ��������������deque�Ĳ��ԣ�Ҳ��deque�ĳ�ʼ״̬
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::clear(){
    //���������ͷβ�����ÿһ��������(����һ�����Ǳ�����)
	for (map_pointer node = start.node + 1; node < finish.node;++node){
		//���������ڵ�����Ԫ��������ע�⣬���õ���destory()�ĵڶ����汾
		destory(*node,*node+buffer_size()); 
		//�ͷŻ������ڴ�
		data_allocator::deallocate(*node,buffer_size());
	}
	//������ͷβ����������
	if (start.node!=finish.node){
		destory(start.cur,start.last);//����ͷ�������е�����Ԫ��
		destory(finish.cur,finish.last);//����Ϊ�������е�����Ԫ��
		data_allocator::deallocate(finish.first,buffer_size());//�ͷ�β������
	}else{
		destory(start.first,finish.cur);//����Ψһ�������ڵ�����Ԫ������
		//ע�⣬�����ͷŻ������ռ䣬��Ψһ�Ļ�����������
		finish = start;
	}
}

template<typename T,typename Alloc,size_t BufSize>
deque<T,Alloc,BufSize>::iterator 
deque<T, Alloc, BufSize>::erase(iterator pos){
	iterator next = pos;
	++next;
	difference_type index = pos - start;//�����֮ǰ��Ԫ�ظ���,����iterator�ġ�-���Ĳ��������أ���������������֮��Ĳ�࣬��Ԫ�ظ���
	if (index < (size() >> 1)){//����һλ����ʾ����2��������壺size()��1/2��
	    //�����֮ǰ��Ԫ�ظ����Ƚ��٣����ƶ������֮ǰ��Ԫ��
		copy_backward(start,pos,next);
		pop_front();//�ƶ���ϣ���ǰһ��Ԫ�����࣬ȥ��֮
	}else{
		//�����֮��Ԫ�رȽ���
		copy(next,finish,pos);//���ƶ������֮���Ԫ��
		pop_back();//���һ��Ԫ�����࣬ȥ��
	}
	return start + index;
}

template<typename T,typename Alloc,size_t BufSize>
deque<T,Alloc,BufSize>::iterator
deque<T, Alloc, BufSize>::erase(iterator first,iterator last){
	if (first==start && last==finish){//�����������������deque,
		clear();                    //ֱ�ӵ���clear()����
		return finish;
	}else{
		difference_type n = last - first;//�������ĳ���
		difference_type elems_before = first - start;//�������֮ǰ��Ԫ�ظ���
		if (elems_before<(size()-n)/2){//���ǰ����Ԫ�ظ����Ƚ���:ʣ��Ԫ��������һ�� =��dequeԪ������-������䳤�ȣ�/2
			copy_backward(start,first,last);//��lastλ����Ӻ���ǰcopy�������������
			iterator new_start = start + n;//���deque�������
			destroy(start,new_start);//�ƶ���ϣ���������Ԫ��
			//�ͷ����໺����
			for (map_pointer cur = start.node; cur < new_start.node;++cur){
				data_allocator::deallocate(*cur,buffer_size());
			}
			start = new_start;//�趨deque�������
		}else{//����������󷽵�Ԫ�رȽ���
			copy(last,finish,first);//��ǰ�ƶ���Ԫ�أ������������
			iterator new_finish = finish - n;//����µĽ�����
			destory(new_finish,finish);//��������Ԫ��
			//�ͷ����໺����
			for (map_pointer cur = new_finish.node; cur <= finish.node;++cur){
				data_allocator::deallocate(*cur,buffer_size());
			}
			finish = new_finish;//�����µĽ�����
		}
		return start + elems_before;
	}
}

template<typename T,typename Alloc,size_t BufSize>
deque<T, Alloc, BufSize>::iterator 
insert(iterator position,const value_type& x){
	if (position.cur==start.cur){//����������deque����ǰ��
		push_front(x);
		return start;
	}else if(position.cur==finish.cur){//����������deque����β��
		push_back(x);
		iterator temp = finish;
		--temp;
		return temp;
	}else{
		return insert_aux(position,x);//����insert_auxȥ��
	}
}

template<typename T,typename Alloc,size_t BufSize>
deque<T,Alloc,BufSize>::iterator
deque<T, Alloc, BufSize>::insert_aux(iterator position,const value_type& x){
	difference_type index = positin - start;//�����֮ǰ��Ԫ�ظ���
	value_type x_copy = x;
	if (index<(size()/2)){//��������֮ǰԪ�ظ����Ƚ���
		push_front(front());
		iterator front1 = start;//һ�±�ʾ�Ǻţ�Ȼ�����Ԫ���ƶ�
		++front1;
		iterator front2 = front1;
		++front2;               //ǰ����Ԫ����ͬ����Ǵӵ�����Ԫ��Ϊ��copy����ʼλ��
		pos = start + index;
		iterator pos1 = pos;
		++pos1;//����ԭʼposition��λ��
		copy(front2,pos1,front1);
	}else{//�����֮���Ԫ�ظ����Ƚ���
		push_back(back());//��β��Ԫ�ؼ��������Ԫ����ͬ��Ԫ��
		iterator back1 = finish;
		--back1;
		iterator back2 = back1;
		--back2;
		pos = start + index;
		copy_backward(pos,back2,back1);//Ԫ���ƶ�
	}
	*pos = x_copy;//�ڲ�����趨��ֵ
	return pos;
}