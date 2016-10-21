//__STL_TEMPLATE_NULL
//class template explicit specialization
#include "stdafx.h"
//#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
//#   define __STL_TEMPLATE_NULL template<>
//#else
//#   define __STL_TEMPLATE_NULL
//#endif
//template<typename T> struct __type_traits{ };
//__STL_TEMPLATE_NULL struct __type_traits<char>{ };
//template<> struct __type_traits<char>{};
//
//template<typename T> struct hash{ };
//__STL_TEMPLATE_NULL struct hash<char>{};
//template<> struct hash<char>{};
//__STL_TEMPLATE_NULL struct hash<unsigned char>{};
//template<> struct hash<unsigned char>{};
#include <iostream>
using namespace std;

template<typename T> 
struct container{
	void operator()(){ cout << "hash<T>"<< endl; }
};

//explicit specialization
//__STL_TEMPLATE_NULL struct hash<char>{
//	void operator()(){ cout << "hash<char>"<< endl; }
//};
template<> struct container<char>{
	void operator()(){ cout << "hash<char>" << endl; }
};

//__STL_TEMPLATE_NULL struct hash<unsigned char>{
//	void operator()(){ cout <<"hash<unsigned char>" << endl; }
//};
template<>  struct container<unsigned char>{
	void operator()(){ cout << "hash<unsigned char>" << endl; }
};

//void test(){
//	hash<long> t1;
//	hash<char> t2;
//	hash<unsigned char> t3;
//	t1();
//	t2();
//	t3();
//}