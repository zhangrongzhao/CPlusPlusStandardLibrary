#include "stdafx.h"
#include <cstdlib>
using namespace std;

template<typename InputIterator,typename Distance>
void advance_II(InputIterator& i,Distance n){
    //������һǰ��
	while (n--){
		++i;
	}
}

//template<typename ForwardIterator,typename Distance>
//void advance_FI(ForwardIterator& f,Distance n){
//    //����,��һǰ��
//	while (n--){
//		++f;
//	}
//}

template<typename BidirectionalIterator,typename Distance>
void advance_BI(BidirectionalIterator& b,Distance n){
	//˫����һǰ��
	if (n >= 0){
		while (n--){ ++b; }
	}else{
		while (n++){ --b; }
	}
}

template<typename RandomAccessIterator,typename Distance>
void advance_RAI(RandomAccessIterator& rai,Distance n){
	//˫����Ծǰ��
	rai += n;
}

////���ߺ�һ
////���⣺����ʱ�жϵ��������ͣ�Ӱ��Ч�ʡ�
////���������Ӧ���ڱ�����ѡ����ȷ�İ汾��
////�������������ģ�����أ������������ѡ����ȷ�汾��
//template<typename InputIterator,typename Distance>
//void advance(InputIterator& i,Distance n){
//	if (is_random_access_iterator(i)){//�˺����д���ƣ�����ʱ�жϵ��������ͣ�Ӱ��Ч�ʡ�Ӧ���ڱ�����ѡ����ȷ�İ汾��
//		advance_RAI(i,n);
//	}else if (is_bidirectional_iterator(i)){//�˺����д���ƣ�����ʱ�жϵ���������
//		advance_BI(i,n);
//	}else{
//		advance_II(i,n);
//	}
//}

//ʹ��traits��ȡ�����������ͣ���Ϊadvance���������������ú���ģ���argument deducation,����overloaded resolution
//��������Ϊclass type.
//����5��class,�������е�����
//�����Ϊ��ǵ�����(tag class)
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
    //�����Ľ��д��ݵ���(forwarding)
	_advance(i,n,input_iterator_tag());
}

template<typename BidirectionalIterator,typename Distance>
void __advance(BidirectionalIterator& i,bidirectional_iterator_tag){
	//˫����һǰ��
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
	__advance(i,n,iterator_traits<InputIterator>::iterator_category());//������ʱ�������������ĸ�����������֮һ(InputIterator,ForwardIterator,BidirectionalIterator,RandomAccessIterator)
}

/************************iterator traits**********************************************/
template<typename I>
struct iterator_traits{ //traits����
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
	typedef random_access_iterator_tag iterator_category;//ԭ��ָ����һ��random access iterator
};

template<typename T>
struct iterator_traits<const T*>{
	typedef T value_type;
	typedef const T* pointer;
	typedef const T& reference;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;//ԭ��pointer-to-const��һ��random access iterator
};
/************************iterator traits**********************************************/

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first,InputIterator last,input_iterator_tag){
	iterator_traits<InputIterator>::difference_type n = 0;
	//��һ�ۼƾ���
	while (first!=last){
		++first;
		++n;
	}
	return n;
}

template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first,RandomAccessIterator last,random_access_iterator_tag){
    //ֱ�Ӽ������
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