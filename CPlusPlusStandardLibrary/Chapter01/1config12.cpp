#include "stdafx.h"
#include <iostream>
#include <cstddef>
using namespace std;

//#ifndef __STL_EXPLICIT_FUNTION_TMPL_ARGS
//#   define __STL_NULL_TMPL_ARGS <>
//#else
//#   define __STL_NULL_TEML_ARGS
//#endif
//
//template<typename T,typename Sequence = deque<T> >
//class stack{
//	friend bool operator== __STL_NULL_TMPL_ARGS(const stack&,const stack&);
//	friend bool operator< __STL_NULL_TMPL_ARGS(const stack&,const stack&);
//};

//template<typename T,typename Sequence = deque<T> >
//class stack{
//	friend bool operator==<>(const stack&,const stack&);
//	friend bool operator< <>(const stack&,const stack&);
//};

class alloc{};

template<typename T,typename Alloc=alloc,size_t BufSiz=0>
class deque{
public:
	deque(){ cout << "deque" << ' '; }
};

template<typename T,typename Sequence=deque<T> >
class stack{
public:
	//friend bool operator==<T>(const stack<T>&,const stack<T>&);
	//friend bool operator< <T>(const stack<T>&,const stack<T>&);

	//friend bool operator==<T>(const stack&,const stack&);
	//friend bool operator< <T>(const stack&,const stack&);

	//friend bool operator==<>(const stack&,const stack&);
	//friend bool operator< <>(const stack&,const stack&);

	/*friend bool operator==(const stack&,const stack&);
	friend bool operator< (const stack&,const stack&);*/

public:
	stack(){ cout << "stack" << endl; }
private:
	Sequence c;
};

//template<typename T,typename Sequence>
//bool operator==(const stack<T,Sequence>& x,const stack<T,Sequence>& y){
//	return cout << "operator==" << '\t';
//}
//
//template<typename T,typename Sequence>
//bool operator<(const stack<T,Sequence>& x,const stack<T,Sequence>& y){
//	return cout << "operator<" << '\t';
//}

//void test(){
//	stack<int> x;
//	stack<int> y;
//
//	cout << (x==y) << endl;
//	cout << (x<y) << endl;
//}