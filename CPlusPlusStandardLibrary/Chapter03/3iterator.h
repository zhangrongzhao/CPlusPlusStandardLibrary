#include "stdafx.h"
#include <cstdlib>
//创建迭代器标记类tag class,目的用于function template 的argument deducation.
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag :public input_iterator_tag{};
struct bidirectional_iterator_tag :public forward_iterator_tag{};
struct random_access_iterator_tag :public bidirectional_iterator_tag{};


//标准iterator
template<typename Category,
         typename T,
         typename Distance=ptrdiff_t,
         typename Pointer=T*,
         typename Reference=T&>
struct iterator{
	typedef Category iterator_Category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef Pointer pointer;
	typedef Reference reference;
};

//iterator traits,一般版本,针对处理iterator 为class type。
template<typename Iterator>
struct iterator_traits{
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::category iterator_category;
};

//iterator为原生指针pointer时，提供pointer特化版本
template<typename T>
struct iterator_traits<T*>{
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;
};

//iterator 为pointer-to-const,提供pointer-to-const特化版本
template<typename T>
struct iterator_traits<const T*>{
	typedef T value_type;
	typedef const T& reference;
	typedef const T* pointer;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;
};

//获取迭代器种类
template<typename Iterator>
inline iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&){
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}

//获取某个迭代器的difference type.
