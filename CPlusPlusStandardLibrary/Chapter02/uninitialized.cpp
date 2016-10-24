#include "stdafx.h"
//
///******************************uninitialized_copy:begin**********************************************/
//template<typename InputIterator,typename ForwardIterator>
//inline ForwardIterator uninitialized_copy(InputIterator first,InputIterator last, ForwardIterator result){
//	return __uninitialized_copy(first,last,result,value_type(result));
//}
//
//template<typename InputIterator,typename ForwardIterator,typename T>
//inline ForwardIterator __uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,T*){
//	typedef typename __type_traits<T>::is_POD_type is_POD;
//	return __uninitialized_copy_aux(first,last,result,is_POD());//使用编译器做参数推导
//}
//
//template<typename InputIterator,typename ForwardIterator>
//inline ForwardIterator __uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,__true_type){
//	return copy(first,last,result);//调用STL算法copy()
//}
//
//template<typename InputIterator,typename ForwardIterator>
//inline ForwardIterator __uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,__false_type){
//	ForwardIterator cur = first;
//	for (; first != last;++first,++cur){
//		construct(&*cur,*first);//一个一个构造。
//	}
//	return cur;
//}
//
//////const char* 特化
////inline char* uninitialized_copy(const char* first,const char* last,char* result){
////	memmove(result,first,last-first);
////	return result + (last-first);
////}
////
//////cont wchar_t*特化
////inline wchar_t* uninitialized_copy(const wchar_t* first,const wchar_t* last,wchar_t* result){
////	memmove(result,first,sizeof(wchar_t)*(last-first));
////	return result + (last-first);
////}
///******************************uninitialized_copy:end************************************************/
///******************************uninitialized_fill:begin**********************************************/
//template<typename ForwardIterator,typename T>
//inline void uninitialized_fill(ForwardIterator first,ForwardIterator last, const T& x){
//	__uninitialized_fill(first,last,x,value_type(first));
//}
//
//template<typename ForwardIterator,typename T>
//inline void __uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& x,T*){
//	typedef typename __type_traits<T>::is_POD_type is_POD;
//	__uninitialized_fill_aux(first,last,x,is_POD());
//}
//
//template<typename ForwardIterator,typename T>
//inline void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& x,__true_type){
//	fill(first,last,x);//调用STL算法fill()
//}
//
//template<typename ForwardIterator,typename T>
//inline void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& x,__false_type){
//	ForwardIterator cur = first;
//	for (; cur != last;++cur){
//		construct(&*cur,x);
//	}
//}
//
///******************************uninitialized_fill:end************************************************/
///******************************uniniatialized_fill_n:begin*******************************************/
//template<typename ForwardIterator,typename Size,typename T>
//inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& x){
//	return __uninitialized_fill_n(first,n,x,value_type(first));//利用value_type()取出first的value type
//}
//
//template<typename ForwardIterator,typename Size,typename T,typename T1>
//inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,typename Size,const T& x,T1*){
//	typedef typename __type_traits<T1>::is_POD_type is_POD;
//	return __uninitialized_fill_n_aux(first,n,x,is_POD());
//}
//
////POD:Plain Old Data,POD型别拥有 trivial constructor/destructor/copy/assignment函数
////如果是POD型别，执行流程转进该函数。由function template参数推导机制而得
//template<typename ForwardIterator,typename Size,typename T>
//inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& x,__true_type){
//	return fill_n(first,n,x);//交由高阶函数执行
//}
//
//template<typename ForwardIterator,typename Size,typename T>
//inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& x,__false_type){
//	ForwardIterator cur = first;
//	for (; n > 0;--n,cur++){
//		construct(&*cur,x);
//	}
//	return cur;
//}
//
///*******************************uniniatialized_fill_n:end*********************************************/
