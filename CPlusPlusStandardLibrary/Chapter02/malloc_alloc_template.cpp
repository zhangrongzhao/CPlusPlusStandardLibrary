#include "stdafx.h"
//һ��������__malloc_alloc_template����

#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined (__THROW_BAD_ALLOC)
#   include <iostream>
#   define __THROW_BAD_ALLOC cerr<<"out of memory"<<endl; exit(1);
#endif

//��һ��������
template<int inst>
class __malloc_alloc_template{
private:
	//out of memory
	static void* oom_malloc(size_t);
	static void* oom_realloc(void*,size_t);
	static void (* __malloc_alloc_oom_handler)();//ָ��ָ����
public:
	static void* allocate(size_t n){
		void* result = malloc(n);//��һ����������ֱ��ʹ��malloc()
		if (result == 0){ //�������ʧ�ܣ������oom_malloc
			result = oom_malloc(n);
		}
		return result;
	}

	static void deallocate(void* p,size_t /*n*/){
		free(p);//��һ��������ֱ��ʹ��free.
	}

	static void* reallocate(void* p,size_t/*old_sz*/,size_t new_sz){
		void* result = realloc(p,new_sz);
		if (result == 0){
			result = oom_realloc(p,new_sz);
		}
		return result;
	}

	//����C++��set_new_handler(),�Զ���out-of-memory handler
	static  void(* set_malloc_handler(void(*f)()))(){//???????????????????????
		void(*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}
};

//malloc_alloc out-of-memory handling
//��ֵΪ0,���ͻ����趨
template<int inst>
void(* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n){
	void(* my_malloc_handler)();
	void* result;

	for (;;){ //���ϳ����ͷţ����ã����ͷţ�������
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler){ __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();//���ô������̣���ͼ�ͷ��ڴ�
		result = malloc(n);//�ٴγ��������ڴ档
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
		(*my_malloc_handler)();//���ô������̣���ͼ�ͷ��ڴ�
		result = realloc(p,n); //�ٴγ��������ڴ�
		if (result){ return (result); }
	}
}

//ע�⣬һ��ֱ�ӽ�����instָ��Ϊ0
typedef __malloc_alloc_template<0> malloc_alloc;