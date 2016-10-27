#include "stdafx.h"
#include <cstddef>

//ԭ��function template�Ĳ����Ƶ����ƣ�ֻ���Ƶ��������޷��Ƶ�����ֵ��
//�������������ֵ��ʹ�õ�������nested type����Ƕ���ͽ��

template<typename T>
struct MyIterator{
	typedef T value_type;
	T* pointer;
	MyIterator(T* p = 0) :pointer(p){ }
	T& operator*() const{ return *pointer; }
};

//template<typename I>
//typename I::value_type
//func(I iterator){
//	return *iterator;
//}

//void main(){
//	MyIterator<int> iterator(new int(8));
//	func(iterator);
//}

//���⣺value_type����Ƕ���ͣ���������class type���ܶ�����Ƕ���͡����ǵ��������Ƕ���class type���磺ԭ��ָ�룬�޷���ԭ��ָ���ڲ��ڶ�����Ƕ���͡�
//����STL�������ԭ��ָ����Ϊһ�ֵ����������Ա�����һ�㻯�������ԭ��ָ�������⻯����
//���������template partial specialization ģ��Ĳ����޶����ṩ��һ��template���塣
//���template��������һ����������������Ƴ�����һ���ػ��汾��

template<typename T>
class C{};

template<typename T>
class C<T*>{ };//�޶�һ������е�TΪԭ��ָ�룬����һ�������ģ�����T��һ���޶�Ϊԭ��ָ�롣�ṩ����һ��ģ�塣

template<typename T>
class C<const T*>{ };//��һ���޶�һ������е�TΪָ������ָ�롣

template<typename I>
struct iterator_traits{ //traits����
	typedef typename I::value_type value_type;
	typedef typename I::pointer pointer;
	typedef typename I::reference reference;
	typedef typename I::difference_type difference_type;
	typedef typename I::iterator_category iterator_category;
};

template<typename I>
struct iterator_traits<I*>{
	typedef I value_type;
	typedef I* pointer;
	typedef I& reference;
	typedef ptrdiff_t difference_type;
	//typedef iterator_category;
};

template<typename I>
struct iterator_traits<const I*>{
	typedef I value_type;
	typedef const I* pointer;
	typedef const I& reference;
	typedef ptrdiff_t difference_type;
	//typedef iterator_category;
};


template<typename I>
typename iterator_traits<I>::value_type//����ֵʹ�õ���������������ȡ
func(I iterator){
	return *iterator;
}

//������associated type ֮ difference type. difference_type������ָ��֮��Ĳ�ࡣͷβ֮��ľ����ʾ���������
template<typename I,typename T>
typename iterator_traits<I>::difference_type
count(I first,I last,const T& value){
	typename iterator_traits<I>::difference_type n = 0;
	for (; first != last;++first){
		if (*first == value){
			++n;
		}
	}
	return n;
}