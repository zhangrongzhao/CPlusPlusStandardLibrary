#include "stdafx.h"
#include <cstddef>

//原因：function template的参数推导机制，只能推导参数，无法推导返回值。
//解决方法：返回值可使用迭代器的nested type，内嵌类型解决

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

//问题：value_type是内嵌类型，迭代器是class type才能定义内嵌类型。但是迭代器并非都是class type。如：原生指针，无法再原生指针内部在定义内嵌类型。
//但是STL必须接受原生指针作为一种迭代器。所以必须让一般化概念针对原生指针做特殊化处理。
//解决方案：template partial specialization 模板的部分限定，提供另一份template定义。
//针对template参数更进一步的条件限制所设计出来的一个特化版本。

template<typename T>
class C{};

template<typename T>
class C<T*>{ };//限定一般概念中的T为原生指针，即对一般概念中模板参数T进一步限定为原生指针。提供另外一份模板。

template<typename T>
class C<const T*>{ };//进一步限定一般概念中的T为指向常量的指针。

template<typename I>
struct iterator_traits{ //traits特性
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
typename iterator_traits<I>::value_type//返回值使用迭代器类型特性萃取
func(I iterator){
	return *iterator;
}

//迭代器associated type 之 difference type. difference_type：两个指针之间的差距。头尾之间的距离表示最大容量。
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