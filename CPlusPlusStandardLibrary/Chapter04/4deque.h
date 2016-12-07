#include "stdafx.h"
#include "../Chapter02/2simple_alloc.h"
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

protected://Internal typedefs
	//专属之空间配置器，每次配置一个元素大小
	typedef simple_alloc<value_type, Alloc> data_allocator;

	//专属之空间配置器，每次配置一个指针大小
	typedef simple_alloc<pointer, Alloc> map_allocator;

protected:
	//负责产生并安排好deque的结构，并将元素的初值设定妥当
	void fill_initialize(size_type n,const value_type& value);
	//产生并安排好deque的结构
	void create_map_and_nodes(size_type num_elements);
	//只有当finish.cur==finish.last-1时才会被调用
	//也就是说当最后一个缓冲区只剩一个备用元素空间时，才会被调用
	void push_back_aux(const value_type& value);
	//只有当start.cur==start.first时才会被调用
	//也就是说第一缓冲区没有备用空间时，才会被调用
	void push_front_aux(const value_type& value);

	void reallocate_map(size_type nodes_to_add, bool add_at_front);
	void reserve_map_at_back(size_type nodes_to_add=1)
	{
		if ((nodes_to_add+1)>(map_size-(finish.node-map)))
		{
			//如果map尾端的节点备用空间不足，
			//符合以上条件则重新换一个map(配置一个更大的，拷贝原来的，释放原来的)
			reallocate_map(nodes_to_add,false);
		}
	}
	void reserve_map_at_front(size_type nodes_to_add=1)
	{
		if (nodes_to_add>(start.node-map))
		{
			//如果map前端的节点备用空间不足，
			//符合以上条件则重新换一个map(配置一个更大的，拷贝原来的，释放原来的)
			reallocate_map(nodes_to_add,true);
		}
	}
	//只有当finish.cur==finish.first时才会被调用，释放缓冲区
	void pop_back_aux();
	//只有当start.cur==start.last-1时才会被调用
	void pop_front_aux();
	//在deque的中间点插入一个元素
	iterator insert_aux(iterator position,const value_type& x);
public:
	deque(int n, const value_type& value) :start(), finish(), map(0), map_size(0)
	{
		fill_initialize(n,value);
	}
	//push_* and pop_*
	void push_back(const value_type& t){
		if (finish.cur!=finish.last-1){
		    //最后缓冲区尚有两个以上的元素备用空间
			construct(finish.cur,t);//直接在备用空间上构造元素
			++finish.cur;//调整最后缓冲区的使用状态
		}else{//最后元素只剩一个元素备用空间
			push_back_aux(t);
		}
	}
	void push_front(const value_type& t){
		if (start.cur!=start.first){//第一缓冲区尚有备用空间
			construct(start.cur-1,t);//直接在备用空间上构造元素
			--start.cur;//调整第一缓冲区的使用状态
		}
		else//第一缓冲区已无备用空间
		{
			push_front_aux(t);
		}
	}

	void pop_back(){
		if (finish.cur!=finish.first){
		    //最后一个缓冲区有一个或更多元素
			--finish.cur;//调整指针，相当于排除了最后一个元素
			destory(finish.cur);//将最后元素析构
		}
		else
		{
			//最后缓冲区没有任何元素,释放缓冲区
			pop_back_aux();
		}
	}
	void pop_front(){
		if (start.cur!=start.last-1){
		    //第一缓冲区内有一个或者多个元素
			destory(start.cur);//将第一元素析构
			++start.cur;//调整指针移动至下一个元素，相当于排除第一个元素
		}
		else
		{
			//第一个缓冲区仅有一个元素
			pop_front_aux();
		}
	}

	void clear();
	//清除pos所指的元素。pos为清除点
	iterator erase(iterator pos);
	//清除[first,last）区间内的所有元素
	iterator erase(iterator first,iterator last);
	//在position处插入一个元素，值为x
	iterator insert(iterator position,const value_type& x);
};

//负责产生并安排好deque的结构，并将元素的初值设定妥当
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::fill_initialize(size_type n,const value_type& value){
	create_map_and_nodes(n);//把deque的结构都产生并安排好
	map_pointer cur;
	__STL_TRY
	{
		//为每个节点的缓冲区设定初值
		for (cur = start.node; cur < finish.node;++cur)
		{
			//
			uninitialized_fill(*cur,*cur+buffer_size(),value);
		}
		//最后一个节点的设定稍有不同（因为尾端可能有备用空间，不必设初值）
		uninitialized_fill(finish.first,finish.cur,value);
	}
	catch (...){
	   ...
	}
}

