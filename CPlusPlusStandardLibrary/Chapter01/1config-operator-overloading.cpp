#include "stdafx.h"

#include <iostream>
using namespace std;

class INT{
	friend ostream& operator << (ostream& os,const INT& i);
public:
	INT(int i) :m_i(i){};

	//prefix:increment and then fetch
	INT& operator++(){
		++(this->m_i);
		return *this;
	}

	//postfix:fetch and then increment
	const INT operator++(int){
		INT temp = *this;
		++(*this);
		return temp;
	}

	//prefix:decrement and then fetch
	INT& operator--(){
		--(this->m_i);
		return *this;
	}

	//postfix:fetch and then decrement
	const INT operator--(int){
		INT temp = *this;
		--(this->m_i);
		return temp;
	}

	//dereference
	int& operator*(){
		return (int&)m_i;
	}
private:
	int m_i;
};

ostream& operator << (ostream& os, const INT& i){
	os << '[' << i.m_i << ']';
	return os;
}

//void main(){
//	INT I(5);
//
//	cout << I++ << endl;
//	cout << ++I << endl;
//	cout << I-- << endl;
//	cout << --I << endl;
//	cout << *I << endl;
//}