#include "stdafx.h"
//
//#include <deque>
//#include <iostream>
//#include <algorithm>
//using namespace std;
//
//int main(){
//	deque<int> ideq(20,9);
//	cout << "size=" << ideq.size() << endl;
//
//	//为每个元素设定新值
//	for (int i = 0; i < ideq.size();++i)
//	{
//		ideq[i] = i;
//	}
//
//	for (int i = 0; i < ideq.size();++i)
//	{
//		cout << ideq[i] << ' ';
//	}
//	cout << endl;
//
//	//在尾端加3个元素
//	for (int i = 0; i < 3;i++){
//		ideq.push_back(i);
//	}
//
//	for (int i = 0; i < ideq.size();++i)
//	{
//		cout << ideq[i] << ' ';
//	}
//	cout << endl;
//	cout << "size=" << ideq.size() << endl;
//
//	//在最尾端增加1个元素，其值为3
//	ideq.push_back(3);
//	for (int i = 0; i < ideq.size();++i)
//	{
//		cout << ideq[i] << ' ';
//	}
//	cout << "size=" << ideq.size() << endl;;
//
//	//在最尾端增加一个元素，其值为99
//	ideq.push_front(99);
//	for (int i = 0; i < ideq.size(); ++i)
//	{
//		cout << ideq[i] << ' ';
//	}
//	cout << "size=" << ideq.size() << endl;
//
//	//在最尾端增加一个元素，其值为99
//	ideq.push_front(98);
//	ideq.push_front(97);
//	for (int i = 0; i < ideq.size(); ++i)
//	{
//		cout << ideq[i] << ' ';
//	}
//	cout << "size=" << ideq.size() << endl;
//
//
//    //搜寻数值为99的元素，并打印出来
//	deque<int>::iterator itr;
//	itr = find(ideq.begin(),ideq.end(),99);
//	cout << *itr << endl;
//	//cout << *(itr) << endl;
//}