template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements){
	//需要节点数=(元素个数/每个缓冲区可容纳的元素个数)+1
	//如果刚好整除，会多配一个节点
	size_type num_nodes = num_elements / buffer_size() + 1;

	//一个map要管理几个节点。最少8个，最多是“所需节点数+2”
	//前后各预留一个，扩充时可用
	map_size = max(initial_map_size(),num_nodes+2);
	//配置出一个“具有map_size个节点”
	map = map_allocator::allocate(map_size);

	//以下令nstart和nfinish指向map所拥有之全部节点的最中央区段
	//保持在最中央，可使头尾两端的扩充能量一样大，每个节点对应一个缓冲区
	map_pointer nstart = map + (map_size - num_nodes) / 2;
	map_pointer nfinish = nstart + num_nodes - 1;

	map_pointer  cur;
	__STL_TRY
	{
		//为map内的每个现用节点配置缓冲区，所有缓冲区加起来就是deque的可用空间（最后一个缓冲区可能留有余裕）
		for (cur = nstart; cur < nfinish;++cur)
		{
			*cur = allocate_node();
		}
	}
    catch (...)
	{
		//commit or rollback:若非全部成功，就一个也不留
	}

	//为deque内的两个迭代器start和end设定正确的内容
	start.set_node(nstart);
	finish.set_node(nfinish);
	start.cur = start.first;//first,cur都是public
	finish.cur = finish.first + num_elements%buffer_size();
	//如果刚好整除，会多分配一个节点
	//此时即令cur指向这多分配的一个节点（所对应缓冲区）的起始处
}

//只有当finish.cur==finish.last-1时才会被调用
//也就是说当最后一个缓冲区只剩一个备用元素空间时，才会被调用
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::push_back_aux(const value_type& value){
	value_type t_copy = t;
	reserve_map_at_back();//若符合某种条件必须重换一个map
	*(finish.node + 1) = allocate_node();//配置一个新节点（缓冲区）
	__STL_TRY{
		construct(finish.cur,t_copy);//针对标的元素设置
		finish.set_node(finish.node+1);//改变finish，令其指向新节点
		finish.cur = finish.first;//设定finish的状态
	}
	__STL_UNWIND(deallocate_node(*(finish.node+1)));
}

//只有当start.cur==start.first时才会被调用
//也就是说第一缓冲区没有备用空间时，才会被调用
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::push_front_aux(const value_type& value){
	value_type t_copy = t;
	reserve_map_at_front();//若符合某种条件则必须重换一个map
	*(start.node - 1) = allocate_node();//配置一个新节点（缓冲区）
	__STL_TRY {
		start.set_node(start.node-1);//改变start迭代器，另其指向新的节点
		start.cur = start.last - 1;//设定start的状态
		construct(start.cur,t_copy);
	}
	catch (...){
	    //"commit or call back":若非全部成功，就一个不留
		start.set_node(start.node+1);//start迭代器退回原来第一缓冲区
		start.cur = start.first;//迭代器当前指针设置为第一缓冲区的first位置
		deallocate_node(*(start.node-1));//释放配置的新节点(新缓冲区)
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
		//配置一块内存，准备给新map使用
		map_pointer new_map = map_allocator::allocate(new_map_size);
		new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + add_at_front ? nodes_to_add : 0;
		//把原map内容copy过来
		copy(start.node,finish.node+1,new_nstart);
		//释放原map
		map_allocator::deallocate(map,map_size);
		//设定新的map的起始地址与大小
		map = new_map;
		map_size = new_map_size;
	}
	//重新设定迭代器start和finish
	start.set_node(new_nstart);
	finish.set_node(new_nstart+old_num_nodes-1);
}

//只有当finish.cur==finish.first时才会被调用，释放缓冲区
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::pop_back_aux(){
	deallocate_node(finish.first);//释放最后一个缓冲区
	finish.set_node(finish.node-1);//调整finish的状态，是指向上一个缓冲区的最后一个元素
	finish.cur = finish.last-1;
	destory(finish.cur);//将该元素析构
}

template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::pop_front_aux(){
	destory(start.cur);//将缓冲区的第一个（也是最后一个，唯一一个）元素析构
	deallocate_node(start.first);//释放第一缓冲区
	start.set_node(start.node+1);//调整start状态，使指向下一个缓冲区的第一个元素
	start.cur = start.first;
}

