#include "stdafx.h"
//一级配置器__malloc_alloc_template剖析

#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined (__THROW_BAD_ALLOC)
#   include <iostream>
#   define __THROW_BAD_ALLOC cerr<<"out of memory"<<endl; exit(1);
#endif

//第一级配置器
template<int inst>
class __malloc_alloc_template{
private:
	//out of memory
	static void* oom_malloc(size_t);
	static void* oom_realloc(void*,size_t);
	static void (* __malloc_alloc_oom_handler)();//指针指向函数
public:
	static void* allocate(size_t n){
		void* result = malloc(n);//第一级别配置器直接使用malloc()
		if (result == 0){ //如果分配失败，则改用oom_malloc
			result = oom_malloc(n);
		}
		return result;
	}

	static void deallocate(void* p,size_t /*n*/){
		free(p);//第一级配置器直接使用free.
	}

	static void* reallocate(void* p,size_t/*old_sz*/,size_t new_sz){
		void* result = realloc(p,new_sz);
		if (result == 0){
			result = oom_realloc(p,new_sz);
		}
		return result;
	}

	//仿真C++的set_new_handler(),自定义out-of-memory handler
	static  void(* set_malloc_handler(void(*f)()))(){//???????????????????????
		void(*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}
};

//malloc_alloc out-of-memory handling
//初值为0,待客户端设定
template<int inst>
void(* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n){
	void(* my_malloc_handler)();
	void* result;

	for (;;){ //不断尝试释放，配置，再释放，在配置
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler){ __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();//调用处理例程，企图释放内存
		result = malloc(n);//再次尝试配置内存。
		if (result){ return (result); }
	}
}

template<int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void* p,size_t n){
	void(* my_malloc_handler)();
	void* result;
	for (;;){
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (my_malloc_handler == 0){ __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();//调用处理例程，企图释放内存
		result = realloc(p,n); //再次尝试配置内存
		if (result){ return (result); }
	}
}

//注意，一下直接将参数inst指定为0
typedef __malloc_alloc_template<0> malloc_alloc;