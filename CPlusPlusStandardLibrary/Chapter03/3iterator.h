#include "stdafx.h"
#include <cstdlib>
//���������������tag class,Ŀ������function template ��argument deducation.
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag :public input_iterator_tag{};
struct bidirectional_iterator_tag :public forward_iterator_tag{};
struct random_access_iterator_tag :public bidirectional_iterator_tag{};


//��׼iterator
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

//iterator traits,һ��汾,��Դ���iterator Ϊclass type��
template<typename Iterator>
struct iterator_traits{
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::category iterator_category;
};

//iteratorΪԭ��ָ��pointerʱ���ṩpointer�ػ��汾
template<typename T>
struct iterator_traits<T*>{
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;
};

//iterator Ϊpointer-to-const,�ṩpointer-to-const�ػ��汾
template<typename T>
struct iterator_traits<const T*>{
	typedef T value_type;
	typedef const T& reference;
	typedef const T* pointer;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;
};

//��ȡ����������
template<typename Iterator>
inline iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&){
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}

//��ȡĳ����������difference type.