//注意：最终需要保留一个缓冲区，这是deque的策略，也是deque的初始状态
template<typename T,typename Alloc,size_t BufSize>
void deque<T, Alloc, BufSize>::clear(){
    //以下是针对头尾以外的每一个缓冲区(他们一定都是饱满的)
	for (map_pointer node = start.node + 1; node < finish.node;++node){
		//将缓冲区内的所有元素析构。注意，调用的是destory()的第二个版本
		destory(*node,*node+buffer_size()); 
		//释放缓冲区内存
		data_allocator::deallocate(*node,buffer_size());
	}
	//至少有头尾两个缓冲区
	if (start.node!=finish.node){
		destory(start.cur,start.last);//析构头缓冲区中的所有元素
		destory(finish.cur,finish.last);//析构为缓冲区中的所有元素
		data_allocator::deallocate(finish.first,buffer_size());//释放尾缓冲区
	}else{
		destory(start.first,finish.cur);//将此唯一缓冲区内的所有元素析构
		//注意，并不释放缓冲区空间，这唯一的缓冲区将保留
		finish = start;
	}
}

template<typename T,typename Alloc,size_t BufSize>
deque<T,Alloc,BufSize>::iterator 
deque<T, Alloc, BufSize>::erase(iterator pos){
	iterator next = pos;
	++next;
	difference_type index = pos - start;//清除点之前的元素个数,调用iterator的“-”的操作符重载，计算两个迭代器之间的差距，即元素个数
	if (index < (size() >> 1)){//右移一位，表示除以2，表达意义：size()的1/2处
	    //清除点之前的元素个数比较少，就移动清除点之前的元素
		copy_backward(start,pos,next);
		pop_front();//移动完毕，最前一个元素冗余，去除之
	}else{
		//清除点之后元素比较少
		copy(next,finish,pos);//就移动清除点之后的元素
		pop_back();//最后一个元素冗余，去除
	}
	return start + index;
}

template<typename T,typename Alloc,size_t BufSize>
deque<T,Alloc,BufSize>::iterator
deque<T, Alloc, BufSize>::erase(iterator first,iterator last){
	if (first==start && last==finish){//如果清除区间就是整个deque,
		clear();                    //直接调用clear()即可
		return finish;
	}else{
		difference_type n = last - first;//清除区间的长度
		difference_type elems_before = first - start;//清除区间之前的元素个数
		if (elems_before<(size()-n)/2){//如果前方的元素个数比较少:剩余元素总数的一半 =（deque元素总数-清除区间长度）/2
			copy_backward(start,first,last);//从last位置起从后往前copy，覆盖清除区间
			iterator new_start = start + n;//标记deque的新起点
			destroy(start,new_start);//移动完毕，析构冗余元素
			//释放冗余缓冲区
			for (map_pointer cur = start.node; cur < new_start.node;++cur){
				data_allocator::deallocate(*cur,buffer_size());
			}
			start = new_start;//设定deque的新起点
		}else{//如果清除区间后方的元素比较少
			copy(last,finish,first);//向前移动后方元素，覆盖清除区间
			iterator new_finish = finish - n;//标记新的结束点
			destory(new_finish,finish);//析构冗余元素
			//释放冗余缓冲区
			for (map_pointer cur = new_finish.node; cur <= finish.node;++cur){
				data_allocator::deallocate(*cur,buffer_size());
			}
			finish = new_finish;//设置新的结束点
		}
		return start + elems_before;
	}
}

template<typename T,typename Alloc,size_t BufSize>
deque<T, Alloc, BufSize>::iterator 
insert(iterator position,const value_type& x){
	if (position.cur==start.cur){//如果插入点是deque的最前端
		push_front(x);
		return start;
	}else if(position.cur==finish.cur){//如果插入点是deque的最尾端
		push_back(x);
		iterator temp = finish;
		--temp;
		return temp;
	}else{
		return insert_aux(position,x);//交给insert_aux去做
	}
}

template<typename T,typename Alloc,size_t BufSize>
deque<T,Alloc,BufSize>::iterator
deque<T, Alloc, BufSize>::insert_aux(iterator position,const value_type& x){
	difference_type index = positin - start;//插入点之前的元素个数
	value_type x_copy = x;
	if (index<(size()/2)){//如果插入点之前元素个数比较少
		push_front(front());
		iterator front1 = start;//一下标示记号，然后进行元素移动
		++front1;
		iterator front2 = front1;
		++front2;               //前两个元素相同，标记从第三个元素为被copy的起始位置
		pos = start + index;
		iterator pos1 = pos;
		++pos1;//计算原始position的位置
		copy(front2,pos1,front1);
	}else{//插入点之后的元素个数比较少
		push_back(back());//在尾端元素加入与最后元素相同的元素
		iterator back1 = finish;
		--back1;
		iterator back2 = back1;
		--back2;
		pos = start + index;
		copy_backward(pos,back2,back1);//元素移动
	}
	*pos = x_copy;//在插入点设定新值
	return pos;
}