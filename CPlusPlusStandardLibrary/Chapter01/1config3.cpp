//class template ÖÐµÄ static data members.
// test __STL_STATIC_TEMPlATE_MEMBER_BUG,defined in <stl_config.h>
#include "stdafx.h"
#include <iostream>
using namespace std;

template <typename T>
class testClass
{
	public :
		static int _data;
};

int testClass<int>::_data = 1;
int testClass<char>::_data = 2;

void testClass_test()
{
	cout << testClass<int>::_data << endl;
	cout << testClass<char>::_data << endl;

}