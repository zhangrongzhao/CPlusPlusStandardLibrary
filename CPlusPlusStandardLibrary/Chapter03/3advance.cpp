#include "stdafx.h"
#include <cstdlib>
using namespace std;

template<typename InputIterator,typename Distance>
void advance_II(InputIterator& i,Distance n){
    //单向，逐一前进
	while (n--){
		++i;
	}
}

//template<typename ForwardIterator,typename Distance>
//void advance_FI(ForwardIterator& f,Distance n){
//    //单向,逐一前进
//	while (n--){
//		++f;
//	}
//}

template<typename BidirectionalIterator,typename Distance>
void advance_BI(BidirectionalIterator& b,Distance n){
	//双向，逐一前进
	if (n >= 0){
		while (n--){ ++b; }
	}else{
		while (n++){ --b; }
	}
}

template<typename RandomAccessIterator,typename Distance>
void advance_RAI(RandomAccessIterator& rai,Distance n){
	//双向，跳跃前进
	rai += n;
}

////三者合一
////问题：运行时判断迭代器类型，影响效率。
////解决方法：应该在编译期选择正确的版本。
////解决方案：函数模板重载，解决编译器期选择正确版本。
//template<typename InputIterator,typename Distance>
//void advance(InputIterator& i,Distance n){
//	if (is_random_access_iterator(i)){//此函数有待设计，运行时判断迭代器类型，影响效率。应该在编译期选择正确的版本。
//		advance_RAI(i,n);
//	}else if (is_bidirectional_iterator(i)){//此函数有待设计，运行时判断迭代器类型
//		advance_BI(i,n);
//	}else{
//		advance_II(i,n);
//	}
//}

//使用traits萃取出迭代器类型，作为advance第三个参数。利用函数模板的argument deducation,进行overloaded resolution
//参数必须为class type.
//定义5个class,代表五中迭代器
//五个作为标记的类型(tag class)
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag :public input_iterator_tag{ };
struct bidirectional_iterator_tag :public forward_iterator_tag{};
struct random_access_iterator_tag :public bidirectional_iterator_tag{};

template<typename InputIterator,typename Distance>
void __advance(InputIterator& i,Distance n,input_iterator_tag){
	while (n--){
		++i;
	}
}

template<typename ForwardIterator,typename Distance>
void __advance(ForwardIterator& i,Distance n,forward_iterator_tag){
    //单纯的进行传递调用(forwarding)
	_advance(i,n,input_iterator_tag());
}

template<typename BidirectionalIterator,typename Distance>
void __advance(BidirectionalIterator& i,bidirectional_iterator_tag){
	//双向，逐一前进
	if (n >= 0){
		while (n--){ ++i; }
	}else{
		while (n++){ --i; }
	}
}

template<typename RandomAccessIterator,typename Distance>
void __advance(RandomAccessIterator& i,Distance n,random_access_iterator_tag){
	i += n;
}

template<typename InputIterator,typename Distance>
void advance(InputIterator& i,Distance n){
	__advance(i,n,iterator_traits<InputIterator>::iterator_category());//产生临时对象，其类型是四个迭代器类型之一(InputIterator,ForwardIterator,BidirectionalIterator,RandomAccessIterator)
}

/************************iterator traits**********************************************/
template<typename I>
struct iterator_traits{ //traits特性
	typedef typename I::value_type value_type;
	typedef typename I::pointer pointer;
	typedef typename I::reference reference;
	typedef typename I::difference_type difference_type;
	typedef typename I::iterator_category iterator_category;
};

template<typename T>
struct iterator_traits<T*>{
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;//原生指针是一种random access iterator
};

template<typename T>
struct iterator_traits<const T*>{
	typedef T value_type;
	typedef const T* pointer;
	typedef const T& reference;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;//原生pointer-to-const是一种random access iterator
};
/************************iterator traits**********************************************/

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first,InputIterator last,input_iterator_tag){
	iterator_traits<InputIterator>::difference_type n = 0;
	//逐一累计距离
	while (first!=last){
		++first;
		++n;
	}
	return n;
}

template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first,RandomAccessIterator last,random_access_iterator_tag){
    //直接计算距离
	return last - first;
}

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first,InputIterator last){
	typedef typename iterator_traits<InputIterator>::iterator_category iterator_category;
	__distance(first,last,iterator_category());
}


template<typename Category,
         typename T,
         typename Distance=ptrdiff_t,
         typename Pointer=T*,
         typename Reference=T&>
struct iterator{
	typedef Category iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef Pointer pointer;
	typedef Reference reference;
};

//template<typename Item>
//struct ListIterator :public std::iterator<std::forward_iterator_tag, Item>{ };