#include "stdafx.h"

#include <iostream>
using namespace std;

template<typename T>
struct testplus{
	T operator()(const T& x, const T& y) const{ return x + y; }
};

template<typename T>
struct testminus{
	T operator()(const T& x, const T& y) const { return x - y; };
};

//int main(){
//	testplus<int> plusobj;
//	testminus<int> minusobj;
//
//	cout << plusobj(3, 5) << endl;
//	cout << minusobj(3, 5) << endl;
//
//	cout << testplus<int>()(43, 50) << endl;
//	cout << testminus<int>()(43, 50) << endl;
//}