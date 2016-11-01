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
	void pop_back(){//擦除尾端元素
		--finish;//将尾端元素往前移动一格，表示放弃尾端元素
		destory(finish);//销毁尾端元素。destory全局函数
	}

	iterator erase(iterator position){//清除某个位置上的元素
		if ((position+1)!=end()){
			copy(position+1,finish,position);//后续元素往前移动
		}
		--finish;
		destory(finish);
		return position;
	}
	iterator erase(iterator first,iterator last){
		iterator i = copy(last,finish,first);//copy为全局函数,将last到finish之间的元素copy的first为起始位置处。
		destory(i,finish);//再将i到finish之间的元素销毁。
		finish = finish - (last-first);
		return first;
	}

	void resize(size_type new_size,const T& x){
		if (new_size<size()){
			erase(begin()+new_size,end());
		}else{
			insert(end(),new_size-size(),x);
		}
	}
	void resize(size_type new_size){ resize(new_size,T()); }

	void insert(iterator position,size_type n,const T& x);

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
		const size_type old_size = size();
		const size_type len = old_size != 0 ? 2 * old_size : 1;
		//配置原则：如果原大小为0，则配置1（个元素大小）
		//如果原大小不为0，则配置原大小的2倍，
		//前半段存放原数据，后半段准备放置新数据

		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;
		try{
		    //将原vector的内容拷贝到新的vector
			new_finish = uninitialized_copy(start,position,new_start);
			//为新元素设定初值x
			construct(new_finish,x);
			//调整水位
			++new_finish;
			//将安插点的原内容copy过来（提示，本函数也可能被insert(p,x)调用）
			new_finish = uninitialized_copy(position,finish,new_finish);//??????这里不明白为什么要这样操作
		}
		catch (...){
			//"commit or rollback"
			destory(new_start,new_finish);
			data_allocator::deallocate(new_start,len);
			throw;
		}

		destory(begin(),end());
		deallocate();

		//调整迭代器，指向新的vector
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}
}

template<typename T,typename Alloc>
void vector<T, Alloc>::insert(iterator position,size_type n,const T& x){
	if (n!=0){
		if (size_type(end_of_storage-finish)>=n){//备用空间大于等于“新增元素的个数”
			T x_copy = x;
			//以下计算插入点之后现有元素个数
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n){
				//“插入点之后的现有元素个数”>“新增元素个数”
				uninitialized_copy(finish-n,finish,finish);//复制finish之前的n个元素到finish之后。
				finish += n;//尾端标记后移
				copy_backward(position,old_finish-n,old_finish);//复制position到old_finish-n出的元素copy到old_finish之前。
				fill(position,position+n,x_copy);//从插入点开始填入新值
			}else{
				//"插入点之后的现有元素个数"<="新增元素个数"
				uninitialized_fill_n(finish,n-elems_after,x_copy);
				finish += n - elems_after;
				uninitialized_copy(position,old_finish,finish);
				finish += elems_after;
				fill(position,old_finish,x_copy);
			}
		}
	}else{
		//备用空间小于"新增元素个数",(需要配置额外内存)
		//首先决定新长度：就长度的2倍，或旧长度+新增元素个数
		const size_type old_size = size();
		const size_type len = old_size + max(old_size,n);

		//以下配置新的vector空间
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;

		__STL_TRY{
		    //以下首先将旧vector插入点之前的元素复制到新空间
			new_finish = uninitialized_copy(start,position,new_start);
			//以下将新增元素（初值皆为n）填入新空间
			new_finish = uninitialized_fill_n(new_finish,n,x);
			//以下再将vector插入点之后的元素复制到新空间
			new_finish = uninitialized_copy(position,finish,new_finish);
		}
        #ifdef  __STL_USE_EXCEPTIONS
		catch (...){
		    //如果异常，实现"commit or rollback"
			destory(new_start,new_finish);
			data_allocator::deallocate(new_start,len);
			throw;
	    }
        #endif /*__STL_USE_EXCEPTIONS*/

		//以下清除并释放旧的vector
		destory(start,finish);
		deallocate();
		//以下调整水位
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}
}


////测试代码
//class Shape{};
//void main(){
//
//}