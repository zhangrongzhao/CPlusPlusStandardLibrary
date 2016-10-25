#include "stdafx.h"

//#include <vector>
//#include <list>
//#include <deque>
//#include <algorithm>
//#include <iostream>
//
//using namespace std;
//
//void main(){
//	const int arraySize = 7;
//	int ia[arraySize] = {0,1,2,3,4,5,6};
//
//	vector<int> ivec(ia,ia+arraySize);
//	list<int> ilist(ia,ia+arraySize);
//	deque<int> ideque(ia,ia+arraySize);
//
//	vector<int>::iterator it1 = find(ivec.begin(),ivec.end(),4);
//	if (it1 == ivec.end()){
//		cout <<"4 not found." << endl;
//	}else{
//		cout <<"4 found" << *it1 << endl;
//	}
//
//	list<int>::iterator it2 = find(ilist.begin(),ilist.end(),6);
//	if (it2 == ilist.end())	{
//		cout << "6 not found." << endl;
//	}else{
//		cout << "6 found" << *it2 << endl;
//	}
//
//	deque<int>::iterator it3 = find(ideque.begin(),ideque.end(),8);
//	if (it3 == ideque.end()){
//		cout << "8 not found." << endl;
//	}else{
//		cout << "8 found" << *it3 << endl;
//	}
//}
//
//void func(){
//	auto_ptr<string> ps(new string("hello world!"));
//	cout << *ps << endl;
//	cout << ps->size() << endl;
//	//离开前不需要delete,auto_ptr自动释放内存
//}