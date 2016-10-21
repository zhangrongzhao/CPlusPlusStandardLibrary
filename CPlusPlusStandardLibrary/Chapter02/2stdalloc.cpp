#include "stdafx.h"
#include <new.h>
//class Foo{};
//
//void main(){
//	Foo* pf = new Foo();//���׶Σ�(1)����::operator new�����ڴ�ռ䣬(2)���ڴ�ռ��ʼ���������������
//	delete pf; //���׶Σ�(1)Foo:~Foo()�����������ͷŶ�����ռ�õ���Դ��(2)����::operator delete�ͷ��ڴ档
//}

//STL: (1)alloc:allocate()�����ڴ棻(2)alloc:deallocate()�ͷ��ڴ�
//     (1)::construct()�������;(2)::destory()���ٶ���

template<typename T1,typename T2>
inline void contruct(T1* p,const T2& value){
	new (p)T1(value);//placement new(�������ѷ����ڴ�ռ��ϣ���ʼ������);����T1::T1(value)
}

//destory��һ���汾
template<typename T>
inline void destroy(T* pointer){
	pointer->~T();
}

//detory�ڶ����汾
template<typename ForwardIterator>
inline void destory(ForwardIterator first,ForwardIterator last){
	__destory(first,last,value_type(first));
}

template<typename ForwardIterator,typename T>
inline void __destory(ForwardIterator first,ForwardIterator last,T*){
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;//�ж��Ƿ���Ĭ����������(trivial destructor)
	__destory_aux(first,last,trivial_destructor());
}

template<typename ForwardIterator>
inline void __destory_aux(ForwardIterator first,ForwardIterator last,__false_type){//û��Ĭ����������(non-trivial destructor)
	for (; first < last; ++first){
		destory(&*first);
	}
}

template<typename ForwardIterator>
inline void __destory_aux(ForwardIterator first,ForwardIterator last,__true_type){//��Ĭ�ϵ���������(trivial destructor)
	//do nothing
}

//�Եڶ����汾���������ػ�char* ��wchar_t*
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
