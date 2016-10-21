//__STL_FUNCTION_TEMPLATE_PARTIAL_ORDER
#include "stdafx.h"
#include <iostream>
using namespace std;

class alloc{};

template<typename T,typename Alloc=alloc>
class vector{
public:
	void swap(vector<T, Alloc>&){ cout << "swap();" << endl; }
};

template<typename T,typename Alloc>
inline void swap(vector<T,Alloc>& x,vector<T,Alloc>& y){
	x.swap(y);
}

//void ::test(){
//	vector<int> x, y;
//	swap(x,y);
//}