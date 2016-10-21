//__STL_MEMBER_TEMPLATES
//class template 之内可否再有template(members)
#include "stdafx.h"
#include <iostream>
using namespace std;

class alloc{};

template<typename T,typename Alloc=alloc>
class vector{
public :
	typedef T value_type;
	typedef value_type* iterator;

	template<typename I>
	void insert(iterator position,I first,I last){
		cout << "insert()" << endl;
	}
};

//void ::test(){
//	int ia[5] = { 0, 1, 2, 3, 4 };
//	vector<int> x;
//	vector<int>::iterator ite;
//	ite = ia;
//	x.insert(ite, ia, ia + 5);
//}

