//__STL_NON_TYPE_TEMPLATE_PARAM_BUG
#include "stdafx.h"
#include <iostream>
using namespace std;

class alloc{};

inline size_t __deque_buf_size(size_t n,size_t sz){
	return n != 0 ? n : (sz<512?size_t(512/sz):size_t(1));
}

template<typename T,typename Ref,typename Ptr,size_t BufSiz>
struct __deque_iterator{
	typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
	typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;

	static size_t buffer_size(){ return __deque_buf_size(BufSiz,sizeof(T)); }
};

template<typename T,typename Alloc=alloc,size_t BufSiz=0>
class deque{
public:
	typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
};

//void test(){
//	cout << deque<int>::iterator::buffer_size << endl;
//	cout << deque<int, alloc, 64>::iterator::buffer_size << endl;
//}