#include "stdafx.h"
#include <new.h>
//class Foo{};
//
//void main(){
//	Foo* pf = new Foo();//两阶段：(1)调用::operator new分配内存空间，(2)对内存空间初始化，构造对象内容
//	delete pf; //两阶段：(1)Foo:~Foo()析构函数，释放对象所占用的资源，(2)调用::operator delete释放内存。
//}

//STL: (1)alloc:allocate()分配内存；(2)alloc:deallocate()释放内存
//     (1)::construct()构造对象;(2)::destory()销毁对象

template<typename T1,typename T2>
inline void contruct(T1* p,const T2& value){
	new (p)T1(value);//placement new(可以在已分配内存空间上，初始化对象);调用T1::T1(value)
}

//destory第一个版本
template<typename T>
inline void destroy(T* pointer){
	pointer->~T();
}

//detory第二个版本
template<typename ForwardIterator>
inline void destory(ForwardIterator first,ForwardIterator last){
	__destory(first,last,value_type(first));
}

template<typename ForwardIterator,typename T>
inline void __destory(ForwardIterator first,ForwardIterator last,T*){
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;//判断是否有默认析构函数(trivial destructor)
	__destory_aux(first,last,trivial_destructor());
}

template<typename ForwardIterator>
inline void __destory_aux(ForwardIterator first,ForwardIterator last,__false_type){//没有默认析构函数(non-trivial destructor)
	for (; first < last; ++first){
		destory(&*first);
	}
}

template<typename ForwardIterator>
inline void __destory_aux(ForwardIterator first,ForwardIterator last,__true_type){//有默认的析构函数(trivial destructor)
	//do nothing
}

//对第二个版本，迭代器特化char* 和wchar_t*
inline void destory(char*,char*){ }
inline void destory(wchar_t*,wchar_t*){}

template<typename T,typename Alloc>
class simple_alloc{
public:
	static T* allocate(size_t n){
		return n == 0 ? 0 : (T*)Alloc::allocate(n*sizeof(T));
	}

	static T* allocate(void){
		return (T*)Alloc::allocate(sizeof(T));
	}

	static void deallocate(T* p,size_t n){
		if (n != 0){
			Alloc::deallocate(p,n*sizeof(T));
		}
	}

	static void deallocate(T* p){
		Alloc::deallocate(p,sizeof(T));
	}
};


template<typename T,typename Alloc=alloc>
class vector{
protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;
	void deallocator(){
		if (...){
			data_allocator::deallocate(start,end_of_storage-start);
		}
	}
};
