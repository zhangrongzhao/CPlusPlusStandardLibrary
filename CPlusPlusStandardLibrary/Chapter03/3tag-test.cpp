#include "stdafx.h"
//仿真测试tag types继承关系所带来的影响
#include <iostream>
using namespace std;

struct B{};
struct D1 :public B{};
struct D2 :public D1{};

template<typename I>
void func(I& p,B){
	cout << "B version" << endl;
}

template<typename I>
void func(I& p,D2){
	cout << "D2 version"<< endl;
}

int main(){
	int* p;
	func(p,B());
	func(p,D1());
	func(p,D2());
}



