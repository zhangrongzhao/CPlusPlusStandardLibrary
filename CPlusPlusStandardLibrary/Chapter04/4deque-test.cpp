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
//	//Ϊÿ��Ԫ���趨��ֵ
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
//	//��β�˼�3��Ԫ��
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
//	//����β������1��Ԫ�أ���ֵΪ3
//	ideq.push_back(3);
//	for (int i = 0; i < ideq.size();++i)
//	{
//		cout << ideq[i] << ' ';
//	}
//	cout << "size=" << ideq.size() << endl;;
//
//	//����β������һ��Ԫ�أ���ֵΪ99
//	ideq.push_front(99);
//	for (int i = 0; i < ideq.size(); ++i)
//	{
//		cout << ideq[i] << ' ';
//	}
//	cout << "size=" << ideq.size() << endl;
//
//	//����β������һ��Ԫ�أ���ֵΪ99
//	ideq.push_front(98);
//	ideq.push_front(97);
//	for (int i = 0; i < ideq.size(); ++i)
//	{
//		cout << ideq[i] << ' ';
//	}
//	cout << "size=" << ideq.size() << endl;
//
//
//    //��Ѱ��ֵΪ99��Ԫ�أ�����ӡ����
//	deque<int>::iterator itr;
//	itr = find(ideq.begin(),ideq.end(),99);
//	cout << *itr << endl;
//	//cout << *(itr) << endl;
//}