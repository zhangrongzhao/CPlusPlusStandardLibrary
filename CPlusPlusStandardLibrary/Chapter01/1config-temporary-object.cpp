#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

template<typename T>
class print{
public :
	void operator()(const T& elem){
		cout << elem << ' ' ;
	}
};


//int _tmain(int argc, _TCHAR* argv[])
//{
//	int ia[6] = {0,1,2,3,4,5};
//	vector<int> iv(ia,ia+6);
//
//	for_each(iv.begin(),iv.end(),print<int>());
//
//	return 0;
//}
