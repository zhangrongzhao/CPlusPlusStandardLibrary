#include "stdafx.h"

#include <list>
#include <iostream>
#include <algorithm>

using namespace std;

int main(){
	//int i;
	list<int> iList;
	cout << "size=" << iList.size() << endl;

	iList.push_back(0);
	iList.push_back(1);
	iList.push_back(2);
	iList.push_back(3);
	iList.push_back(4);
	cout << "size=" << iList.size() << endl;

	list<int>::iterator ite;
	for (ite = iList.begin(); ite !=iList.end();++ite){
		cout << *ite << ' ';
	}
	cout << endl;

	ite = find(iList.begin(),iList.end(),3);
	if (ite != iList.end()){
		iList.insert(ite,99);
	}

	cout << "size=" << iList.size() << endl;
	cout << *ite << endl;

	for (ite = iList.begin(); ite != iList.end();++ite){
		cout << *ite << ' ';
	}
	cout << endl;

	ite = find(iList.begin(),iList.end(),1);
	if (ite != iList.end()){
		cout << *(iList.erase(ite)) << endl;
	}

	for (ite = iList.begin(); ite != iList.end();++ite){
		cout << *ite << ' ';
	}
	cout << endl;
}

