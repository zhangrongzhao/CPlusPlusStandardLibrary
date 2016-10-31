#include "stdafx.h"
#include <cstdlib>
#include "../Chapter02/2stdalloc.cpp"
//alloc是SGI STL的空间配置器
class alloc{};

template<typename T,typename Alloc=alloc>
class vector{
public:
	//vector的嵌套类型定义
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected:
	//以下，simple_alloc是SGI STL的空间配置器，见2.2.4
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;//表示目前使用空间的头
	iterator finish;//表目前使用空间的尾
	iterator end_of_storage;//表示目前可用空间的尾
	void insert_aux(iterator position, const T& x);
	void deallocate(){
		if (start){
			data_allocator::deallocate(start,end_of_storage-start);
		}
	}
	void fill_initialize(size_type n,const T& value){
		start = allocate_and_fill(n,value);
		finish = start + n;
		end_of_storage = finish;
	}
public:
	iterator begin(){ return start; }
	iterator end(){ return finish; }
	size_type size() const{ return size_type(end()-begin()); }
	size_type capacity() const{return size_type(end_of_storage-begin())};
	bool empty() const{ return begin() == end(); }
	reference operator[](size_type n){ return *(begin()+n); }

	vector() :start(0), finish(0), end_of_storage(0){ }
	vector(size_type n, const T& value){ fill_initialize(n,value); }
	vector(int n, const T& value){ fill_initialize(n,value); }
	vector(long n, const T& value){ fill_initialize(n,value); }
	explicit vector(size_type n){ fill_initialize(n,T()); }

	~vector(){
		destroy(start,finish);//销毁对象
		deallocate();//归还内存
	}

	reference front(){ return *begin(); }
	reference back(){ return *(end()-1); }
	void push_back(const T& x){//将元素插入最尾端
		if (finish!=end_of_storage){//还有备用空间
			construct(finish,x);//全局函数
			++finish;
		}else{//空间满载，扩容后，插入尾端
			insert_aux(end(),x);
		}
	}
	void pop_back(){//取出尾端元素
		--finish;
		destory(finish);
	}

	iterator erase(iterator position){//清除某个位置上的元素
		if ((position+1)!=end()){
			copy(position+1,finish,position);//后续元素往前移动
		}
		--finish;
		destory(finish);
		return position;
	}

	void resize(size_type new_size,const T& x){
		if (new_size<size()){
			erase(begin()+new_size,end());
		}else{
			insert(end(),new_size-size(),x);
		}
	}

	void resize(size_type new_size){ resize(new_size,T()); }
	void clear(){ erase(begin(),end()); }
protected:
	//配置空间并填满内容
	iterator allocate_end_fill(size_type n,const T& x){
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result,n,x);//全局函数
		return result;
	}
};

template<typename T,typename Alloc>
void vector<T,Alloc>::insert_aux(iterator position,const T& x){
	if (finish!=end_of_storage){//还有备用空间
		construct(finish,*(finish-1));//备用空间起始处构造一个元素，并以vector最后一个元素为初值
		++finish;//调整水位
		T x_copy = x;
		copy_backward(position,finish-2,finish-1);
		*position = x_copy;
	}else{//已无备用空间

	}
}


////测试代码
//class Shape{};
//void main(){
//	vector<int>::iterator iviterator;
//	vector<Shape>::iterator sviterator;
//}